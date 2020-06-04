#ifndef MAIL_MANAGER_HPP
#define MAIL_MANAGER_HPP
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <map>
#include <deque>
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
  unsigned query_id = 0;
  bool remove = false;
};

class Mail_date{
public:
  Mail_date():mail(nullptr){};
  Mail_date(Mail *p):mail(p){};
  bool operator<(const Mail_date &obj) const{
    return (mail->date < obj.mail->date) || (mail->date == obj.mail->date && mail->id < obj.mail->id);
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

class Expression{
  public:
  Expression() = default;
  Expression(char e[24]):operand(true){
    strcpy(expression, e);
  }
  Expression(char o):op(o),operand(false){
    if (o == '(')
      priority = -1;
    else if (o == '!')
      priority = 5;
    else if (o == '&')
      priority = 4;
    else if (o == '|')
      priority = 3;
    else if (o == ')')
      priority = 0;
  };
  char expression[24];
  char op;
  int priority;
  bool operand;
};
/*
class ExpCalc{
public:
  ExpCalc(string s):expression(s){};
  string expression;
  bool extract = false;
  bool value;
  bool negate = false;
};
*/
class MailManager{
public:
  void add(string &file_path);
  void remove(int id);
  void longest();
  void query(Query &q);
private:
  void _matching(set<unsigned>&ids, vector<Mail *>&mails, deque<Expression>&exp_pool);
  void _matching(set<unsigned>&ids, deque<Expression>&exp_pool);
  void _add_data(Mail *&mail);
  unordered_map<int,Mail *>id2mail;
  unordered_map<string,unordered_set<int>>receiver2id;
  unordered_map<string,unordered_set<int>>sender2id;
  set<Mail_length>length_set;
  set<Mail_date>date_set;
  unsigned amount = 0;
  unsigned query_id = 1;
};

#endif
