#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <unordered_map>
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
void MailManager::add(string &file_path){
  ifstream fin;
  fin.open(file_path);
  Mail *mail = new Mail;
  char keyword[16], buf;
  // From
  fin >> keyword >> mail->sender;

  // Date
  unsigned year, date, hour, minute;
  string month(10,0);
  fin >> keyword >> date >> month >> year >> keyword >> hour >> buf >> minute;
  mail->date = Date(year, monthTransform[month], date, hour, minute);
  
  // ID
  fin >> keyword >> mail->id;
  if (id2mail.find(mail->id) != id2mail.end()){
    fin.close();
    delete mail;
    cout << '-' << endl;
    return;
  }
  // Subject
  string buffer;
  fin >> keyword;
  getline(fin, buffer);
  stringstream sin(buffer);
  string word;
  while (sin >> buf){
    if (!isalnum(buf)){
      if (!word.empty()){
        mail->content.insert(word);
        word.clear();
      }
    }
    else
      word.push_back(buf);
  }
  if (!word.empty()){
    mail->content.insert(word);
    word.clear();
  }
  
  // To
  fin >> keyword >> mail->receiver;
  
  // Content
  fin >> keyword;
  while (fin >> buf){
    if (!isalnum(buf)){
      if (!word.empty()){
        mail->content.insert(word);
        word.clear();
      }
    }
    else{
      ++mail->length;
      word.push_back(buf);
    }
  }
  if (!word.empty()){
    mail->content.insert(word);
    word.clear();
  }
  fin.close();

  // Add to Some Data Structure
  id2mail[mail->id] = mail;
  if (receiver2id.find(mail->receiver) == receiver2id.end())
    receiver2id[mail->receiver] = set<int>();
  receiver2id[mail->receiver].insert(mail->id);
  if (sender2id.find(mail->sender) == sender2id.end())
    sender2id[mail->sender] = set<int>();
  sender2id[mail->sender].insert(mail->id);
  length_set.insert(Mail_length(mail));
  date_set.insert(Mail_date(mail));
  ++amount;
  cout << amount << endl;
}

void MailManager::remove(int id){
  if (id2mail.find(id) == id2mail.end()){
    cout << '-' << endl;
    return;
  }
  Mail *mail = id2mail[id];
  id2mail.erase(id);
  set<int> &r_set = receiver2id[mail->receiver];
  r_set.erase(mail->id);
  if (r_set.empty())
    receiver2id.erase(mail->receiver);
  set<int> &s_set = sender2id[mail->sender];
  s_set.erase(mail->id);
  if (s_set.empty())
    sender2id.erase(mail->sender);
  length_set.erase(Mail_length(mail));
  date_set.erase(Mail_date(mail));
  delete mail;
  --amount;
  cout << amount << endl;
}

void MailManager::longest(){
  if (amount == 0)
    cout << '-' << endl;
  else
    cout << length_set.begin()->mail->id << ' ' << length_set.begin()->mail->length << endl;
}

void MailManager::query(Query q){
  vector<Mail *>mails;
  unsigned filter = 0;
  if (q.exist_sender)
    ++filter;
  if (q.exist_receiver)
    ++filter;
  if (q.exist_start_date || q.exist_end_date)
    ++filter;
  if (q.exist_sender){
    for (auto id : sender2id[q.sender]){
      Mail *mail = id2mail[id];
      if (mail->query_id != query_id){
        mail->query_id = query_id;
        mail->poke = 0;
      }
      ++mail->poke;
      if (mail->poke == filter)
        mails.push_back(mail);
    }
  }
  if (q.exist_receiver){
    for (auto id : receiver2id[q.receiver]){
      Mail *mail = id2mail[id];
      if (mail->query_id != query_id){
        mail->query_id = query_id;
        mail->poke = 0;
      }
      ++mail->poke;
      if (mail->poke == filter)
        mails.push_back(mail);
    }
  }
  if (q.exist_start_date || q.exist_end_date){
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
    date_set.erase(Mail_date(mail_beg));
    date_set.erase(Mail_date(mail_end));
    delete mail_beg;
    delete mail_end;
  }
  /* Expression */

  ++query_id;
}
