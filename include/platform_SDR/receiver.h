#pragma once

#include "memory"
#include "rtl-sdr.h"
#include "thread"
struct ReceiverSettings {
    int direct_sampling{0};
    int gain{0};
    int ppm_error{0};
    int agcMode{1};
    uint32_t sampleCount;
    uint32_t centralFreq; // fc
    uint32_t sampleFreq;  // fd

    ReceiverSettings(uint32_t centralFreq, uint32_t sampleFreq) : centralFreq(centralFreq), sampleFreq(sampleFreq) { }
    ~ReceiverSettings() = default;
};

class Receiver {
public:
    Receiver();
    ~Receiver();

    void setSettingsReceiver(ReceiverSettings* sett);

private:
    struct Pimpl;
    std::unique_ptr<Pimpl> m_d;
};
