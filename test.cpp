#include <string>
#include "kalloc.h"
#include <gtest/gtest.h>

kChunk *ch;
void* a[10];

TEST(hwTest, TESTkcCreate) {
  ch = new kChunk(25, 5);
  EXPECT_EQ(ch->getfree(), 5) << "getfree 1: " << ch->getfree() << std::endl;
}

TEST(hwTest, TESTkcAdd) {
  uint8_t a = ch->getfree();
  if (!a) return;
  a[0] = ch->allocate(1);
  EXPECT_EQ(ch->getfree(), a-1);
  a[1] = ch->allocate(1);
  EXPECT_EQ(ch->getfree(), a-2);
  a[2] = ch->allocate(1);
  EXPECT_EQ(ch->getfree(), a-3);
  a[3] = ch->allocate(1);
  EXPECT_EQ(ch->getfree(), a-4);
}

TEST(hwTest, TESTkcShuffle) {
  uint8_t a = ch->getfree();
  ch->deallocate(a[2]);
  EXPECT_EQ(ch->getfree(), a+1);
  ch->deallocate(a[0]);
  EXPECT_EQ(ch->getfree(), a+2);
  a[4] = ch->allocate(1);
  EXPECT_EQ(ch->getfree(), a+1);
  a[5] = ch->allocate(1);
  EXPECT_EQ(ch->getfree(), a);
  ch->deallocate(a[1]);
  EXPECT_EQ(ch->getfree(), a+1);
  a[6] = ch->allocate(1);
  EXPECT_EQ(ch->getfree(), a);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
