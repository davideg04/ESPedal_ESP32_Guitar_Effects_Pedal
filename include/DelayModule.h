#pragma once
#include "BaseEffect.h"
#include <stdint.h>

#define DELAY_SIZE 22050 

class DelayModule : public BaseEffect {
  private:
    int param_time = 50;
    int param_feedback = 50;
    
    // Il "Nastro" magnetico isolato nella memoria della classe
    int32_t delay_buffer[DELAY_SIZE] = {0}; 
    int delay_write_index = 0;

  public:
    void setParams(int time, int feedback);
    int64_t process(int64_t inputSample, bool isLeftChannel) override;
};