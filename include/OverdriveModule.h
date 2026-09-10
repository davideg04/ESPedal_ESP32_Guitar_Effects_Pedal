#pragma once
#include "BaseEffect.h"

class OverdriveModule : public BaseEffect {
  private:
    int param_drive = 50;
    int param_level = 50;

  public:
    void setParams(int drive, int level);
    
    // Sovrascriviamo la funzione base con la matematica del Tube Overdrive
    int64_t process(int64_t inputSample, bool isLeftChannel) override;
};