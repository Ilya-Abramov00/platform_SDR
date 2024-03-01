#pragma once

#include "board.h"

#include <map>

class PlatformSDR {
public:
    PlatformSDR();
    ~PlatformSDR()=default;

    std::shared_ptr<Board> getBoard();

private:
    std::size_t availableDevise;
    std::map<std::size_t, std::shared_ptr<Board>> collectionBoard;
};
