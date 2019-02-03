#include <string>
#include "kalloc.h"
#include <gtest/gtest.h>

kChunk *ch;
void *b[10];

TEST(hwTest, TESTkcCreate) {
  ch = new kChunk(25, 5);
  EXPECT_EQ(ch->getfree(), 5);
}

TEST(hwTest, TESTkcAdd) {
  uint8_t a = ch->getfree();
  if (!a) return;
  b[0] = ch->allocate(1);
  EXPECT_EQ(ch->getfree(), a-1);
  b[1] = ch->allocate(1);
  EXPECT_EQ(ch->getfree(), a-2);
  b[2] = ch->allocate(1);
  EXPECT_EQ(ch->getfree(), a-3);
  b[3] = ch->allocate(1);
  EXPECT_EQ(ch->getfree(), a-4);
}

TEST(hwTest, TESTkcShuffle) {
  uint8_t a = ch->getfree();
  ch->deallocate(b[2]);
  EXPECT_EQ(ch->getfree(), a+1);
  ch->deallocate(b[0]);
  EXPECT_EQ(ch->getfree(), a+2);
  b[4] = ch->allocate(1);
  EXPECT_EQ(ch->getfree(), a+1);
  b[5] = ch->allocate(1);
  EXPECT_EQ(ch->getfree(), a);
  ch->deallocate(b[1]);
  EXPECT_EQ(ch->getfree(), a+1);
  b[6] = ch->allocate(1);
  EXPECT_EQ(ch->getfree(), a);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
