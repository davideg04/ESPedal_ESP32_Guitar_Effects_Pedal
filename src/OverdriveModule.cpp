#include "OverdriveModule.h"
#include <math.h>

void OverdriveModule::setParams(int drive, int level) {
    param_drive = drive;
    param_level = level;
}

int64_t OverdriveModule::process(int64_t inputSample, bool isLeftChannel) {
    // Hardware True Bypass
    if (!enabled) return inputSample; 

    // --- 1. THE DRIVE STAGE ---
    // A linear taper from 1.0x (Clean) to 30.0x (Heavy Overdrive)
    float drive_slider = (float)param_drive / 100.0f;
    float current_drive = 1.0f + (drive_slider * 29.0f); 
    
    float s = (float)inputSample * current_drive;
    
    // --- 2. THE TUBE SOFT-CLIPPER ---
    // Your excellent math! Gently rounds the wave at 100 Million
    const float LIMIT = 100000000.0f; 
    s = s / (1.0f + (fabs(s) / LIMIT)); 
    
    // --- 3. AUTO-GAIN & MASTER LEVEL ---
    // Shrink the wave mathematically as the drive goes up so it doesn't explode
    float auto_gain_compensation = 1.0f / sqrtf(current_drive);
    
    // 50 on the UI = Normal Volume (1.0x). 100 = Double Volume (2.0x).
    float master_level = (float)param_level / 50.0f; 
    
    // We add a tiny 1.2x boost at the end because soft-clipping naturally 
    // eats a little bit of the volume even at minimum settings.
    float final_output = s * auto_gain_compensation * master_level * 1.2f;

    return (int64_t)final_output; 
}