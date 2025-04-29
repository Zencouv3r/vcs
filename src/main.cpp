#include "repo.hpp"
#include <functional>
#include <map>
#include <iostream>

int main(int argc, char *argv[])
{
  Repo repo;

  std::map<std::string, std::function<void(std::unordered_set<std::string>&&)>> cmtable = {
    {"init", std::bind(&Repo::init, &repo, std::placeholders::_1)},
    {"status", std::bind(&Repo::status, &repo, std::placeholders::_1)},
    {"add", std::bind(&Repo::add, &repo, std::placeholders::_1)},
    {"commit", std::bind(&Repo::commit, &repo, std::placeholders::_1)},
    {"list", std::bind(&Repo::list, &repo, std::placeholders::_1)},
  };

  if (argc <= 1) {
    std::cout << "no args";
    return 0;
  }
  
  std::unordered_set<std::string> args;
  if(argc >= 2)
    for(size_t i = 2; i < argc; i++)
      args.insert(argv[i]);

  if (cmtable.contains(argv[1])) {
    try {
    cmtable[argv[1]](std::move(args));
    }
    catch(const std::exception& e) {
      std::cout << "Error: " << e.what() << "\n";
    };
  }
  else {
    std::cout << "Unknown command";
  }

  return 0;
}


