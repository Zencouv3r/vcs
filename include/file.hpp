#include <memory>
#include <string>

class FileImpl;

typedef struct fdata {
  std::string name;
  std::string path;
  uint64_t    hash;
  
  operator std::string() const {
    return name + "$" + path + "$" + std::to_string(hash) + "$";
  }
} fdata;

class File
{ 
  private:
    std::unique_ptr<FileImpl> impl;

  public:
    File(const std::string& path);
    ~File();
    File(const File& oth);
    File(File&& oth);
    fdata get() const;
    std::string bloboficate() const;
    void debloboficate(std::string);
    
    bool operator==(const File& oth) const;
};

namespace std {
  template<>
  struct hash<File> {
    size_t operator()(const File& f) const noexcept {
      return std::hash<std::string>()(f.get().path);
    }
  };
}
