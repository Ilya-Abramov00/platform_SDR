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

    std::shared_ptr<TransferControl> getTransferControl(TransferParams params);
    std::shared_ptr<TransferControl> getTransferControl();

    std::shared_ptr<Receiver> getReceiver(ReceiverSettings settings);
    std::shared_ptr<Receiver> getReceiver();

    ~Board();

private:
    std::size_t dev_index{0};
    rtlsdr_dev_t* dev{nullptr};

    void open();
    void close();
};
