#pragma once

#include "board.h"

#include <map>

class PlatformSDR {
public:
    PlatformSDR();
    ~PlatformSDR();

    auto getBoard();

private:
    std::size_t availableDevise;
    std::map<std::size_t, std::unique_ptr<Board>> collectionBoard;
};
