#include "platform_SDR/transfercontrol.h"

TransferControl::TransferControl(rtlsdr_dev_t* dev, const TransferParams& params) : dev(dev), params(params) { }
void TransferControl::start() { }
void TransferControl::stop() { }

/*struct TransferControl::Pimpl {
    int resetBuffer();
};*/
