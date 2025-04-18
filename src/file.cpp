#include <file.hpp>
#include <hash.hpp>
#include <vector>
#include <iostream>

namespace fs = std::filesystem;

typedef std::string str;

inline std::vector<str> split(str s, char lt) {
  std::vector<str>  strings;
  const char* pleft = &s[0];
  const char* prght = &s[0];
  while(*prght != '\0') {
    prght++;
    if(*prght == lt) {
      strings.push_back(str(pleft, prght));
      pleft = prght+1;
    }
  }
  return strings;
}

class FileImpl
{
  private:
    std::string name;
    std::string path;
    uint64_t    hash;

  public:
    FileImpl(const std::string& path) : path(path) {
      name = fs::path(path).filename();
      std::ifstream f(path);
      hash = chash(f);
    };
    ~FileImpl() {};

    fdata get() {return fdata{name, path, hash};}

    auto bloboficate() {
      return str(get());
    };

    void debloboficate(std::string s) {
      auto data = split(s, '$');
      name = data[0];
      hash = data[1];
      path = data[2];
    };
};

File::File(const std::string& path)
    : impl(std::make_unique<FileImpl>(path)) {}

File::~File() {}

File::File(File&& oth) : impl(std::move(oth.impl)) {}

fdata File::get() const {
  return impl->get();
};

std::string File::bloboficate() const {
  return impl->bloboficate();
}

void File::debloboficate(std::string s) {
  impl->debloboficate(s);
}


