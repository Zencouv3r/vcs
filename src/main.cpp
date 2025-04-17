#include "repo.hpp"
#include <functional>
#include <map>
#include <iostream>
#include <file.hpp>

int main(int argc, char *argv[])
{
  Repo repo;

  std::map<std::string, std::function<void()>> cmtable = {
    {"init", std::bind(&Repo::init, &repo)},
    {"status", std::bind(&Repo::status, &repo)},
  };

  if (argc <= 1) {
    std::cout << "no args";
    return 0;
  }

  if (cmtable.contains(argv[1])) {
    cmtable[argv[1]]();
  }
  else {
    std::cout << "Unknown command";
  }

  return 0;
}


