#ifndef MAIL_MANAGER_HPP
#define MAIL_MANAGER_HPP
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <queue>
#include <ext/pb_ds/assoc_container.hpp>
#include "common.hpp"
using namespace std;
using namespace __gnu_pbds;
inline void str2lower(char *str);
inline void str2lower(String &str);

unsigned long djb2_hash(const String &str);

struct StringHasher{
  size_t operator()(const String &obj) const{
    return djb2_hash(obj);
  }
};

class Mail{
public:
  Mail() = default;
  Mail(unsigned i):id(i){};
  String receiver;
  String sender;
  Date date;
  unsigned id;
  unordered_set<String, StringHasher>content; // Subject & Content
  unsigned length = 0;
  unsigned poke;
  unsigned query_id = 0;
  bool remove = false;
  bool length_remove = false;
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
  Expression(String &e):operand(true),expression(e){}
  Expression(const char &o):op(o),operand(false){
    switch(o){
      case '(':
        priority = -8;
        break;
      case ')':
        priority = -4;
        break;
      case '!':
        priority = 0;
        break;
      case '&':
        priority = -1;
        break;
      case '|':
        priority = -2;
        break;
    }
  };
  String expression;
  char op;
  int priority;
  bool operand;
};

class ExpNode{
public:
  char op;
  String expression;
  bool negate = false;
  ExpNode *left = nullptr;
  ExpNode *right = nullptr;
  bool operand;
  unsigned depth;
};
class ExpTree{
public:
  ~ExpTree(){
    _free_node(root);
  }
  void post2tree(vector<Expression *>&post);
  ExpNode *root = nullptr;
private:
  void _post2tree_node(vector<Expression *>&post, int &index, ExpNode *&node);
  void _free_node(ExpNode *&node);
};

class FastQuery{
public:
  friend std::istream &read(std::istream &is, Query &obj);
  String receiver;
  String sender;
  Date start_date = 0;  // default min
  Date end_date = ~0;   // default max
  Expression expression[128]; // Maybe would change to array or something
  int count = 0;
  bool exist_receiver = false, exist_sender = false,
    exist_start_date = false, exist_end_date = false;
};
/* Another read option */
void read(FastQuery &obj);

class MailManager{
public:
  void add(String &file_path);
  void remove(int id);
  void longest();
  void query(FastQuery &q);
private:
  void _matching(vector<unsigned>&ids, vector<Mail *>&mails, ExpTree &exp_tree);
  void _matching(vector<unsigned>&ids, ExpTree &exp_tree);
  bool _valid_mail(unordered_set<String, StringHasher>&content, ExpNode *&node);
  void _add_data(Mail *&mail);
  gp_hash_table<String, Mail *, StringHasher>id_cache;
  gp_hash_table<int,Mail *>id2mail;
  gp_hash_table<String,unordered_set<Mail *>, StringHasher>receiver2id;
  gp_hash_table<String,unordered_set<Mail *>, StringHasher>sender2id;
  priority_queue<Mail_length>length_max_queue;
  set<Mail_date>date_set;
  unsigned amount = 0;
  unsigned query_id = 1;
  Mail mail_beg = 0U;
  Mail mail_end = UINT32_MAX;
  ExpTree exp_tree;
};

#endif
