#pragma once
#include "rtl-sdr.h"
#include "transferparams.h"

#include <complex>
#include <functional>
#include <memory>
#include <thread>

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

    std::function<void(std::complex<int8_t>*, uint32_t)> process;
    TransferParams params;
    std::unique_ptr<std::thread> thread;
    bool needProcessing = true;
};
#include "memory"