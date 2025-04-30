#pragma once
#include <memory>
#include <vector>

#define REPONAME std::string(".vcs")

typedef std::vector<std::string> strvec;

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
  void init(strvec&& args);
  void status(strvec&& args);
  void add(strvec&& args);
  void commit(strvec&& args);
  void list(strvec&& args);
  void switchto(strvec&& args);
};

