#ifndef COMMON_HPP
#define COMMON_HPP
using namespace std;
/* Fast I/O */
void fastscan(int &number);

/* If year < 4096 then the value could be store by unsigned */
class Date{
public:
  friend std::ostream &write(std::ostream &os, const Date &obj);
  Date() = default;
  Date(unsigned v):value(v){};
  Date(int v):value(v){};
  Date(unsigned &year, unsigned &month, unsigned &date, unsigned &hour, unsigned &minute);
  Date(char *str);
  bool operator<(const Date &obj);
  bool operator==(const Date &obj);
  bool operator>(const Date &obj);
  bool operator<=(const Date &obj);
  bool operator>=(const Date &obj);
  unsigned value;
private:
  unsigned _value_calc(unsigned &year, unsigned &month, unsigned &date, unsigned &hour, unsigned &minute);
};

class Query{
public:
  friend std::istream &read(std::istream &is, Query &obj);
  char receiver[64];
  char sender[64];
  Date start_date = 0;  // default min
  Date end_date = ~0;   // default max
  char expression[512]; // Maybe not string
  bool exist_receiver = false, exist_sender = false,
    exist_start_date = false, exist_end_date = false;
};
std::istream &read(std::istream &is, Query &obj);
std::istream &operator>>(std::istream &is, Query &obj);
void read(Query &obj);

#endif
