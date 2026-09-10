#pragma once
#include <Arduino.h>

class BaseEffect {
  protected:
    bool enabled = false;

  public:
    virtual ~BaseEffect() {}

    // Every effect can be turned on or off
    void setEnabled(bool state) {
        enabled = state;
    }

    bool isEnabled() {
        return enabled;
    }

    // The core DSP function. "= 0" means every specific effect MUST write its own version of this math.
    virtual int64_t process(int64_t inputSample, bool isLeftChannel) = 0;
};