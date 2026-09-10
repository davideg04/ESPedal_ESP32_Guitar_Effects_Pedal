#pragma once
#include "BaseEffect.h"
#include <Arduino.h>

class TunerModule : public BaseEffect {
  private:
    static const int BUFFER_SIZE = 2048;
    float audio_buffer[BUFFER_SIZE];      // Core 1 writes here
    float processing_buffer[BUFFER_SIZE]; // Core 0 reads here
    int write_index = 0;
    
    // The safely shared flag between cores
    volatile bool buffer_ready = false;   
    float current_frequency = 0.0f;

  public:
    int64_t process(int64_t inputSample, bool isLeftChannel) override;
    
    // Core 0 Background Task
    void analyzePitchTask(); 
    float getFrequency();
};