
#include "platform_SDR/platform.h"
#include "platform_SDR/receiver.h"
#include "platform_SDR/transferparams.h"

#include <gtest/gtest.h>
#include <vector>

class PlatformTest : public ::testing::Test {
protected:
    void SetUp() { }
    void TearDown() { }
};

TEST_F(PlatformTest, startHwSingle) {
    PlatformSDR platformSdr;
    auto board = platformSdr.getBoard();
    ASSERT_TRUE(board.get() != nullptr);
    TransferParams params(0, TypeTransaction::single, 0, 0);
    ASSERT_TRUE(board->getTransferControl(params) != nullptr);
    auto receiver = board->getReceiver();

    ReceiverSettings sett(100e6, 1e6);
    receiver->setSettingsReceiver(sett);
}
