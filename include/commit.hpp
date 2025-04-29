#include "file.hpp"
#include <unordered_set>

class CommitImpl;

class Commit { 
private:
  std::unique_ptr<CommitImpl> impl;
public:
  Commit(std::unordered_set<File>& files, const std::string desc);
  Commit(std::unordered_set<File>&& files, const std::string desc);
  Commit(const std::string path);
  ~Commit();
  void save() const;
  uint64_t gethash() const;
};
