#ifndef LOOPERMODULE_H
#define LOOPERMODULE_H

#include <Arduino.h>

class LooperModule {
  private:
    bool enabled = false;
    int looper_state = 0; // 0=STOP, 1=REC, 2=PLAY
    int32_t* looper_buffer = nullptr;
    
    // Split the index into an Integer for Recording, and a Float for Playback
    int32_t rec_index = 0;
    int32_t looper_max_index = 0;
    float play_index = 0.0f; 
    int64_t last_looper_sample = 0;

    // --- NEW KNOB VARIABLES ---
    float playback_volume = 1.0f; 
    float playback_speed = 1.0f;  

  public:
    void init();
    void setLooperState(int state);
    int64_t process(int64_t inputSample, bool isLeftChannel);

    // --- NEW KNOB CONTROLS ---
    void setVolume(int vol) {
        // 50 = 1.0x Normal Volume. 100 = 2.0x Boost.
        playback_volume = (float)vol / 50.0f; 
    }
    
    void setSpeed(int speed) {
        // --- FINE-TUNED TAPE SPEED ---
        // First, we calculate how far the knob is from the perfect center (50).
        // If the UI sends 50, the offset is 0.0.
        float center_offset = (float)speed - 50.0f; 
        
        // Next, we multiply that offset to control the "Sensitivity".
        // Using 0.005f means a maximum range of +/- 25% speed.
        // UI at 0   = 0.75x speed (Slightly slower, deeper pitch)
        // UI at 50  = 1.00x speed (Perfectly Normal)
        // UI at 100 = 1.25x speed (Slightly faster, higher pitch)
        float scaled_offset = center_offset * 0.005f;
        
        playback_speed = 1.0f + scaled_offset; 
    }
};

#endif