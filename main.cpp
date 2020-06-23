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
  // Fast I/O
  // ios_base::sync_with_stdio(false);
  // cin.tie(NULL);
  char command[8];
  String file_path;
  int id;
  FastQuery q;

  while (scanf("%s", command) != EOF) {
    if (command[0] == 'a') {
      cin >> file_path;
      mailbox.add(file_path);
    } else if (command[0] == 'r') {
      fastscan(id);
      mailbox.remove(id);
    } else if (command[0] == 'l') {
      mailbox.longest();
    } else if (command[0] == 'q') {
      read(q);
      mailbox.query(q);
    }
  }
  return 0;
}
