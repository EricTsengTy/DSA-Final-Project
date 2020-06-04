#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <unordered_map>
#include <deque>
#include "mail_manager.hpp"
using namespace std;
unordered_map<string,unsigned> monthTransform = {
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

void str2lower(char *str){
  for (int i = 0, len = strlen(str); i != len; ++i)
    str[i] = tolower(str[i]);
}

void str2lower(string &str){
  for (auto &i : str)
    i = tolower(i);
}

void MailManager::_add_data(Mail *&mail){
  if (receiver2id.find(mail->receiver) == receiver2id.end())
    receiver2id[mail->receiver] = unordered_set<int>();
  receiver2id[mail->receiver].insert(mail->id);
  if (sender2id.find(mail->sender) == sender2id.end())
    sender2id[mail->sender] = unordered_set<int>();
  sender2id[mail->sender].insert(mail->id);
  length_max_queue.push(Mail_length(mail));
  date_set.insert(Mail_date(mail));
  if (mail->length_remove)
    length_max_queue.push(Mail_length(mail));
}

void MailManager::add(string &file_path){
  ifstream fin;
  fin.open(file_path);
  Mail *mail = new Mail;
  char keyword[16], buf;
  // From
  fin >> keyword >> mail->sender;
  str2lower(mail->sender);

  // Date
  unsigned year, date, hour, minute;
  string month(10,0);
  fin >> keyword >> date >> month >> year >> keyword >> hour >> buf >> minute;
  mail->date = Date(year, monthTransform[month], date, hour, minute);
  
  // ID
  fin >> keyword >> mail->id;
  if (id2mail.find(mail->id) != id2mail.end()){
    Mail *past = id2mail[mail->id];
    if (past->remove){
      past->remove = false;
      _add_data(past);
      ++amount;
      cout << amount << endl;
    }
    else
      cout << '-' << endl;
    fin.close();
    delete mail;
    return;
  }
  // Subject
  string buffer;
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
  _add_data(mail);
  ++amount;
  cout << amount << endl;
}

void MailManager::remove(int id){
  auto p = id2mail.find(id);
  if (p == id2mail.end() || p->second->remove){
    cout << '-' << endl;
    return;
  }
  Mail *mail = id2mail[id];
  //id2mail.erase(id);
  mail->remove = true;
  unordered_set<int> &r_set = receiver2id[mail->receiver];
  r_set.erase(mail->id);
  if (r_set.empty())
    receiver2id.erase(mail->receiver);
  unordered_set<int> &s_set = sender2id[mail->sender];
  s_set.erase(mail->id);
  if (s_set.empty())
    sender2id.erase(mail->sender);
  date_set.erase(Mail_date(mail));
  //delete mail;
  --amount;
  cout << amount << endl;
}

void MailManager::longest(){
  if (amount == 0)
    cout << '-' << endl;
  else{
    while (length_max_queue.top().mail->remove){
      length_max_queue.top().mail->length_remove = true;
      length_max_queue.pop();
    }
    cout << length_max_queue.top().mail->id << ' ' << length_max_queue.top().mail->length << endl;
  }
}

void MailManager::_matching(set<unsigned>&ids, vector<Mail *>&mails, deque<Expression>&exp_pool){
  for (const auto &i : mails){
    vector<bool>match;
    for (const auto &j : exp_pool){
      if (j.operand)
        match.push_back((i->content.find(j.expression) != i->content.end()));
      else if (j.op == '!')
        match.back() = !match.back();
      else{
        if (j.op == '&')
          match[match.size() - 2] = match[match.size() - 2] && match.back();
        else if (j.op == '|')
          match[match.size() - 2] = match[match.size() - 2] || match.back();
        match.pop_back();
      }  
    }
    if (match.size() != 1)
      cout << "Error" << endl;
    if (match.back())
      ids.insert(i->id);
  }
}
void MailManager::_matching(set<unsigned>&ids, deque<Expression>&exp_pool){
  for (const auto &pairs : id2mail){
      Mail *i = pairs.second;
      if (i->remove)
        continue;
      vector<bool>match;
      for (const auto &j : exp_pool){
        if (j.operand)
          match.push_back((i->content.find(j.expression) != i->content.end()));
        else if (j.op == '!')
          match.back() = !match.back();
        else{
          if (j.op == '&')
            match[match.size() - 2] = match[match.size() - 2] && match.back();
          else if (j.op == '|')
            match[match.size() - 2] = match[match.size() - 2] || match.back();
          match.pop_back();
        }  
      }
      if (match.size() != 1)
        cout << "Error" << endl;
      if (match.back())
        ids.insert(i->id);
    }
}
/*
void MailManager::_matching(set<unsigned>&ids, vector<Mail *>&mails, deque<Expression>&exp_pool){
  for (const auto &i : mails){
    vector<ExpCalc>match;
    for (const auto &j : exp_pool){
      if (j.operand)
        match.push_back(ExpCalc(j.expression));
      else if (j.op == '!'){
        if (match.back().extract)
          match.back().value = !match.back().value;
        else
          match.back().negate = !match.back().negate;
      }
      else{
        ExpCalc &left = match[match.size() - 2];
        ExpCalc &right = match.back();
        if (j.op == '&'){
          if (left.extract && right.extract)
            left.value = left.value && right.value;
          else if (left.extract && left.value)
            left.value = (i->content.find(right.expression) != i->content.end()) ^ right.negate;
          else if (right.extract){
            if (right.value)
              left.value = (i->content.find(left.expression) != i->content.end()) ^ left.negate;
            else
              left.value = right.value;
          }
          else if (!left.extract && !right.extract){
            if ((i->content.find(left.expression) == i->content.end()) ^ left.negate)
              left.value = false;
            else
              left.value = (i->content.find(right.expression) != i->content.end()) ^ right.negate;
          }
          left.extract = true;
        }
        else if (j.op == '|'){
          if (left.extract && right.extract)
            left.value = left.value || right.value;
          else if (left.extract && !left.value)
            left.value = (i->content.find(right.expression) != i->content.end()) ^ right.negate;
          else if (right.extract){
            if (!right.value)
              left.value = (i->content.find(left.expression) != i->content.end()) ^ left.negate;
            else
              left.value = right.value;
          }
          else if (!left.extract && !right.extract){
            if ((i->content.find(left.expression) != i->content.end()) ^ left.negate)
              left.value = true;
            else
              left.value = (i->content.find(right.expression) != i->content.end()) ^ right.negate;
          }
          left.extract = true;
        }
        match.pop_back();
      }  
    }
    if (match.size() != 1)
      cout << "Error" << endl;
    if (!match.back().extract)
      match.back().value = (i->content.find(match.back().expression) != i->content.end()) ^ match.back().negate;
    if (match.back().value)
      ids.insert(i->id);
  }
}
*/

void MailManager::query(Query &q){
  vector<Mail *>mails;
  unsigned filter = (q.exist_sender ? 1 : 0)
                  + (q.exist_receiver ? 1 : 0)
                  + ((q.exist_start_date || q.exist_end_date) ? 1 : 0);
  bool date_check = (q.exist_start_date || q.exist_end_date);
  if (q.exist_sender){
    str2lower(q.sender);
    for (const auto &id : sender2id[q.sender]){
      Mail *mail = id2mail[id];
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
    for (const auto &id : receiver2id[q.receiver]){
      Mail *mail = id2mail[id];
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
    Mail *mail_beg = new Mail;
    Mail *mail_end = new Mail;
    mail_beg->date = q.start_date;
    mail_beg->id = 0;
    mail_end->date = q.end_date;
    mail_end->id = UINT32_MAX;
    date_set.insert(Mail_date(mail_beg));
    date_set.insert(Mail_date(mail_end));
    auto beg = date_set.find(Mail_date(mail_beg));
    ++beg;
    auto tail = date_set.find(Mail_date(mail_end));
    for (; beg != tail; ++beg){
      if (beg->mail->query_id != query_id){
        beg->mail->query_id = query_id;
        beg->mail->poke = 0;
      }
      ++beg->mail->poke;
      if (beg->mail->poke == filter)
        mails.push_back(beg->mail);
    }
    date_set.erase(mail_beg);
    date_set.erase(mail_end);
    delete mail_beg;
    delete mail_end;
  }
  /* Expression */
  deque<Expression>exp_pool, op_pool;
  char word[24];
  int count = 0;
  for (const auto &i : q.expression){
    if (i == '\0')
      break;
    if (!isalnum(i)){
      if (count != 0){
        word[count] = '\0';
        exp_pool.push_back(word);
        count = 0;
      }
      if (i == '(')
        op_pool.push_back(i);
      else{
        Expression tmp(i);
        while (!op_pool.empty() && (tmp.priority < op_pool.back().priority || (op_pool.back().op != '!' && op_pool.back().priority == tmp.priority))){
          exp_pool.push_back(op_pool.back());
          op_pool.pop_back();
        }
        if (!op_pool.empty() && i == ')')
          op_pool.pop_back();
        else
          op_pool.push_back(tmp);
      }
    }
    else{
      word[count] = tolower(i);
      ++count;
    }
  }
  if (count != 0){
    word[count] = '\0';
    exp_pool.push_back(word);
    count = 0;
  }
  while (!op_pool.empty()){
    exp_pool.push_back(op_pool.back());
    op_pool.pop_back();
  }
  /* Find ids that match expression */
  set<unsigned>ids;
  if (filter != 0)
    _matching(ids, mails, exp_pool);
  else
    _matching(ids, exp_pool);
  if (ids.empty())
    cout << '-' << endl;
  else{
    auto p = ids.begin();
    cout << *p;
    ++p;
    for (; p != ids.end(); ++p)
      cout << ' ' << *p;
    cout << endl;
  }
  ++query_id;
}
