#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <algorithm>
#include "mail_manager.hpp"
using namespace std;
unordered_map<string,unsigned,StringHasher> monthTransform = {
  pair<string,unsigned>("January", 1),
  pair<string,unsigned>("February", 2),
  pair<string,unsigned>("March", 3),
  pair<string,unsigned>("April", 4),
  pair<string,unsigned>("May", 5),
  pair<string,unsigned>("June", 6),
  pair<string,unsigned>("July", 7),
  pair<string,unsigned>("August", 8),
  pair<string,unsigned>("September", 9),
  pair<string,unsigned>("October", 10),
  pair<string,unsigned>("November", 11),
  pair<string,unsigned>("December", 12),
};

unsigned long djb2_hash(const string &str){
  unsigned long value = 5381;
  for (const auto &i : str){
    value = ((value << 5) + value) + i;
  }
  return value;
}

/*
unsigned month2num(string &str){
  switch(str[0]){
    case 'J':
      switch(str[3]){
        case 'u':
          return 1;
        case 'e':
          return 6;
        case 'y':
          return 7;
      }
    case 'F':
      return 2;
    case 'M':
      switch(str[2]){
        case 'r':
          return 3;
        case 'y':
          return 5;
      }
    case 'A':
      switch(str[1]){
        case 'p':
          return 4;
        case 'u':
          return 8;
      }
    case 'S':
      return 9;
    case 'O':
      return 10;
    case 'N':
      return 11;
    case 'D':
      return 12;
  }
}
*/
inline void str2lower(char *str){
  for (int i = 0, len = strlen(str); i != len; ++i)
    str[i] = tolower(str[i]);
}

inline void str2lower(string &str){
  for (auto &i : str)
    i = tolower(i);
}

void ExpTree::_free_node(ExpNode *&node){
  if (node == nullptr)
    return;
  vector<ExpNode *>nodes;
  nodes.push_back(node);
  while (!nodes.empty()){
    ExpNode *p = nodes.back();
    nodes.pop_back();
    if (p->left != nullptr)
      nodes.push_back(p->left);
    if (p->right != nullptr)
      nodes.push_back(p->right);
    delete p;
  }
}
void ExpTree::post2tree(vector<Expression *>&post){
  int length = post.size() - 1;
  _post2tree_node(post, length, root);
}

void ExpTree::_post2tree_node(vector<Expression *>&post, int &index, ExpNode *&node){
  if (node == nullptr)
    node = new ExpNode;
  node->negate = false;
  while (!post[index]->operand && post[index]->op == '!'){
    node->negate = !node->negate;
    --index;
  }
  if (post[index]->operand){
    node->expression = post[index]->expression;
    node->depth = 0;
    node->operand = true;
    --index;
  }
  else{
    node->operand = false;
    node->op = post[index]->op;
    --index;
    _post2tree_node(post, index, node->right);
    _post2tree_node(post, index, node->left);
    node->depth = max(node->left->depth, node->right->depth) + 1;
  }
}
void MailManager::add(string &file_path){
  auto p = id_cache.find(file_path);
  if (p != id_cache.end()){
    if (p->second->remove){
      if (p->second->length_remove){
        length_max_queue.push(p->second);
        p->second->length_remove = false;
      }
      p->second->remove = false;
      ++amount;
      cout << amount << '\n';
    }
    else
      cout << '-' << '\n';
    return;
  }
  ifstream fin;
  fin.open(file_path);
  Mail *mail = new Mail;
  char keyword[16], buf;
  // From
  fin >> keyword >> mail->sender;
  str2lower(mail->sender);

  // Date
  unsigned year, date, hour, minute;
  string buffer;
  fin >> keyword >> date >> buffer >> year >> keyword >> hour >> buf >> minute;
  mail->date.set_value(year, monthTransform[buffer], date, hour, minute);
  
  // ID
  fin >> keyword >> mail->id;
  // Subject
  fin >> keyword;
  getline(fin, buffer);
  string word;
  for (const auto &i : buffer){
    if (!isalnum(i)){
      if (!word.empty()){
        mail->content.insert(word);
        word.clear();
      }
    }
    else
      word.push_back(tolower(i));
  }
  if (!word.empty()){
    mail->content.insert(word);
    word.clear();
  }
  
  // To
  fin >> keyword >> mail->receiver;
  str2lower(mail->receiver);

  // Content
  fin >> keyword >> noskipws >> buf;
  while (fin >> buf){
    if (!isalnum(buf)){
      if (!word.empty()){
        mail->content.insert(word);
        word.clear();
      }
    }
    else{
      ++mail->length;
      word.push_back(tolower(buf));
    }
  }
  if (!word.empty()){
    mail->content.insert(word);
    word.clear();
  }
  fin.close();
  id2mail[mail->id] = mail;
  // Add to Some Data Structure
  id_cache[file_path] = mail;
  receiver2id[mail->receiver].insert(mail);
  sender2id[mail->sender].insert(mail);
  length_max_queue.push(mail);
  date_set.insert(mail);
  ++amount;
  cout << amount << '\n';
}

void MailManager::remove(int id){
  auto p = id2mail.find(id);
  if (p == id2mail.end() || p->second->remove){
    cout << '-' << '\n';
    return;
  }
  p->second->remove = true;
  --amount;
  cout << amount << '\n';
}

void MailManager::longest(){
  if (amount == 0)
    cout << '-' << '\n';
  else{
    while (length_max_queue.top().mail->remove){
      length_max_queue.top().mail->length_remove = true;
      length_max_queue.pop();
    }
    cout << length_max_queue.top().mail->id << ' ' << length_max_queue.top().mail->length << '\n';
  }
}
bool MailManager::_valid_mail(unordered_set<string, StringHasher>&content, ExpNode *&node){
  if (node->operand)
    return (content.find(node->expression) != content.end()) ^ node->negate;
  if (node->left->depth < node->right->depth){
    if (node->op == '&')
      return (_valid_mail(content, node->left) && _valid_mail(content, node->right)) ^ node->negate;
    else
      return (_valid_mail(content, node->left) || _valid_mail(content, node->right)) ^ node->negate;
  }
  else{
    if (node->op == '&')
      return (_valid_mail(content, node->right) && _valid_mail(content, node->left)) ^ node->negate;
    else
      return (_valid_mail(content, node->right) || _valid_mail(content, node->left)) ^ node->negate;
  }
}

void MailManager::_matching(vector<unsigned>&ids, vector<Mail *>&mails, ExpTree &exp_tree){
  for (auto &i : mails){
    if (i->remove)
      continue;
    if (_valid_mail(i->content, exp_tree.root))
      ids.push_back(i->id);
  }
}
void MailManager::_matching(vector<unsigned>&ids, ExpTree &exp_tree){
  for (auto &pairs : id2mail){
    if (pairs.second->remove)
      continue;
    if (_valid_mail(pairs.second->content, exp_tree.root))
      ids.push_back(pairs.second->id);
  }
}
  

void MailManager::query(FastQuery &q){
  vector<Mail *>mails;
  unsigned filter = (q.exist_sender ? 1 : 0)
                  + (q.exist_receiver ? 1 : 0)
                  + ((q.exist_start_date || q.exist_end_date) ? 1 : 0);
  bool date_check = (q.exist_start_date || q.exist_end_date);
  if (q.exist_sender){
    str2lower(q.sender);
    for (auto &mail : sender2id[q.sender]){
      if (mail->query_id != query_id){
        mail->query_id = query_id;
        mail->poke = 0;
      }
      ++mail->poke;
      if (date_check && q.start_date <= mail->date && mail->date <= q.end_date)
        ++mail->poke;
      if (mail->poke == filter)
        mails.push_back(mail);
    }
    date_check = false;
  }
  if (q.exist_receiver){
    str2lower(q.receiver);
    for (auto &mail : receiver2id[q.receiver]){
      if (mail->query_id != query_id){
        mail->query_id = query_id;
        mail->poke = 0;
      }
      ++mail->poke;
      if (date_check && q.start_date <= mail->date && mail->date <= q.end_date)
        ++mail->poke;
      if (mail->poke == filter)
        mails.push_back(mail);
    }
    date_check = false;
  }
  if (date_check){
    mail_beg.date = q.start_date;
    mail_end.date = q.end_date;
    auto beg = date_set.lower_bound(&mail_beg);
    auto tail = date_set.upper_bound(&mail_end);
    for (; beg != tail; ++beg){
      if (beg->mail->query_id != query_id){
        beg->mail->query_id = query_id;
        beg->mail->poke = 0;
      }
      ++beg->mail->poke;
      if (beg->mail->poke == filter)
        mails.push_back(beg->mail);
    }
  }
  /* Expression */
  vector<Expression *>exp_pool, op_pool;
  string word;
  int count = 0;
  
  /* 7 seconds */
  Expression left;
  left.priority = -256;
  op_pool.push_back(&left);
  for (int k = 0; k != q.count; ++k){
    Expression &i = q.expression[k];
    if (!i.operand){
      if (i.op == '(')
        op_pool.push_back(&i);
      else{
                              /* Strange Implmentation Haha */
        while ((i.priority << 1) < op_pool.back()->priority){
          exp_pool.push_back(op_pool.back());
          op_pool.pop_back();
        }
        if (i.op == ')')
          op_pool.pop_back();
        else
          op_pool.push_back(&i);
      }
    }
    else
      exp_pool.push_back(&i);
  }
  while (op_pool.size() != 1){
    exp_pool.push_back(op_pool.back());
    op_pool.pop_back();
  }
  
  exp_tree.post2tree(exp_pool);
  
  /* Find ids that match expression */
  vector<unsigned>ids;
  if (filter != 0)
    _matching(ids, mails, exp_tree);
  else
    _matching(ids, exp_tree);
  sort(ids.begin(), ids.end());
  if (ids.empty())
    cout << '-' << '\n';
  else{
    auto p = ids.begin();
    cout << *p;
    ++p;
    for (; p != ids.end(); ++p)
      cout << ' ' << *p;
    cout << '\n';
  }
  ++query_id;
}

/* Another read option */
void read(FastQuery &obj){
  obj.exist_end_date = obj.exist_receiver = obj.exist_sender = obj.exist_start_date = false;
  obj.start_date = 0;
  obj.end_date = ~0;
  obj.count = 0;
  char buf;
  while ((buf = getchar()) == '-' || buf == ' '){
    if (buf == ' ')
      continue;
    buf = getchar();
    if (buf == 'f'){
      obj.exist_sender = true;
      int count = -1;
      getchar();
      while ((buf = getchar()) != '\"')
        obj.sender[++count] = buf;
      obj.sender[++count] = '\0';
    }
    else if (buf == 't'){
      obj.exist_receiver = true;
      int count = -1;
      getchar();
      while ((buf = getchar()) != '\"')
        obj.receiver[++count] = buf;
      obj.receiver[++count] = '\0';
    }
    else if (buf == 'd'){
      buf = getchar();
      char date[16];
      date[12] = '\0';
      if (buf != '~'){
        obj.exist_start_date = true;
        date[0] = buf;
        for (int i = 1; i != 12; ++i)
          date[i] = getchar();
        obj.start_date = Date(date);
        getchar();
      }
      if ((buf = getchar()) != ' '){
        obj.exist_end_date = true;
        date[0] = buf;
        for (int i = 1; i != 12; ++i)
          date[i] = getchar();
        obj.end_date = Date(date);
        getchar();
      }
    }
  }
  string word;
  while (buf != '\n'){
    if (!isalnum(buf)){
      if (!word.empty()){
        obj.expression[obj.count] = word;
        ++obj.count;
        word.clear();
      }
      obj.expression[obj.count] = buf;
      ++obj.count;
    }
    else
      word.push_back(tolower(buf));
    buf = getchar();
  }
  if (!word.empty()){
    obj.expression[obj.count] = Expression(word);
    ++obj.count;
  }
}
