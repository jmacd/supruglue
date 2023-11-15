#include "lib/soc/sysevts.h"
#include "gtest/gtest.h"

TEST(RpmsgTest, SysEvts) {
  EXPECT_EQ(16, SYSEVT_PR1_PRU_MST_INTR0_INTR_REQ);
}
