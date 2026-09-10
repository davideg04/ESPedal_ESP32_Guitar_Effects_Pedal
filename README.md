# ESP32-S3 Digital Guitar Effects Pedal


**ESPedal** is a custom, digital multi-effect pedalboard and audio processing unit built around the ESP32-S3 microcontroller. It performs real-time digital signal processing (DSP) for guitar or other audio sources, featuring a full chain of customizable effects, a looper, and a chromatic tuner. 

The device operates headlessly, broadcasting its own Wi-Fi Access Point. Users can control all effects in real-time via an interactive Web UI hosted directly on the ESP32-S3, utilizing WebSockets for ultra-low latency communication.

## Features

- **Real-Time DSP Audio Processing:** Dedicated audio loop running on Core 1 for low-latency I2S audio processing.
- **Extensive Effects Chain:** Includes EQ, Overdrive, Fuzz, Tremolo, Delay, and Reverb modules.
- **Integrated Looper:** Record, play, overdub, and adjust speed/volume using the ESP32-S3's external PSRAM for extended recording time.
- **Built-in Chromatic Tuner:** Real-time pitch detection algorithm running on Core 0.
- **Wireless Web UI:** Control pedal parameters wirelessly via smartphone or PC without downloading a dedicated app.
- **Custom Hardware Integration:** Includes Altium source files for a custom I2S audio PCB/shield.

<img width="1177" height="842" alt="ESPedal_WEBUI" src="https://github.com/user-attachments/assets/1623ead8-6def-4c26-9d07-14ff414041f9" />
