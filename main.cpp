#include <iostream>
#include <memory>
#include <map>
#include <list>
#include <assert.h>
#include "kalloc.h"
#include "version.h"

int main(int argc, char **argv)
{
  std::cout << "Version 0.0." << PROJECT_VERSION_PATCH << std::endl;
  
  auto m = std::map<int, int, std::less<int>, kalloc<std::pair<const int, int>, 5>>{};
  
  for (uint8_t i=1; i<14; i++) m[i] = i*10;
  for (int i=1; i<14; i+=2) m.erase(i);
  for (uint8_t i=1; i<14; i++) m[i+20] = i*10;

  std::cout << "M1" << std::endl;
  return 0;
}
