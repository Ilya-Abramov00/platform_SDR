#pragma once
#include "receiver.h"
#include "transfercontrol.h"
#include "transferparams.h"

#include <iostream>
#include <map>
#include <memory>

class Board {
public:
    Board(std::size_t numberDev);

    std::unique_ptr<TransferControl> getTransferControl(TransferParams params);
    std::unique_ptr<TransferControl> getTransferControl();

    std::unique_ptr<Receiver> getReceiver(ReceiverSettings settings);
    std::unique_ptr<Receiver> getReceiver();

    ~Board();

private:
    std::size_t dev_index{0};
    rtlsdr_dev_t* dev{nullptr};

    void open();
    void close();
};
