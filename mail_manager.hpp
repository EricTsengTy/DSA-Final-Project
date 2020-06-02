#ifndef MAIL_MANAGER_HPP
#define MAIL_MANAGER_HPP
#include <string>
#include "common.hpp"
using std::string;

class MailManager{
public:
  void add(string file_path);
  void remove(int id);
  void longest();
  void query(Query q);
  int a;
};

#endif
