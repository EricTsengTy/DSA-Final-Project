#ifndef COMMON_HPP
#define COMMON_HPP
using namespace std;
/* Fast I/O */
void fastscan(int &number);

class String{
public:
  friend std::istream &read(std::istream &is, String &obj);
  String() = default;
  String(const string s){
    for (const auto &i : s)
      push_back(i);
    _size = s.size();
  }
  String(const char *s){
    strcpy(str, s);
    _size = strlen(str);
  }
  bool operator<(const String &obj) const{
    return strcmp(str, obj.str) < 0;
  }
  bool operator>(const String &obj) const{
    return strcmp(str, obj.str) > 0;
  }
  bool operator==(const String &obj) const{
    return strcmp(str, obj.str) == 0;
  }
  bool operator<=(const String &obj) const{
    return strcmp(str, obj.str) <= 0;
  }
  bool operator>=(const String &obj) const{
    return strcmp(str, obj.str) >= 0;
  }
  void copy(char *s){
    strcpy(str, s);
    _size = strlen(s);
  }
  size_t size() const{
    return _size;
  }
  bool empty(){
    return _size == 0;
  }
  void clear(){
    _size = 0;
  }
  void push_back(char c){
    str[_size] = c;
    str[++_size] = '\0';
  }
  char str[64];
private:
  size_t _size = 0;
};

std::istream &read(std::istream &is, String &obj);
std::istream &operator>>(std::istream &is, String &obj);


/* If year < 4096 then the value could be store by unsigned */
class Date{
public:
  friend std::ostream &write(std::ostream &os, const Date &obj);
  Date() = default;
  Date(unsigned v):value(v){};
  Date(int v):value(v){};
  Date(unsigned &year, unsigned &month, unsigned &date, unsigned &hour, unsigned &minute);
  Date(char *str);
  void set_value(unsigned &year, unsigned &month, unsigned &date, unsigned &hour, unsigned &minute){
    value = _value_calc(year, month, date, hour, minute);
  }
  bool operator<(const Date &obj);
  bool operator==(const Date &obj);
  bool operator>(const Date &obj);
  bool operator<=(const Date &obj);
  bool operator>=(const Date &obj);
private:
  unsigned _value_calc(unsigned &year, unsigned &month, unsigned &date, unsigned &hour, unsigned &minute);
  unsigned value;
};

class Query{
public:
  friend std::istream &read(std::istream &is, Query &obj);
  String receiver;
  String sender;
  Date start_date = 0;  // default min
  Date end_date = ~0;   // default max
  String expression; // Maybe would change to array or something
  bool exist_receiver = false, exist_sender = false,
    exist_start_date = false, exist_end_date = false;
};
std::istream &read(std::istream &is, Query &obj);
std::istream &operator>>(std::istream &is, Query &obj);

#endif
