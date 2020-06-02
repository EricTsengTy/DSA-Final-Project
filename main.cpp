#include <cstring>
#include <string>
#include <iostream>
#include "common.hpp"
#include "mail_manager.hpp"

MailManager mailbox;

int main(void)
{
  using std::cin;
  using std::string;
  
  char command[8];
  string file_path;
  int id;
  Query q;

  while (cin >> command) {
    if (strcmp(command, "add") == 0) {
      cin >> file_path;
      mailbox.add(file_path);
    } else if (strcmp(command, "remove") == 0) {
      cin >> id;
      mailbox.remove(id);
    } else if (strcmp(command, "longest") == 0) {
      mailbox.longest();
    } else if (strcmp(command, "query") == 0) {
      cin >> q;
      mailbox.query(q);
    }
  }
  return 0;
}
