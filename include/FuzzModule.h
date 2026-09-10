#ifndef FUZZMODULE_H
#define FUZZMODULE_H

#include <Arduino.h>

class FuzzModule {
  private:
    bool enabled = false;
    float current_drive = 1.0f;
    float output_volume = 1.0f;

  public:
    void setEnabled(bool state) { enabled = state; }
    bool isEnabled() { return enabled; }
    
    void setParams(int drive, int level);
    int64_t process(int64_t inputSample, bool isLeftChannel);
};

#endif