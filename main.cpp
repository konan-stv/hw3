#include <iostream>
#include <memory>
#include <map>
#include <list>
#include <assert.h>

struct kChunk {
  std::size_t bsize;
  uint8_t bcount;
  uint8_t bfree;
  uint8_t *chunk;
  uint8_t favail;

  kChunk(std::size_t bs, uint8_t bc);
  ~kChunk();
  void *allocate(std::size_t n);
  bool deallocate(void *p);
  unsigned int getfree() {return bfree;}
};

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

template<typename T, unsigned int chsize>
struct kalloc {
  using value_type = T;
  using pointer = T*;
  using const_pointer = const T*;
  using reference = T&;
  using const_reference = const T&;
  
  kChunk *lastA, *lastD;   // cache - last allocated and last deallocated
  std::list<kChunk> chunks;

  template<typename U>
  struct rebind {
    using other = kalloc<U, chsize>;
  };

  kalloc() {lastA = lastD = nullptr;}
  ~kalloc() {}

  template<typename U, unsigned int a> 
  kalloc(const kalloc<U, a>) {}

  T *allocate(std::size_t n) {
    if (!chunks.size()) {
      chunks.emplace_back(sizeof(T), chsize);
      lastA = &chunks.back();
      std::cout << "C1" << std::endl;
    } else {
      lastA = nullptr;
      if (lastD) {    // check last deallocated
        if (lastD->getfree())
          lastA = lastD;
      }
      if (!lastA) {   // then check existing chunks
        for (auto i=chunks.rbegin(); i!=chunks.rend(); i++) 
          if (i->getfree()) {
            lastA = &(*i);    // god bless c++ 
            break;
          }
      }
      if (!lastA) {   // create new chank
        chunks.emplace_back(sizeof(T), chsize);
        lastA = &chunks.back();
        std::cout << "C" << std::endl;
      }
    }
    
    return reinterpret_cast<T *>(lastA->allocate(n));
  }

  void deallocate(T *p, std::size_t n) {
    assert(chunks.size() != 0);
    std::cout << "A2: " << chsize << std::endl;
    if (!lastA->deallocate(p)) {
      for (auto i=chunks.begin(); i!=chunks.end(); i++) 
        if (i->deallocate(p)) {
          lastD = &(*i);
          return;
        }
    } else lastD = lastA;
  }

  template<typename U, typename ...Args>
  void construct(U *p, Args &&...args) {
    new(p) U(std::forward<Args>(args)...);
    std::cout << "A3: " << chsize << std::endl;
  };

  void destroy(T *p) {
    p->~T();
    std::cout << "A4: " << chsize << std::endl;
  }
};

int main(int argc, char **argv)
{
  auto m = std::map<int, int, std::less<int>, kalloc<std::pair<const int, int>, 5>>{};
  
  for (uint8_t i=1; i<14; i++) m[i] = i*10;

  std::cout << "M1" << std::endl;
  return 0;
}
