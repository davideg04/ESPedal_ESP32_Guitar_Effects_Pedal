#pragma once
#include "BaseEffect.h"

class TremoloModule : public BaseEffect {
  private:
    int param_speed = 50;
    int param_depth = 50;
    float fase_lfo = 0.0f;

  public:
    void setParams(int speed, int depth);
    int64_t process(int64_t inputSample, bool isLeftChannel) override;
};