#ifndef MAIL_MANAGER_HPP
#define MAIL_MANAGER_HPP
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <map>
#include <queue>
#include "common.hpp"
using namespace std;

class Mail{
public:
  char receiver[64];
  char sender[64];
  Date date;
  unsigned id;
  unordered_set<string>content; // Subject & Content
  unsigned length = 0;
  unsigned poke;
  unsigned query_id = -1;
};

class Mail_date{
public:
  Mail_date():mail(nullptr){};
  Mail_date(Mail *p):mail(p){};
  bool operator<(const Mail_date &obj) const{
    return (mail->date < obj.mail->date) || (mail->date == obj.mail->date && mail->id > obj.mail->id);
  }
  Mail *mail;
};

class Mail_length{
public:
  Mail_length():mail(nullptr){};
  Mail_length(Mail *p):mail(p){};
  bool operator<(const Mail_length &obj) const{
    return (mail->length > obj.mail->length) || (mail->length == obj.mail->length && mail->id < obj.mail->id);
  }
  Mail *mail;
};

class MailManager{
public:
  void add(string &file_path);
  void remove(int id);
  void longest();
  void query(Query q);
private:
  unordered_map<int,Mail *>id2mail;
  unordered_map<string,set<int>>receiver2id;
  unordered_map<string,set<int>>sender2id;
  set<Mail_length>length_set;
  set<Mail_date>date_set;
  unsigned amount = 0;
  unsigned query_id = 0;
};

#endif
