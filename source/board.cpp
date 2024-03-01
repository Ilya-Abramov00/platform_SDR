#include "platform_SDR/board.h"

#include "rtl-sdr.h"

Board::Board(std::size_t numberDev) {
    dev_index = numberDev;
    open();
}
std::shared_ptr<TransferControl> Board::getTransferControl(TransferParams params) {
    return std::make_shared<TransferControl>(dev, params);
}
std::shared_ptr<TransferControl> Board::getTransferControl() {
    return std::make_shared<TransferControl>(dev);
}
std::shared_ptr<Receiver> Board::getReceiver() {
    return std::make_shared<Receiver>(dev);
}
std::shared_ptr<Receiver> Board::getReceiver(ReceiverSettings set) {
    return std::make_shared<Receiver>(dev, set);
}

void Board::open() {
    int r = rtlsdr_open(&dev, (uint32_t)dev_index);
    if(r < 0) {
        std::cerr << "Failed to open rtlsdr device" << dev_index << std::endl;
        exit(1);
    } else {
        std::cerr << "Open device " << dev_index << std::endl;
    }
}
void Board::close() {
    auto r = rtlsdr_reset_buffer(dev);
    if(r < 0)
        std::cerr << "WARNING: Failed to reset buffers" << std::endl;

    r = rtlsdr_close(dev);
    if(r < 0) {
        std::cerr << "FAIL close: " << r << std::endl;
    } else {
        std::cerr << "Close device " << dev_index << std::endl;
    }
}
Board::~Board() {
    close();
}
