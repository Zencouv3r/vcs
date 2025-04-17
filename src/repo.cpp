#include <memory>
#include <string>
#include <repo.hpp>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <file.hpp>
#include <vector>

#define reponame std::string(".vcs")

namespace fs = std::filesystem;

class RepoImpl
{
public: 
  RepoImpl() {};
  ~RepoImpl() {};

  void init() {
    
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
  
  void status() {
    std::vector<File> files;

    for(const auto& f : fs::directory_iterator(".")) {
      if(!f.is_directory())
        files.push_back(f.path().string());
    }

    for(const auto& f : files)
      std::cout << f.get().hash << " " << f.get().name << "\n";

  }
};

Repo::Repo() : impl(std::make_unique<RepoImpl>()) {}
Repo::~Repo() {}

void Repo::init() {
  impl->init();
};

void Repo::log(std::string s) {
  impl->log(s);
};

void Repo::status() {
  impl->status();
}
