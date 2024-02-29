#pragma once

#include "memory"
#include "rtl-sdr.h"
#include "thread"

class TransferControl {
public:
    TransferControl();
    ~TransferControl();

    void start();
    void stop();

private:
    struct Pimpl;
    std::unique_ptr<Pimpl> m_d;
};
