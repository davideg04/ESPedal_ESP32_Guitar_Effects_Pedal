#include "EQModule.h"

void EQModule::setParams(int bass, int mid, int treble) {
    param_bass = bass;
    param_mid = mid;
    param_treble = treble;
}

int64_t EQModule::process(int64_t inputSample, bool isLeftChannel) {
    if (!enabled) return inputSample;

    float sample = (float)inputSample;
    
    // 1. Calculate Multipliers
    // 0 = 0.0x (Total Cut), 50 = 1.0x (Flat), 100 = 2.0x (Boost)
    float b_gain = param_bass / 50.0f;
    float m_gain = param_mid / 50.0f;
    float t_gain = param_treble / 50.0f;

    // 2. Crossover Frequencies (Tuned for 44.1kHz sample rate)
    // alpha_low splits at ~250Hz (Bass), alpha_high splits at ~2500Hz (Treble)
    float alpha_low = 0.035f; 
    float alpha_high = 0.35f;

    float bass_part, treble_part, mid_part;

    // 3. Split the signal into 3 isolated bands
    if (isLeftChannel) {
        low_L += alpha_low * (sample - low_L);       // Extract Lows
        high_L += alpha_high * (sample - high_L);    // Extract Mids+Lows
        
        bass_part = low_L;
        treble_part = sample - high_L;               // Treble is what's left above high_L
        mid_part = high_L - low_L;                   // Mids are between high_L and low_L
    } else {
        low_R += alpha_low * (sample - low_R);
        high_R += alpha_high * (sample - high_R);
        
        bass_part = low_R;
        treble_part = sample - high_R;
        mid_part = high_R - low_R;
    }

    // 4. Apply the massive range gain and recombine
    float output = (bass_part * b_gain) + (mid_part * m_gain) + (treble_part * t_gain);

    return (int64_t)output;
}