#include <cstring>
#include <string>
#include <iostream>
#include "common.hpp"
#include "mail_manager.hpp"

MailManager mailbox;

int main(void)
{
  char command[8];
  std::string file_path;
  int id;
  Query q;

  while (std::cin >> command) {
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
