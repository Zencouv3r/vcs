#include <file.hpp>
#include <hash.hpp>
#include <memory>
#include <string>
#include <vector>

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
      if (path == "")
        return;
      name = fs::path(path).filename();
      std::ifstream f(path);
      hash = chash(f);
    };
    ~FileImpl() {};

    FileImpl(FileImpl& oth) : name(oth.name), path(oth.path), hash(oth.hash) {};

    fdata get() {return fdata{name, path, hash};}

    auto bloboficate() {
      return str(get());
    };

    void debloboficate(std::string s) {
      auto data = split(s, '$');
      name = data[0];
      path = data[1];
      hash = std::stoull(data[2]);
    };

    bool operator==(const File& oth) const {
      bool p = this->path == oth.get().path;
      return p;
   }
};

File::File(const std::string& path)
    : impl(std::make_unique<FileImpl>(path)) {}

File::~File() {}

File::File(const File& oth) : impl(std::make_unique<FileImpl>(*oth.impl)) {}

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

bool File::operator==(const File& oth) const {
  return impl->operator==(oth);
}

