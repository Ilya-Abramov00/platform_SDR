#include "platform_SDR/transfercontrol.h"

#include <iostream>
TransferControl::TransferControl(rtlsdr_dev_t* dev, TransferParams params) : dev(dev), params(params) { }
TransferControl::TransferControl(rtlsdr_dev_t* dev) : dev(dev) { }
void TransferControl::start() {
    needProcessing = true;

    if(rtlsdr_set_testmode(dev, params.testMode)) {
        std::cerr << "WARNING: Failed to set testMode." << std::endl;
    }

    if(params.typeTransaction == TypeTransaction::single) {
        startSingle();
    } else if(params.typeTransaction == TypeTransaction::loop) {
        startLoop();
    }
}
void TransferControl::stop() {
    if(params.typeTransaction == TypeTransaction::loop && needProcessing) {
        auto r = rtlsdr_cancel_async(dev);
        if(r < 0) {
            std::cerr << "FAIL Stop" << r << std::endl;
        }
        thread->join();
    }
    needProcessing = false;
}

void TransferControl::startLoop() {
    rtlsdr_read_async_cb_t callback = [](uint8_t* buf, uint32_t size, void* ctx) {
        auto* d = reinterpret_cast<TransferControl*>(ctx); // контекст которые мы передали, кастим к объекту класса

        d->process((std::complex<signed char>*)buf, size);
    };

    thread = std::make_unique<std::thread>([this, &callback]() {
        resetBuffer(); // должен быть обязательно!!

        auto r = rtlsdr_read_async(dev, callback, this, params.ircSize,
                                   2 * params.bufferSize); // this в данном случае является контекстом

        if(r < 0)
            std::cerr << "FAIL read_async: " << r << std::endl;
    });
}

void TransferControl::startSingle() {
    std::vector<std::complex<int8_t> > complexBuff(params.bufferSize);
    while(needProcessing) {
        getComplex(complexBuff.data(), 2 * params.bufferSize);
        process(complexBuff.data(), 2 * params.bufferSize);
    }
}
bool TransferControl::getComplex(std::complex<int8_t>* complexBuff, std::size_t sizeOfBuff) {
    resetBuffer();

    int n_read;
    auto result = rtlsdr_read_sync(dev, reinterpret_cast<char*>(complexBuff), sizeOfBuff, &n_read);

    if(n_read != sizeOfBuff) {
        std::cerr << "Warning: получено " << n_read << "данных" << std::endl;
    }
    if(result < 0)
        std::cerr << "WARNING: sync read failed." << std::endl;

    return result;
}
void TransferControl::setCallBack(std::function<void(std::complex<int8_t>*, uint32_t)> function) {
    process = std::move(function);
}
void TransferControl::setTransferParams(TransferParams sett) {
    params = sett;
}
void TransferControl::resetBuffer() {
    auto r = rtlsdr_reset_buffer(dev);
    if(r < 0)
        std::cerr << "WARNING: Failed to reset buffers " << r << std::endl;
}
