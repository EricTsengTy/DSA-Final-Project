# DSA-Final-Project

## Structure of Program
```
main.cpp
|-- common.hpp
|   |-- common.cpp
|
|-- mail_manager.hpp
    |-- mail_manager.cpp
```

## Main Class
```
class MailManager {
public:
  //need to provide a default constructor and 4 member functions
  MailManager();
  void add(const std::string &path_to_file);
  void remove(const int id);
  void longest() const;
  void query(const Query &q);
};
```
