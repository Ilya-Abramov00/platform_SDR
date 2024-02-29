#pragma once
#include "rtl-sdr.h"
#include "transfercontrol.h"
#include "transferparams.h"
#include "receiver.h"

#include <iostream>
#include <map>
#include <memory>

class Board {
public:
    Board(uint32_t numberDev);

    std::shared_ptr<TransferControl> getTransferControl(const TransferParams& params);
    std::shared_ptr<Receiver> getReceiver();

    ~Board();

private:
    struct Pimpl;
    std::unique_ptr<Pimpl> m_d;
};
