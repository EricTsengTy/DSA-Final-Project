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
    if (strcmp(command, "add")) {
      cin >> file_path;
      mailbox.add(file_path);
    } else if (strcmp(command, "remove")) {
      cin >> id;
      mailbox.remove(id);
    } else if (strcmp(command, "longest")) {
      mailbox.longest();
    } else if (strcmp(command, "query")) {
      cin >> q;
      mailbox.query(q);
    }
  }
  return 0;
}
