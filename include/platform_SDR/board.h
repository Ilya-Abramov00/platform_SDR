#pragma once
#include "receiver.h"
#include "transfercontrol.h"
#include "transferparams.h"

#include <iostream>
#include <map>
#include <memory>

class Board {
public:
    Board(uint32_t numberDev);

    std::shared_ptr<TransferControl> getTransferControl(const TransferParams& params);
    std::shared_ptr<Receiver> getReceiver();

    ~Board() = default;

private:
    int dev_index{0};
    rtlsdr_dev_t* dev{nullptr};

    void open();
    void close();
};
