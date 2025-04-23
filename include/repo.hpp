#pragma once
#include <memory>
#include <unordered_set>

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
  void init(std::unordered_set<std::string>&& args);
  void status(std::unordered_set<std::string>&& args);
  void add(std::unordered_set<std::string>&& args);
};

