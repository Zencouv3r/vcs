#include "hash.hpp"
#include <vector>

uint64_t chash(std::ifstream& file) {

  file.seekg(0, std::ios::end);
  std::streamsize size = file.tellg();
  file.seekg(0, std::ios::beg);

  std::vector<char> buff(size);

  file.read(buff.data(), size);
  
  uint64_t hash = 0x0;

  for (auto c : buff) {
    hash ^= static_cast<uint8_t>(c);
    hash = (hash << 5) | (hash >> (64 - 5));
  }

  return hash;
}
