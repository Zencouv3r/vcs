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
    File(File&& oth);
    fdata get() const;
    std::string bloboficate() const;
    void debloboficate(std::string);
};
