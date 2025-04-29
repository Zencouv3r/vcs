#pragma once
#include <memory>
#include <unordered_set>

#define REPONAME std::string(".vcs")

typedef std::unordered_set<std::string> strset;

class RepoImpl;
class File;

class Repo 
{
private:
  std::unique_ptr<RepoImpl> impl;
  void log(std::string s);
  void serialize(std::vector<File> &files, std::ifstream f);
  void deserialize();

public:
  Repo();
  ~Repo();
  void init(strset&& args);
  void status(strset&& args);
  void add(strset&& args);
  void commit(strset&& args);
  void list(strset&& args);
  void switchto(strset&& args);
};

