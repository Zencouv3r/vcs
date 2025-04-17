#pragma once
#include <memory>

class RepoImpl;
  
class Repo 
{
private:
  std::unique_ptr<RepoImpl> impl;
  void log(std::string s);

public:
  Repo();
  ~Repo();
  void init();
  void status();
};
