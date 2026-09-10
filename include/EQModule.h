#pragma once
#include "BaseEffect.h"

class EQModule : public BaseEffect {
  private:
    int param_bass = 50;
    int param_mid = 50;
    int param_treble = 50;

    // Filter memory for Left Channel
    float low_L = 0;
    float high_L = 0;

    // Filter memory for Right Channel
    float low_R = 0;
    float high_R = 0;

  public:
    void setParams(int bass, int mid, int treble);
    int64_t process(int64_t inputSample, bool isLeftChannel) override;
};