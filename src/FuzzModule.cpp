#include "FuzzModule.h"

void FuzzModule::setParams(int drive, int level) {
    // 1. Drive Knob (1.0x to 51.0x)
    current_drive = 1.0f + ((float)drive * 0.5f); 
    
    // 2. Level Knob (Master Volume)
    // 50 on the UI = 1.0x. 100 on the UI = 2.0x Volume Boost.
    float master_level = (float)level / 50.0f;
    
    // 3. Gentle Square Wave Compensation
    // A square wave holds more acoustic energy than a clean wave.
    // We gently roll off up to 30% of the master volume at max drive 
    // so turning the Fuzz up doesn't blow out your speakers.
    float square_comp = 1.0f - ((float)drive / 100.0f * 0.3f); 
    
    output_volume = master_level * square_comp; 
}

int64_t FuzzModule::process(int64_t inputSample, bool isLeftChannel) {
    if (!enabled) return inputSample; // Hardware True Bypass

    float x = (float)inputSample;
    
    // Step 1: The Gain Stage
    float driven = x * current_drive;

    // Step 2: The Physical Clipping Diodes (The Brick Wall)
    const float FUZZ_CEILING = 60000000.0f; 

    if (driven > FUZZ_CEILING) {
        driven = FUZZ_CEILING;
    } else if (driven < -FUZZ_CEILING) {
        driven = -FUZZ_CEILING;
    }

    // Step 3: The Output Stage
    float final_output = driven * output_volume;

    return (int64_t)final_output;
}