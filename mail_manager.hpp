#ifndef MAIL_MANAGER_HPP
#define MAIL_MANAGER_HPP
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <queue>
#include "common.hpp"
using namespace std;

void str2lower(char *str);
void str2lower(string &str);

class Mail{
public:
  string receiver;
  string sender;
  Date date;
  unsigned id;
  unordered_set<string>content; // Subject & Content
  unsigned length = 0;
  unsigned poke;
  unsigned query_id = 0;
  bool remove = false;
  bool length_remove = true; // Bad Implementation
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
    return (mail->length < obj.mail->length) || (mail->length == obj.mail->length && mail->id > obj.mail->id);
  }
  Mail *mail;
};

class Expression{
  public:
  Expression() = default;
  Expression(string &e):operand(true),expression(e){}
  Expression(const char &o):op(o),operand(false){
    switch(o){
      case '(':
        priority = -1;
        break;
      case '!':
        priority = 5;
        break;
      case '&':
        priority = 4;
        break;
      case '|':
        priority = 3;
        break;
      case ')':
        priority = 0;
        break;
    }
  };
  string expression;
  char op;
  int priority;
  bool operand;
};

class ExpNode{
public:
  char op;
  string expression;
  bool negate = false;
  ExpNode *left = nullptr;
  ExpNode *right = nullptr;
  unsigned depth;
};
class ExpTree{
public:
  ~ExpTree(){
    _free_node(root);
  }
  void post2tree(vector<Expression>&post);
  ExpNode *root = nullptr;
private:
  ExpNode *_post2tree_node(vector<Expression>&post, int &index);
  void _free_node(ExpNode *&node);
};

class MailManager{
public:
  void add(string &file_path);
  void remove(int id);
  void longest();
  void query(Query &q);
private:
  void _matching(vector<unsigned>&ids, vector<Mail *>&mails, ExpTree &exp_tree);
  void _matching(vector<unsigned>&ids, ExpTree &exp_tree);
  bool _valid_mail(unordered_set<string>&content, ExpNode *&node);
  void _add_data(Mail *&mail);
  unordered_map<string,int>id_cache;
  unordered_map<int,Mail *>id2mail;
  unordered_map<string,unordered_set<Mail *>>receiver2id;
  unordered_map<string,unordered_set<Mail *>>sender2id;
  priority_queue<Mail_length>length_max_queue;
  set<Mail_date>date_set;
  unsigned amount = 0;
  unsigned query_id = 1;
};

#endif
