#include "LooperModule.h"

// 10 seconds of audio at 44.1kHz (441,000 samples)
// This requires ~1.76 Megabytes of RAM
const int MAX_LOOPER_SAMPLES = 1984500;

void LooperModule::init() {
    // ps_malloc explicitly forces the ESP32 to put this massive array 
    // into the external PSRAM chip instead of the tiny internal CPU memory.
    looper_buffer = (int32_t*)ps_malloc(MAX_LOOPER_SAMPLES * sizeof(int32_t));
    
    if (looper_buffer == nullptr) {
        Serial.println("[LOOPER SYSTEM] -> ERROR: PSRAM Allocation FAILED!");
    } else {
        Serial.println("[LOOPER SYSTEM] -> PSRAM Allocated Successfully!");
        memset(looper_buffer, 0, MAX_LOOPER_SAMPLES * sizeof(int32_t));
    }
}

void LooperModule::setLooperState(int state) {
    if (looper_buffer == nullptr) return; // Fail-safe if PSRAM is missing

    if (state == 1) { // 🔴 REC
        looper_state = 1;
        rec_index = 0;
        // Wipe the old loop memory clean
        memset(looper_buffer, 0, MAX_LOOPER_SAMPLES * sizeof(int32_t));
    } 
    else if (state == 2) { // ▶️ PLAY
        // If we just finished recording, lock in the exact length of the loop!
        if (looper_state == 1) {
            looper_max_index = rec_index; 
        }
        looper_state = 2;
        play_index = 0.0f; // Reset the floating "tape head" to the start
    } 
    else if (state == 0) { // ⏹️ STOP
        looper_state = 0;
        rec_index = 0;
        play_index = 0.0f;
    }
}

int64_t LooperModule::process(int64_t inputSample, bool isLeftChannel) {
    // If memory failed to load, act as a true bypass
    if (looper_buffer == nullptr) return inputSample;
    
    int64_t outputSample = inputSample;

    if (isLeftChannel) {
        if (looper_state == 1) { // 🔴 REC
            looper_buffer[rec_index] = (int32_t)inputSample;
            rec_index++;
            
            // If we hit the 10-second limit, automatically switch to playback
            if (rec_index >= MAX_LOOPER_SAMPLES) { 
                looper_max_index = MAX_LOOPER_SAMPLES;
                looper_state = 2; 
                play_index = 0.0f;
            }
        }
        else if (looper_state == 2) { // ▶️ PLAY
            // 1. Calculate Fractional Position (For the Speed Knob)
            int idx = (int)play_index;
            int next_idx = idx + 1;
            // Wrap around seamlessly for the math
            if (next_idx >= looper_max_index) next_idx = 0; 
            
            // 2. LINEAR INTERPOLATION (Smooth Pitch Shifting)
            float frac = play_index - (float)idx;
            float s1 = (float)looper_buffer[idx];
            float s2 = (float)looper_buffer[next_idx];
            float recorded_sample = s1 + frac * (s2 - s1);
            
            // 3. Micro-Crossfade (Anti-Clicking at the loop seam)
            int fade_samples = 441; // 10ms fade window
            float fade_multiplier = 1.0f;
            if (looper_max_index > fade_samples * 2) {
                if (idx < fade_samples) {
                    fade_multiplier = (float)idx / fade_samples; // Fade In
                } 
                else if (idx > (looper_max_index - fade_samples)) {
                    fade_multiplier = (float)(looper_max_index - idx) / fade_samples; // Fade Out
                }
            }
            
            // 4. Apply Volume and Mix
            recorded_sample = recorded_sample * fade_multiplier * playback_volume;
            outputSample = inputSample + (int64_t)recorded_sample;
            
            // 5. Advance the "Tape Head" by the Speed Knob value
            play_index += playback_speed;
            
            // 6. Loop back around to the beginning
            if (play_index >= (float)looper_max_index) {
                play_index -= (float)looper_max_index;
            }
        }
        
        // Save this exact math so the Right Channel can copy it
        last_looper_sample = outputSample; 
    } else {
        // Right channel blindly copies the Left channel to save CPU power and perfectly sync stereo timing
        if (looper_state == 2) {
            outputSample = last_looper_sample;
        }
    }

    return outputSample;
}