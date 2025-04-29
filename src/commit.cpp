#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include "commit.hpp"
#include "repo.hpp"


namespace fs = std::filesystem;
class CommitImpl {
private:
  const std::unordered_set<File>  files;
  std::string               description;
  uint64_t                         hash;
public:
  CommitImpl(std::unordered_set<File>& files, std::string desc) 
  : files(files), description(desc), hash(0x0) {
    for(const auto& f : files)
      hash+= f.get().hash;
  }

  CommitImpl(std::unordered_set<File>&& files, std::string desc) 
  : files(std::move(files)), description(desc), hash(0x0) {
    for(const auto& f : files)
      hash+= f.get().hash;
  }

  CommitImpl(std::string path) : hash(std::stoull(fs::path(path).filename())) {
    std::cout << hash;
  };

  void save() const {
    std::string name = std::to_string(hash);
    std::ofstream cmt(REPONAME+"/COMMITS/"+name);

    cmt << description + "\n";

    for(const auto& f : files) {
      cmt << f.bloboficate() << "\n";
      std::string hname = std::to_string(f.get().hash);
      if(!fs::exists(REPONAME+"/OBJS/"+hname))
        fs::copy(f.get().path, REPONAME+"/OBJS/"+hname);
    }
    
  }

  uint64_t gethash() const {
    return hash;
  }
};

Commit::Commit(std::unordered_set<File>& files, const std::string desc) :
  impl(std::make_unique<CommitImpl>(files, desc)) {};

Commit::Commit(std::unordered_set<File>&& files, const std::string desc) 
: impl(std::make_unique<CommitImpl>(std::move(files), desc)) {}

Commit::Commit(const std::string path) : impl(std::make_unique<CommitImpl>(path)) {};

Commit::~Commit() = default;

void Commit::save() const {
  impl->save();
}

uint64_t Commit::gethash() const {
  return impl->gethash();
}
