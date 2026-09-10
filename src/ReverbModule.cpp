#include "ReverbModule.h"

void ReverbModule::setParams(int size, int mix) {
    param_size = size;
    param_mix = mix;
}

int64_t ReverbModule::process(int64_t inputSample, bool isLeftChannel) {
    if (!enabled) return inputSample;

    float decay = 0.3f + (param_size / 150.0f); 
    float mix = param_mix / 100.0f;
    
    if(isLeftChannel) {
       r1x++; if(r1x>=R1) r1x=0;
       r2x++; if(r2x>=R2) r2x=0;
       r3x++; if(r3x>=R3) r3x=0;
    }
    
    int32_t out1 = rb1[r1x]; 
    int32_t out2 = rb2[r2x]; 
    int32_t out3 = rb3[r3x];
    float rev_out = (out1 + out2 + out3) / 3.0f;
    
    if(isLeftChannel) {
       rb1[r1x] = inputSample + (out1 * decay);
       rb2[r2x] = inputSample + (out2 * decay);
       rb3[r3x] = inputSample + (out3 * decay);
    }
    
    return (int64_t)((inputSample * (1.0f - mix)) + (rev_out * mix * 2.0f));
}