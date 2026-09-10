#include "DelayModule.h"

void DelayModule::setParams(int time, int feedback) {
    param_time = time;
    param_feedback = feedback;
}

int64_t DelayModule::process(int64_t inputSample, bool isLeftChannel) {
    int max_delay_len = 500 + (param_time * 210); 
    float feedback = param_feedback / 120.0f; 
    
    if(isLeftChannel) { 
        delay_write_index++; 
        if(delay_write_index >= DELAY_SIZE) delay_write_index = 0; 
    }
    
    int read_idx = delay_write_index - max_delay_len;
    if (read_idx < 0) read_idx += DELAY_SIZE;
    
    int32_t suono_vecchio = delay_buffer[read_idx];
    
    // Se è spento, smette di registrare input (ma il vecchio suono continua a girare)
    int64_t input_al_nastro = enabled ? inputSample : 0;
    
    if(isLeftChannel) {
        delay_buffer[delay_write_index] = input_al_nastro + (suono_vecchio * feedback);
    }
    
    // Riproduce il delay sia se è ON, sia per far morire le code se appena spento
    if(enabled || suono_vecchio > 100 || suono_vecchio < -100) {
        return inputSample + suono_vecchio;
    }
    return inputSample;
}