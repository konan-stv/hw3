#include <iostream>
#include <memory>
#include <map>
#include <list>
#include <assert.h>
#include "kalloc.h"
#include "kcont.h"
#include "version.h"

unsigned int fact(unsigned int n) {
  if (n == 0) return 0;
  if (n == 1) return 1;
  else return n*fact(n-1);
}

int main(int argc, char **argv)
{
  (void)argc;
  (void)argv;

  std::cout << "Version 0.0." << PROJECT_VERSION_PATCH << std::endl;
  
  auto m = std::map<int, int, std::less<int>, kalloc<std::pair<const int, int>, 10>>{};
  
  for (int i=0; i<10; i++) m[i] = fact(i);
  for (int i=0; i<10; i++) std::cout << i << " " << m[i] << std::endl;

  //std::cout << "M1" << std::endl;
  return 0;
}
