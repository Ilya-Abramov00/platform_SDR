#include "platform_SDR/board.h"

struct Board::Pimpl {
    void open();
    int dev_index{0};
    int dev_given{0};
    uint32_t samCount;
    rtlsdr_dev_t* dev{nullptr};
    int do_exit{0};
    rtlsdr_read_async_cb_t callback;

    int setCenterFreq(uint32_t freq);
    int setSampleRate(uint32_t samp_rate);
    int setAutoGain();
    int nearestGain(int gain);
    int setGain(int gain);
    int setDirectSampling(int on);
    int setPpm(int ppm_error);
    void setAgcMode(int on);
    void setOffsetTuningOn();
    int resetBuffer();
    int setTunerBandwidth(uint32_t bw);
    static uint32_t roundPowerTwo(uint32_t& size);
};

Board::Board(uint32_t numberDev) {
    m_d->dev_index = numberDev;
    m_d->open();
}