#pragma once
#include "transferparams.h"

#include <complex>
#include <functional>
#include <memory>
#include <thread>
#include "rtl-sdr/rtl_sdr.h"
class TransferControl {
public:
    TransferControl(rtlsdr_dev_t* dev, TransferParams params);
    explicit TransferControl(rtlsdr_dev_t* dev);

    ~TransferControl() = default;

    void start();
    void stop();

    void setTransferParams(TransferParams sett);
    void setCallBack(std::function<void(std::complex<int8_t>*, uint32_t)> f);

private:
    void resetBuffer();

    void startLoop();
    void startSingle();
    bool getComplex(std::complex<int8_t>* complexBuff, std::size_t sizeOfBuff);

    rtlsdr_dev_t* dev{nullptr};
    rtlsdr_read_async_cb_t callback;
    std::function<void(std::complex<int8_t>*, uint32_t)> process;
    TransferParams params;
    std::unique_ptr<std::thread> thread;
    bool needProcessing = true;
};
#include "memory"