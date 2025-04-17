#include <file.hpp>
#include <filesystem>
#include <fstream>
#include <hash.hpp>

namespace fs = std::filesystem;

class FileImpl
{
  private:
    std::string name;
    std::string path;
    uint64_t hash;

  public:
    FileImpl(const std::string& path) : path(path) {
      name = fs::path(path).filename();
      std::ifstream f(path);
      hash = chash(f);
    };
    ~FileImpl() {};

    fdata get() {return fdata{name, path, hash};}
};

File::File(const std::string& path)
    : impl(std::make_unique<FileImpl>(path)) {}

File::~File() {}

File::File(File&& oth) : impl(std::move(oth.impl)) {}

fdata File::get() const {
  return impl->get();
};


