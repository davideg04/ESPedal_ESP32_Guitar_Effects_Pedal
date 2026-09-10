#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include "AudioTools.h"

// --- I TUOI MODULI DIGITALI ---
#include "FuzzModule.h"
#include "OverdriveModule.h"
#include "TremoloModule.h"
#include "DelayModule.h"
#include "ReverbModule.h"
#include "LooperModule.h"
#include "EQModule.h"
#include "TunerModule.h"

// --- LA TUA INTERFACCIA WEB ---
#include "WebUI.h"

const char* ssid = "D.A.V.I.D.E_Pedal";  
const char* password = "rockandroll";    

WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

AudioInfo info(44100, 2, 32); 
I2SStream i2s_in;   
I2SStream i2s_out;  

// --- MEMORIA GLOBALE ---
float current_gain_multiplier = 1.0f; // 1.0 = Unity Gain (100%)
int32_t buffer[128]; // Low latency buffer

// Master DC Blocker State Variables
float dc_x1_L = 0, dc_y1_L = 0;
float dc_x1_R = 0, dc_y1_R = 0;
const float DC_R = 0.995f; 

// ======================================================================
// 1. CREAZIONE DEGLI OGGETTI (La tua Pedaliera Fisica)
// ======================================================================
OverdriveModule myOverdrive;
EQModule myEQ;
FuzzModule myFuzz;
TremoloModule myTremolo;
DelayModule myDelay;
ReverbModule myReverb;
LooperModule myLooper;
TunerModule myTuner;

// ======================================================================
// 2. WEBSOCKET ROUTING (Collega l'interfaccia agli oggetti C++)
// ======================================================================
int current_od_drive = 50, current_od_level = 50;
int current_eq_b = 50, current_eq_m = 50, current_eq_t = 50;
int current_fuzz_drive = 50, current_fuzz_mix = 50;
int current_trem_s = 50, current_trem_d = 50;
int current_delay_t = 50, current_delay_f = 50;
int current_rev_s = 50, current_rev_m = 50;

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  if (type == WStype_TEXT) {
    char cmd = payload[0];
    
    if (cmd == 'G') {
        int val = String((char*)&payload[1]).toInt();
        current_gain_multiplier = (float)val / 50.0f; // 50 UI = 1.0x Volume
    }
    else if (cmd == 'L') { 
        int val = String((char*)&payload[1]).toInt();
        myLooper.setLooperState(val);
    }

    else if (cmd == 'V') { 
        int val = String((char*)&payload[1]).toInt();
        myLooper.setVolume(val);
    }
    else if (cmd == 'W') { 
        int val = String((char*)&payload[1]).toInt();
        myLooper.setSpeed(val);
    }

    else {
      int eff = payload[1] - '0'; 
      int val = String((char*)&payload[2]).toInt();
      
      if (cmd == 'S' && eff == 0) myOverdrive.setEnabled(val == 1);
      else if (cmd == 'P' && eff == 0) { current_od_drive = val; myOverdrive.setParams(current_od_drive, current_od_level); }
      else if (cmd == 'Q' && eff == 0) { current_od_level = val; myOverdrive.setParams(current_od_drive, current_od_level); }

      else if (cmd == 'S' && eff == 1) myEQ.setEnabled(val == 1);
      else if (cmd == 'P' && eff == 1) { current_eq_b = val; myEQ.setParams(current_eq_b, current_eq_m, current_eq_t); }
      else if (cmd == 'Q' && eff == 1) { current_eq_m = val; myEQ.setParams(current_eq_b, current_eq_m, current_eq_t); }
      else if (cmd == 'R' && eff == 1) { current_eq_t = val; myEQ.setParams(current_eq_b, current_eq_m, current_eq_t); }

      else if (cmd == 'S' && eff == 2) myFuzz.setEnabled(val == 1);
      else if (cmd == 'P' && eff == 2) { current_fuzz_drive = val; myFuzz.setParams(current_fuzz_drive, current_fuzz_mix); }
      else if (cmd == 'Q' && eff == 2) { current_fuzz_mix = val; myFuzz.setParams(current_fuzz_drive, current_fuzz_mix); }

      else if (cmd == 'S' && eff == 3) myTremolo.setEnabled(val == 1);
      else if (cmd == 'P' && eff == 3) { current_trem_s = val; myTremolo.setParams(current_trem_s, current_trem_d); }
      else if (cmd == 'Q' && eff == 3) { current_trem_d = val; myTremolo.setParams(current_trem_s, current_trem_d); }

      else if (cmd == 'S' && eff == 4) myDelay.setEnabled(val == 1);
      else if (cmd == 'P' && eff == 4) { current_delay_t = val; myDelay.setParams(current_delay_t, current_delay_f); }
      else if (cmd == 'Q' && eff == 4) { current_delay_f = val; myDelay.setParams(current_delay_t, current_delay_f); }

      else if (cmd == 'S' && eff == 5) myReverb.setEnabled(val == 1);
      else if (cmd == 'P' && eff == 5) { current_rev_s = val; myReverb.setParams(current_rev_s, current_rev_m); }
      else if (cmd == 'Q' && eff == 5) { current_rev_m = val; myReverb.setParams(current_rev_s, current_rev_m); }

      else if (cmd == 'S' && eff == 6) myTuner.setEnabled(val == 1);
    }
  }
}

// ======================================================================
// 3. BACKGROUND RTOS TASKS (CORE 0)
// ======================================================================
void TaskWiFi(void *pvParameters) {
  for (;;) { 
      webSocket.loop(); 
      server.handleClient(); 
      vTaskDelay(10 / portTICK_PERIOD_MS); 
  }
}

unsigned long lastTunerUpdate = 0;
void TaskTunerMath(void *pvParameters) {
    for (;;) {
        if (myTuner.isEnabled()) {
            myTuner.analyzePitchTask(); 
            
            if (millis() - lastTunerUpdate > 100) {
                float freq = myTuner.getFrequency();
                if (freq > 75.0) {
                    webSocket.broadcastTXT("T" + String(freq, 1));
                } else if (freq == -1.0f) {
                    webSocket.broadcastTXT("T0.0"); // Silence
                }
                lastTunerUpdate = millis();
            }
        }
        vTaskDelay(10 / portTICK_PERIOD_MS); 
    }
}

// ======================================================================
// 4. SETUP HARDWARE
// ======================================================================
void setup() {
  Serial.begin(115200);
  delay(2000); // Wait for Serial Monitor to open

    Serial.println("\n--- ESP32-S3 MEMORY DIAGNOSTIC ---");

    // 1. Check Internal SRAM (used for normal variables and buffers)
    size_t internal_total = ESP.getHeapSize();
    size_t internal_free = ESP.getFreeHeap();
    
    Serial.printf("Internal SRAM Total: %d bytes (~%.2f KB)\n", 
                  internal_total, (float)internal_total / 1024.0f);
    Serial.printf("Internal SRAM Free:  %d bytes (~%.2f KB)\n", 
                  internal_free, (float)internal_free / 1024.0f);

    // 2. Check External PSRAM (used for massive arrays like the Looper)
    // Note: These functions return 0 if PSRAM is disabled or missing
    size_t psram_total = ESP.getPsramSize();
    size_t psram_free = ESP.getFreePsram();

    Serial.printf("External PSRAM Total: %d bytes (~%.2f MB)\n", 
                  psram_total, (float)psram_total / (1024.0f * 1024.0f));
    Serial.printf("External PSRAM Free:  %d bytes (~%.2f MB)\n", 
                  psram_free, (float)psram_free / (1024.0f * 1024.0f));
    Serial.println("-----------------------------------\n");


  xTaskCreatePinnedToCore(TaskWiFi, "WiFi_Task", 10000, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(TaskTunerMath, "Tuner_Task", 8192, NULL, 1, NULL, 0);
  
  myLooper.init();

  WiFi.softAP(ssid, password);
  WiFi.setTxPower(WIFI_POWER_2dBm); 
  WiFi.setSleep(false); 
  
  server.on("/", []() { server.send(200, "text/html", index_html); });
  server.begin(); 
  webSocket.begin(); 
  webSocket.onEvent(webSocketEvent);

  // Low Latency I2S Setup
  auto config_in = i2s_in.defaultConfig(RX_MODE); 
  config_in.copyFrom(info); 
  config_in.port_no = 1; 
  config_in.use_apll = false; 
  config_in.buffer_count = 4;  
  config_in.buffer_size = 128; 
  config_in.pin_bck = 4; 
  config_in.pin_data = 5; 
  config_in.pin_ws = 6; 
  config_in.pin_mck = 7;      
  i2s_in.begin(config_in);

  auto config_out = i2s_out.defaultConfig(TX_MODE); 
  config_out.copyFrom(info); 
  config_out.port_no = 0; 
  config_out.use_apll = false;    
  config_out.buffer_count = 4;  
  config_out.buffer_size = 128; 
  config_out.pin_bck = 15; 
  config_out.pin_data = 16; 
  config_out.pin_ws = 17;     
  i2s_out.begin(config_out);
}

// ======================================================================
// 5. IL CUORE AUDIO (DSP Loop) - CORE 1
// ======================================================================
void loop() {
  size_t bytes_read = i2s_in.readBytes((uint8_t*)buffer, sizeof(buffer));
  int num_samples = bytes_read / 4; 
  
  for(int i = 0; i < num_samples; i++) {
    float x0 = (float)buffer[i]; 
    float y0 = 0;
    bool is_left = (i % 2 == 0);

    // --- 1. MASTER DC BLOCKER ---
    if (is_left) {
        y0 = x0 - dc_x1_L + (DC_R * dc_y1_L);
        dc_x1_L = x0;  
        dc_y1_L = y0;
    } else {
        y0 = x0 - dc_x1_R + (DC_R * dc_y1_R);
        dc_x1_R = x0;  
        dc_y1_R = y0;
    }

    // --- 2. TUNER (Reads unattenuated signal) ---
    int64_t sample = (int64_t)y0; 
    sample = myTuner.process(sample, is_left); 

    // --- 3. MASTER INPUT GAIN ---
    sample = (int64_t)(sample * current_gain_multiplier); 

    // --- 4. EFFECTS CHAIN ---        
    sample = myEQ.process(sample, is_left);           
    sample = myOverdrive.process(sample, is_left);    
    sample = myFuzz.process(sample, is_left);         
    sample = myTremolo.process(sample, is_left);      
    sample = myDelay.process(sample, is_left);        
    sample = myReverb.process(sample, is_left);       
    sample = myLooper.process(sample, is_left);       

    // --- 5. HARD LIMITER ---
    if(sample > 2147483647LL) sample = 2147483647LL;   
    if(sample < -2147483648LL) sample = -2147483648LL; 
    
    buffer[i] = (int32_t)sample;
  }
  
  i2s_out.write((uint8_t*)buffer, bytes_read);
}