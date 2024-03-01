#pragma once
#include "rtl-sdr.h"

#include <memory>

struct ReceiverSettings {
    int direct_sampling{0};
    int gain{0};
    int ppm_error{0};
    int agcMode{1};
    uint32_t centralFreq; // fc
    uint32_t sampleFreq;  // fd

    ReceiverSettings(uint32_t centralFreq, uint32_t sampleFreq) : centralFreq(centralFreq), sampleFreq(sampleFreq) { }
    ~ReceiverSettings() = default;
};

class Receiver {
public:
    Receiver(rtlsdr_dev_t* dev, ReceiverSettings settings);
    explicit Receiver(rtlsdr_dev_t* dev);

    ~Receiver() = default;

    void setSettingsReceiver(const ReceiverSettings& sett);

    void setCenterFreq(uint32_t freq);
    void setSampleRate(uint32_t samp_rate);
    void setAutoGain();
    void setGain(int gain);
    void setDirectSampling(int on);
    void setPpm(int ppm_error);
    void setAgcMode(int on);
    void setTunerBandwidth(uint32_t bw);

private:
    int nearestGain(int gain);

    rtlsdr_dev_t* dev{nullptr};
};
