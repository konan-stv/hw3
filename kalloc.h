#ifndef KALLOC_H
#define KALLOC_H
#include <iostream>
#include <memory>
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
    lastA = nullptr;

    if (chunks.size()) {
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
    }
    
    if (!lastA) {   // create new chunk
      chunks.emplace_back(sizeof(T), chsize);
      lastA = &chunks.back();
      std::cout << "C" << std::endl;
    }
    
    return reinterpret_cast<T *>(lastA->allocate(n));
  }

  void deallocate(T *p, std::size_t n) {
    assert(chunks.size() != 0);
    std::cout << "A2: " << chsize << std::endl;
    if (!lastA->deallocate(p)) {
      for (auto i=chunks.rbegin(); i!=chunks.rend(); i++) 
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
  }

  void destroy(T *p) {
    p->~T();
    std::cout << "A4: " << chsize << std::endl;
  }
};

#endif    // KALLOC_H