
#include "platform_SDR/platform.h"
#include "platform_SDR/receiver.h"
#include "platform_SDR/transferparams.h"

#include <gtest/gtest.h>
#include <vector>

class PlatformTest : public ::testing::Test {
};

TEST_F(PlatformTest, startHwSingle) {
    PlatformSDR platformSdr;
    auto board = platformSdr.getBoard();

    ReceiverSettings sett(100e6, 1e6);
    auto receiver = board->getReceiver();
    receiver->setSettingsReceiver(sett);

    TransferParams params(1024 * 32, TypeTransaction::single, 2, true);
    auto transferControl = board->getTransferControl(params);
    std::vector<uint8_t> buf(0);
    buf.reserve(1024 * 1024 * 512);

    transferControl->setCallBack([&buf, &transferControl](std::complex<int8_t>* ptr, uint32_t size) {
        auto uintPtr = (uint8_t*)(ptr);
        buf.insert(buf.end(), uintPtr, uintPtr + size);
        transferControl->stop();
    });
    transferControl->start();

    for(std::size_t i = 1; i != buf.size(); i++) {
        ASSERT_EQ(uint8_t(buf[i - 1] + 1), uint8_t(buf[i]));
    }
}

TEST_F(PlatformTest, startTestModeHwLoop_freq) {
    size_t bufferSize = 1024 * 2;
    bool flag         = true;
    PlatformSDR platformSdr;
    auto board = platformSdr.getBoard();
    ReceiverSettings set(200e6, 2e6);
    auto receiver        = board->getReceiver(set);
    auto transferControl = board->getTransferControl();

    std::vector<uint8_t> buf(0);
    buf.reserve(1024 * 1024 * 512);

    for(std::size_t irqSize = 0; irqSize < 16; irqSize += 1) {
        for(std::size_t sampRate = 2.3e6; flag == true; sampRate += 100e3) {
            {
                receiver->setSampleRate(sampRate);
                TransferParams setTransaction(bufferSize, TypeTransaction::loop, irqSize, true);
                transferControl->setTransferParams(setTransaction);

                buf.clear();
                transferControl->setCallBack([&buf](std::complex<int8_t>* ptr, uint32_t size) {
                    auto uintPtr = (uint8_t*)(ptr);
                    buf.insert(buf.end(), uintPtr, uintPtr + size);
                });

                transferControl->start();
                std::this_thread::sleep_for(std::chrono::milliseconds(900));
                transferControl->stop();

                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                for(std::size_t i = 1; i != buf.size(); i++) {
                    if(uint8_t(buf[i - 1] + 1) != uint8_t(buf[i])) {
                        flag = false;
                    }
                }
                if(!flag) {
                    std::cerr << "FAIL " << std::endl;
                    std::cerr << "sampRate = " << sampRate / 1e6 << std::endl;
                    std::cerr << "irqsize = " << irqSize << std::endl << std::endl;
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
        flag = true;
    }
}
TEST_F(PlatformTest, startTestModeHwLoop_2_Dev) {
    size_t ircSize       = 4;
    size_t bufferSize    = 1024 * 32;
    uint32_t centralFreq = 100e6;
    uint32_t sampleFreq  = 2.4e6;

    PlatformSDR platformSdr;
    auto board1 = platformSdr.getBoard();
    auto board2 = platformSdr.getBoard();

    ReceiverSettings sett{centralFreq, sampleFreq};
    TransferParams setTransaction(bufferSize, TypeTransaction::loop, ircSize, true);

    board1->getReceiver()->setSettingsReceiver(sett);
    board2->getReceiver()->setSettingsReceiver(sett);

    auto transferControl1 = board1->getTransferControl(setTransaction);
    auto transferControl2 = board2->getTransferControl(setTransaction);

    std::vector<uint8_t> buf1(0);
    buf1.reserve(1024 * 1024 * 512);
    transferControl1->setCallBack([&buf1](std::complex<int8_t>* ptr, uint32_t size) {
        auto uintPtr = (uint8_t*)(ptr);
        buf1.insert(buf1.end(), uintPtr, uintPtr + size);
    });

    std::vector<uint8_t> buf2(0);
    buf2.reserve(1024 * 1024 * 512);
    transferControl2->setCallBack([&buf2](std::complex<int8_t>* ptr, uint32_t size) {
        auto uintPtr = (uint8_t*)(ptr);
        buf2.insert(buf2.end(), uintPtr, uintPtr + size);
    });

    transferControl1->start();
    transferControl2->start();

    std::this_thread::sleep_for(std::chrono::seconds(5));
    transferControl1->stop();
    transferControl2->stop();

    for(std::size_t i = 1; i != buf1.size(); i++) {
        ASSERT_EQ(uint8_t(buf1[i - 1] + 1), uint8_t(buf1[i]));
    }
    for(std::size_t i = 1; i != buf2.size(); i++) {
        ASSERT_EQ(uint8_t(buf2[i - 1] + 1), uint8_t(buf2[i]));
    }
}