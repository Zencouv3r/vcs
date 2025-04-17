#include <memory>
#include <string>

class FileImpl;

typedef struct fdata {
  std::string name;
  std::string path;
  uint64_t hash;
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
};
