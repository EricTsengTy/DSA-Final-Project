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

  while (scanf("%s", command) != EOF) {
    if (command[0] == 'a') {
      cin >> file_path;
      mailbox.add(file_path);
    } else if (command[0] == 'r') {
      cin >> id;
      mailbox.remove(id);
    } else if (command[0] == 'l') {
      mailbox.longest();
    } else if (command[0] == 'q') {
      cin >> q;
      mailbox.query(q);
    }
  }
  return 0;
}
