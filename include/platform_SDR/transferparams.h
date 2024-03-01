#pragma once
#include <cstddef>
enum class TypeTransaction { single, loop };

class TransferParams {
public:
    TransferParams(size_t bufferSize, TypeTransaction typeTransaction, size_t ircSize, bool testMode) :
        bufferSize(bufferSize), typeTransaction(typeTransaction), ircSize(ircSize), testMode(testMode) { }
    ~TransferParams()=default;

private:
    std::size_t bufferSize{0};
    TypeTransaction typeTransaction{0};
    std::size_t ircSize{0};
    bool testMode = false;
};
