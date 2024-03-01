#pragma once
#include "rtl-sdr.h"
#include "transferparams.h"

#include <memory>

class TransferControl {
public:
    TransferControl(rtlsdr_dev_t* dev, const TransferParams& params);
    ~TransferControl() = default;

    void start();
    void stop();

private:
    rtlsdr_dev_t* dev{nullptr};
    rtlsdr_read_async_cb_t callback;
    std::size_t samCount;
    const TransferParams& params;
};
#include "memory"