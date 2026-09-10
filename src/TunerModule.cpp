#include "TunerModule.h"
#include <cmath>

int64_t TunerModule::process(int64_t inputSample, bool isLeftChannel) {
    if (!enabled) return inputSample; 

    if (isLeftChannel) {
        if (write_index < BUFFER_SIZE) {
            audio_buffer[write_index++] = (float)inputSample;
        } else if (!buffer_ready) {
            memcpy(processing_buffer, audio_buffer, sizeof(audio_buffer));
            write_index = 0;
            buffer_ready = true; 
        } else {
            write_index = 0; 
        }
    }
    
    return inputSample; 
}

void TunerModule::analyzePitchTask() {
    if (!buffer_ready) return; 

    // 1. DC Blocking
    float dc_offset = 0;
    for (int i = 0; i < BUFFER_SIZE; i++) dc_offset += processing_buffer[i];
    dc_offset /= BUFFER_SIZE;

    // 2. Center Wave & Find True Peak
    float peak = 0;
    for (int i = 0; i < BUFFER_SIZE; i++) {
        processing_buffer[i] -= dc_offset; 
        
        float abs_val = fabs(processing_buffer[i]); 
        if (abs_val > peak) peak = abs_val;
    }

    // 3. The Calibrated Noise Gate (8 Million)
    if (peak > 8000000.0f) { 
        
        // 4. Normalize the buffer
        for (int i = 0; i < BUFFER_SIZE; i++) {
            processing_buffer[i] /= peak;
        }

        float max_correlation = 0;
        int max_lag = 0;
        
        // 5. Autocorrelation (High E to Low B)
        for (int lag = 44; lag < 600; lag++) {
            float sum = 0;
            int overlap = BUFFER_SIZE - lag; 
            
            for (int i = 0; i < overlap; i++) {
                sum += processing_buffer[i] * processing_buffer[i + lag];
            }
            
            float average_correlation = sum / overlap;

            // 6. The Octave Drop Fix (Lag Penalty)
            float biased_correlation = average_correlation * (1.0f - (lag * 0.0002f));

            if (biased_correlation > max_correlation) {
                max_correlation = biased_correlation;
                max_lag = lag;
            }
        }
        
        // 7. Parabolic Interpolation 
        if (max_lag > 44 && max_lag < 599) {
            float y1 = 0, y2 = 0, y3 = 0;
            
            for (int i = 0; i < BUFFER_SIZE - (max_lag - 1); i++) y1 += processing_buffer[i] * processing_buffer[i + max_lag - 1];
            for (int i = 0; i < BUFFER_SIZE - max_lag; i++)       y2 += processing_buffer[i] * processing_buffer[i + max_lag];
            for (int i = 0; i < BUFFER_SIZE - (max_lag + 1); i++) y3 += processing_buffer[i] * processing_buffer[i + max_lag + 1];

            y1 /= (BUFFER_SIZE - (max_lag - 1));
            y2 /= (BUFFER_SIZE - max_lag);
            y3 /= (BUFFER_SIZE - (max_lag + 1));

            float shift = 0.5f * (y1 - y3) / (y1 - 2.0f * y2 + y3);
            current_frequency = 44100.0f / ((float)max_lag + shift);
        } else if (max_lag > 0) {
            current_frequency = 44100.0f / (float)max_lag;
        }
    } else {
        current_frequency = -1.0f; 
    }

    buffer_ready = false; 
}

float TunerModule::getFrequency() { 
    return current_frequency; 
}