#include <cstdio>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <sys/types.h>
#include <unordered_set>
#include "commit.hpp"
#include "repo.hpp"

namespace fs = std::filesystem;

typedef std::unordered_set<File> fileset;

/* TODO: make it better than O(n) (someday) */
inline bool cmpr_hash(const File &f, const fileset &v) {
  for(const auto &file : v) {
  if (f.get().name == file.get().name &&
      f.get().hash == file.get().hash)
    return true;
  }
  return false;
}

class RepoImpl
{
private:
  void write_to_index(std::unordered_set<File> &files) {
    std::ofstream index(REPONAME + "/INDEX");
    for(auto &file : files) 
      index << file.bloboficate() << "\n";
  };
  void read_from_index(std::unordered_set<File>& files) {
    std::ifstream f(REPONAME +"/INDEX");
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

  void write_logs(std::string s) {
    std::ofstream logf(REPONAME+"/LOG",
                       std::ios::app);
    logf << s;
  }

  void init(strvec&& args) {
    if(fs::exists(REPONAME)) {
      std::cout << "Repo already exist!";
      return;
    }

    fs::create_directory(REPONAME);

    std::ofstream indexf   (REPONAME +"/INDEX");
    std::ofstream logf       (REPONAME +"/LOG");
    std::ofstream curf      (REPONAME + "/CUR");
    fs::create_directory   (REPONAME +"/OBJS/");
    fs::create_directory(REPONAME +"/COMMITS/");

    write_logs("Repo Created Succesfully");

    std::cout << "created!\n";
  }
  
  void status(strvec&& args) {
    if(!fs::exists(REPONAME)) { 
      std::cout << "Repo doesnt exist\n";
      return;
    }


    std::unordered_set<File> actualfiles;
    for (const auto& f : fs::recursive_directory_iterator(".")) {
      if (!f.is_directory()) {
        auto path = f.path().lexically_normal();
        if (path.string()[0] != '.')
          actualfiles.insert(File(path.string()));
      }
    }

    std::unordered_set<File> indxfiles;
    read_from_index(indxfiles);

    std::cout << "Detected unindexed changes: \n";
    for(const auto& f : actualfiles)
      if(!cmpr_hash(f, indxfiles))
        std::cout << fs::path(f.get().path) << "\n";

    // std::cout << "Detected deleted files: \n";
    // for(const auto& f : indxfiles)
    //   if(!cmpr_hash(f, actualfiles))
    //     std::cout << fs::path(f.get().path) << "\n";
    
    // std::cout << "Indexed Files: \n";
    // for(const auto& f : indxfiles)
    //   std::cout << f.bloboficate() << "\n";
    //
    // std::cout << "Actual Files: \n";
    // for(const auto& f : actualfiles)
    //   std::cout << f.bloboficate() << "\n";
  }

  void add(strvec&& args) {
    if (args.empty()) {
      std::cout << "Nothing to index\n";
      return;
    }

    if(!fs::exists(REPONAME)) { 
      std::cout << "Repo doesnt exist\n";
      return;
    }

    std::unordered_set<File> indxfiles;
    read_from_index(indxfiles);

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

  void commit(strvec&& args) {
    std::string msg;

    bool msg_flag = false;

    for (size_t i = 0; i < args.size(); i++) {
      if (args[i] == "-m") {
        msg = args[i+1];
        msg_flag = true;
      }
    }

    if (!msg_flag) {
      std::cerr << "error: commit message required (use -m)\n";
      return;
    }

    std::unordered_set<File> indxfiles;
    read_from_index(indxfiles);

    std::ifstream curf(REPONAME + "/CUR");
    std::string cur_ref_hash = "";

    std::getline (curf, cur_ref_hash);

    curf.close();

    // for (const auto& f : fs::directory_iterator(REPONAME + "/COMMITS"))
    //   Commit cmt(fs::path(f).string());

    Commit cur_commit(indxfiles, msg);
    if (std::to_string(cur_commit.gethash()) == cur_ref_hash) {
      std::cout << "Nothing to commit\n";
      return;
    }

    cur_commit.save();
    std::ofstream outf(REPONAME + "/CUR");
    outf << cur_commit.gethash();
  }

  void list(strvec&& args) {
    for(auto &com : fs::directory_iterator(REPONAME + "/COMMITS")) {
      std::ifstream commit(com.path());
      std::string desc;
      std::getline(commit, desc);
      std::cout << "commit " << com.path().filename() <<
        "\n Date: " << desc << "\n" << "  Description: " << "\n";
    }
  }

  void switchto(strvec&& args) {
    if(args.empty()) {
      std::cout << "not enough args\n";
      return;
    }

    std::unordered_set<File> files;
    
    for(auto &com : fs::directory_iterator(REPONAME + "/COMMITS")) {
      if(args[0] == com.path().filename().string()) {
        std::ifstream commit(com);
        std::string readbuff;
        std::getline(commit, readbuff);
        
        while(std::getline(commit, readbuff)) {
          File file("");
          file.debloboficate(readbuff);
          files.insert(file);
        }

        for(auto &f : fs::directory_iterator(".")) {
          if (f.path().filename().string()[0] != '.') {
            if(f.is_directory())
              fs::remove_all(f.path());
            else
              fs::remove(f.path());
          }
        }

        for (const auto &f : files) {
            fs::path file = f.get().path;
            fs::create_directories(file.parent_path());
            fs::copy(
                REPONAME + "/OBJS/" + std::to_string(f.get().hash),
                file,
                fs::copy_options::overwrite_existing
            );
        }
        std::ofstream curf(REPONAME + "/CUR");
        curf << args[0];
        write_to_index(files);
        std::cout << "Switched to commit " << args[0];
        return;
      } 
    }

    std::cout << "Commit not found";
  }
};

Repo::Repo() : impl(std::make_unique<RepoImpl>()) {}
Repo::~Repo() {}

void Repo::log(std::string s) {
  impl->write_logs(s);
};

void Repo::init(strvec&& args) {
  impl->init(std::move(args));
};

void Repo::status(strvec&& args) {
  impl->status(std::move(args));
}

void Repo::add(strvec&& args) {
  impl->add(std::move(args));
}

void Repo::commit(strvec&& args) {
  impl->commit(std::move(args));
}

void Repo::list(strvec&& args) {
  impl->list(std::move(args));
}

void Repo::switchto(strvec&& args) { 
  impl->switchto(std::move(args));
}
