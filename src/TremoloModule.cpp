#include "TremoloModule.h"
#include <math.h>
#include <Arduino.h>

void TremoloModule::setParams(int speed, int depth) {
    param_speed = speed;
    param_depth = depth;
}

int64_t TremoloModule::process(int64_t inputSample, bool isLeftChannel) {
    if (!enabled) return inputSample;

    float speed_hz = 1.0f + (param_speed / 5.0f); 
    float step = (2.0f * PI * speed_hz) / 44100.0f;
    float depth = param_depth / 100.0f; 
    float lfo_val = (sin(fase_lfo) + 1.0f) / 2.0f; 
    
    int64_t output = inputSample * (1.0f - depth + (lfo_val * depth));
    
    // L'oscillatore avanza solo 1 volta per frame stereo (sul canale sinistro)
    if(isLeftChannel) { 
        fase_lfo += step; 
        if (fase_lfo > 2.0f * PI) fase_lfo -= 2.0f * PI; 
    }
    return output;
}