#include "platform_SDR/platform.h"



class RtlSdrDev {
public:
    static uint32_t deviceSearch() {
        char vendor[256] = {0}, product[256] = {0}, serial[256] = {0};
        auto device_count = rtlsdr_get_device_count();
        if(!device_count) {
            std::cerr << "No supported devices found.\n";
            return {};
        }
        std::cerr << "Found " << device_count << " device(s):\n";
        for(uint32_t i = 0; i < device_count; i++) {
            if(rtlsdr_get_device_usb_strings(i, vendor, product, serial) == 0) {
                std::cerr << i << ": " << vendor << ", " << product << ", " << serial << "\n";
            } else {
                std::cerr << i << "Failed to query data\n";
            }
        }
        return device_count;
    }
};

PlatformSDR::PlatformSDR() {
    availableDevise = RtlSdrDev::deviceSearch();
}
auto PlatformSDR::getBoard() {
    if(!availableDevise) {
        throw std::runtime_error("ERROR: Нет доступных устройст");
    }
    auto ptr = std::make_unique<Board>(availableDevise - 1);
    collectionBoard.insert({availableDevise, std::move(ptr)});
    availableDevise--;

    return collectionBoard.find(availableDevise + 1);
}

