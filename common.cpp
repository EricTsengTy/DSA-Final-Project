#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "common.hpp"
using namespace std;

/* Fast I/O */
void fastscan(int &number) { 
    //variable to indicate sign of input number 
    bool negative = false; 
    register int c; 
  
    number = 0; 
  
    // extract current character from buffer 
    c = getchar(); 
    while (c == ' ')
      c = getchar();
  
    // Keep on extracting characters if they are integers 
    // i.e ASCII Value lies from '0'(48) to '9' (57) 
    for (; (c>47 && c<58); c=getchar()) 
        number = number *10 + c - 48; 
  
    // if scanned input has a negative sign, negate the 
    // value of the input number 
    if (negative) 
        number *= -1; 
} 

/*** Date ***/
/* Initialization */
Date::Date(unsigned &year, unsigned &month, unsigned &date, unsigned &hour, unsigned &minute){
  value = _value_calc(year, month, date, hour, minute);
}
/* Look For a Good Solution */
Date::Date(char *str){
  /* YYYYMMDDHHMM */
  unsigned year, month, date, hour, minute;
  sscanf(str, "%4u%2u%2u%2u%2u", &year, &month, &date, &hour, &minute);
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
bool Date::operator<=(const Date &obj){
  return value <= obj.value;
}
bool Date::operator>=(const Date &obj){
  return value >= obj.value;
}
unsigned Date::_value_calc(unsigned &year, unsigned &month, unsigned &date, unsigned &hour, unsigned &minute){
  return minute | (hour << 6) | (date << 11) | (month << 16) | (year << 20);
}

/*** Query ***/
/* stdin */
std::istream &read(std::istream &is, Query &obj){
  obj = Query();
  char *buf = obj.expression;
  is >> obj.expression;
  while (buf[0] == '-'){
    if (buf[1] == 'f'){
      strcpy(obj.sender, buf + 3);
      obj.sender[strlen(obj.sender) - 1] = '\0';
      obj.exist_sender = true;
    }
    else if (buf[1] == 't'){
      strcpy(obj.receiver, buf + 3);
      obj.receiver[strlen(obj.receiver) - 1] = '\0';
      obj.exist_receiver = true;
    }
    else if (buf[1] == 'd'){
      char *parser = buf + 2;
      parser = strtok(parser, "~");
      if (parser == buf + 2){
        obj.start_date = Date(parser);
        parser = strtok(nullptr, "~");
	    obj.exist_start_date = true;
      }
      if (parser != nullptr){
        obj.end_date = Date(parser);
	    obj.exist_end_date = true;
      }
    }
    is >> obj.expression;
  }
  return is;
}

/* Another read option */
void read(Query &obj){
  obj = Query();
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
  int count = 0;
  obj.expression[count] = buf;
  while ((buf = getchar()) != '\n' && buf != 13)
    obj.expression[++count] = buf;
  obj.expression[++count] = '\0';
}

std::istream &operator>>(std::istream &is, Query &obj){
  return read(is, obj);
}
