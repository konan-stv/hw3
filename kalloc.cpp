#include "kalloc.h"

kChunk::kChunk(std::size_t bs, uint8_t bc) {
  bsize = bs+1;  // 1st byte for next free address
  bfree = bcount = bc;
  printf("KC bs %d bc %d", int(bs), bc);
  chunk = new uint8_t[bsize * bcount];
  if (!chunk)
    throw std::bad_alloc();
  favail = 0;
  uint8_t* p = chunk;
  for (uint8_t i=0; i!=bcount; p+=bsize) *p = ++i;
  //std::cout << "KC free " << bfree << " favail " << favail << std::endl;
  printf(" ok free %d favail %d\r\n", bfree, favail);
}

kChunk::~kChunk() {
  delete [] chunk;
  chunk = nullptr;
//  std::cout << "KF" << std::endl;
  printf("KF free %d favail %d\r\n", bfree, favail);
}

void *kChunk::allocate(std::size_t n) {
  assert(n == 1);
  if (!bfree) {
    printf("Full\r\n");
    return nullptr;
  }
  uint8_t *p = chunk + (favail * bsize);
  favail = *p;
  bfree--;
//  std::cout << "KA free " << bfree << " favail " << favail << std::endl;
  printf("KA free %d favail %d\r\n", bfree, favail);
  return p+1;
}

bool kChunk::deallocate(void *p) {
  uint8_t *tr = (static_cast<uint8_t*>(p))-1;
  if (tr < chunk) return 0;    // not this chunk
  if (tr > (chunk+bsize*bcount)) return 0;    // not this chunk
  assert(((tr-chunk)%bsize) == 0); // bounds check 
//  std::cout << "KD free " << bfree << " favail " << favail;
  printf("KD free %d favail %d", bfree, favail);
  *tr = favail;
  favail = (tr-chunk)/bsize;
  bfree++;
//  std::cout << " new favail " << favail << std::endl;
  printf(" new favail %d new free %d\r\n", favail, bfree);
  return 1;
}

