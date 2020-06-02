#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "common.hpp"
using namespace std;
/*** Date ***/
/* Initialization */
Date::Date(unsigned year, unsigned month, unsigned date, unsigned hour, unsigned minute){
  value = _value_calc(year, month, date, hour, minute);
}
/* Look For a Good Solution */
Date::Date(char *str){
  /* YYYYMMDDHHMM */
  str += 10;
  unsigned minute = atoi(str);
  *str = '\0';
  str -= 2;
  unsigned hour = atoi(str);
  *str = '\0';
  str -= 2;
  unsigned date = atoi(str);
  *str = '\0';
  str -= 2;
  unsigned month = atoi(str);
  *str = '\0';
  str -= 4;
  unsigned year = atoi(str);
  value = _value_calc(year, month, date, hour, minute);
}
/* Comparator */
bool Date::operator<(const Date &obj){
  return value < obj.value;
}
bool Date::operator==(const Date &obj){
  return value == obj.value;
}
bool Date::operator>(const Date &obj){
  return value > obj.value;
}
unsigned Date::_value_calc(unsigned year, unsigned month, unsigned date, unsigned hour, unsigned minute){
  return minute | (hour << 6) | (date << 11) | (month << 16) | (year << 20);
}
/* stdout */
std::ostream &write(std::ostream &os, const Date &obj){
  os << obj.value;
  return os;
}
std::ostream &operator<<(std::ostream &os, const Date &obj){
  return write(os, obj);
}

/*** Query ***/
/* stdin */
std::istream &read(std::istream &is, Query &obj){
  char *buf = obj.expression;
  is >> obj.expression;
  while (buf[0] == '-'){
    if (buf[1] == 'f'){
      strcpy(obj.sender, buf + 3);
      obj.sender[strlen(obj.sender) - 1] = '\0';
    }
    else if (buf[1] == 't'){
      strcpy(obj.receiver, buf + 3);
      obj.receiver[strlen(obj.receiver) - 1] = '\0';
    }
    else if (buf[1] == 'd'){
      char *parser = buf + 2;
      parser = strtok(parser, "~");
      if (parser == buf + 2){
        obj.start_date = Date(parser);
        parser = strtok(nullptr, "~");
      }
      if (parser != nullptr)
        obj.end_date = Date(parser);
    }
    is >> obj.expression;
  }
  return is;
}
std::istream &operator>>(std::istream &is, Query &obj){
  return read(is, obj);
}
