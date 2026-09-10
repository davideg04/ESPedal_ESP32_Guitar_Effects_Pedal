#pragma once
#include "BaseEffect.h"
#include <stdint.h>

#define R1 1557 
#define R2 1613 
#define R3 1753 

class ReverbModule : public BaseEffect {
  private:
    int param_size = 50;
    int param_mix = 50;
    
    int32_t rb1[R1]={0};
    int32_t rb2[R2]={0};
    int32_t rb3[R3]={0};
    int r1x=0, r2x=0, r3x=0;

  public:
    void setParams(int size, int mix);
    int64_t process(int64_t inputSample, bool isLeftChannel) override;
};