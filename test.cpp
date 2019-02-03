#include <string>
#include "kalloc.h"
#include <gtest/gtest.h>

kChunk *ch;

TEST(hwTest, TESTkcCreate) {
  ch = new kChunk(25, 5);
  EXPECT_EQ(ch->getfree(), 5) << "getfree 1: " << ch->getfree() << std::endl;
}

TEST(hwTest, TESTkcAdd) {
  uint8_t a = ch->getfree();
  if (!a) return;
  ch->allocate(1);
  EXPECT_EQ(ch->getfree(), a-1);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
