#include <memory>
#include <unordered_set>
#include <string>
#include <repo.hpp>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <file.hpp>
#include <vector>

#define reponame std::string(".vcs")

namespace fs = std::filesystem;

typedef std::unordered_set<std::string> strset;
typedef std::unordered_set<File> fileset;

/* TODO: make it not O(n) (someday) */
inline bool cmpr_hash(const File &f, const fileset &v) {
  for(const auto &file : v) {
    if (f.get().hash == file.get().hash)
      return true;
  }
  return false;
}

class RepoImpl
{
private:
  void write_to_index(std::unordered_set<File> &files) {
    std::ofstream index(reponame + "/INDEX");
    for(auto &file : files) 
      index << file.bloboficate() << "\n";

    index.close();
  };
  void deserialize(std::unordered_set<File>& files, std::ifstream& f) {
    std::string s;
    while(std::getline(f, s)) {
      File newf("");
      newf.debloboficate(s);
      files.insert(std::move(newf));
    }
  };
public: 
  RepoImpl() {};
  ~RepoImpl() {};

  void init(strset&& args) {
    
    if(fs::exists(".vcs")) {
      std::cout << "Repo already exist!";
      return;
    }

    fs::create_directory(reponame);

    std::ofstream indexf(reponame +"/INDEX");
    std::ofstream logf(reponame +"/LOG");

    log("Repo Created Succesfully");

    std::cout << "created!\n";
  }

  void log(std::string s) {
    std::ofstream logf(reponame+"/LOG",
                       std::ios::app);
    logf << s;
  }
  
  void status(strset&& args) {
    std::ifstream index(reponame +"/INDEX");

    std::vector<File> actualfiles;
    for (const auto& f : fs::recursive_directory_iterator(".")) {
      if (!f.is_directory()) {
        auto path = f.path().lexically_normal();
        if (path.string()[0] != '.')
          actualfiles.push_back(path.string());
      }
    }

    std::unordered_set<File> indxfiles;
    deserialize(indxfiles, index);

    std::cout << "Detected unindexed changes: \n";
    for(const auto& f : actualfiles)
      if(!cmpr_hash(f, indxfiles))
        std::cout << fs::path(f.get().path) << "\n";
    
    // std::cout << "Indexed Files: \n";
    // for(const auto& f : indxfiles)
    //   std::cout << f.bloboficate() << "\n";
    //
    // std::cout << "Actual Files: \n";
    // for(const auto& f : actualfiles)
    //   std::cout << f.bloboficate() << "\n";
  }

  void add(strset&& args) {
    if (args.empty()) {
      std::cout << "Nothing to index\n";
      return;
    }

    std::ifstream index(reponame +"/INDEX");
    std::unordered_set<File> indxfiles;
    deserialize(indxfiles, index);

    /* TODO: make hidden files unaddable */ 
    for(const auto& f : args) {
      auto path = fs::absolute(f).string();
      if (!fs::exists(path)) {
        std::cout << "File " << fs::path(f).filename() << " Not Found\n";
      } else {
      File newf((fs::absolute(f)));
      if(indxfiles.contains(newf)) {
        indxfiles.erase(newf);
        indxfiles.insert(newf);
        }
      else
        indxfiles.insert(newf);
      }
    }

    write_to_index(indxfiles);
  }
};

Repo::Repo() : impl(std::make_unique<RepoImpl>()) {}
Repo::~Repo() {}

void Repo::log(std::string s) {
  impl->log(s);
};

void Repo::init(strset&& args) {
  impl->init(std::move(args));
};

void Repo::status(strset&& args) {
  impl->status(std::move(args));
}

void Repo::add(strset&& args) {
  impl->add(std::move(args));
}
