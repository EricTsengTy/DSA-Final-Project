#ifndef COMMON_HPP
#define COMMON_HPP
#include <iostream>
using namespace std;
/* If year < 4096 then the value could be store by unsigned */
void str2lower(char *str);
void str2lower(string &str);

class Date{
public:
  friend std::ostream &write(std::ostream &os, const Date &obj);
  Date() = default;
  Date(unsigned v):value(v){};
  Date(int v):value(v){};
  Date(unsigned year, unsigned month, unsigned date, unsigned hour, unsigned minute);
  Date(char *str);
  bool operator<(const Date &obj);
  bool operator==(const Date &obj);
  bool operator>(const Date &obj);
  bool operator<=(const Date &obj);
  bool operator>=(const Date &obj);
private:
  unsigned _value_calc(unsigned year, unsigned month, unsigned date, unsigned hour, unsigned minute);
  unsigned value;
};
std::ostream &write(std::ostream &os, const Date &obj);
std::ostream &operator<<(std::ostream &os, const Date &obj);

class Query{
public:
  friend std::istream &read(std::istream &is, Query &obj);
  char receiver[64] = "";
  char sender[64] = "";
  Date start_date = 0;  // default min
  Date end_date = ~0;   // default max
  char expression[512] = ""; // Maybe not string
  bool exist_receiver = false, exist_sender = false,
    exist_start_date = false, exist_end_date = false;
};
std::istream &read(std::istream &is, Query &obj);
std::istream &operator>>(std::istream &is, Query &obj);


#endif
