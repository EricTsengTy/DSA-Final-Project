# DSA-Final-Project

## Structure of Program
```
main.cpp
|-- common.hpp
|   |-- common.cpp
|
|-- data_structure.hpp
    |-- data_structure1.cpp
    |-- data_structure2.cpp
    |-- data_structure3.cpp
```

## Main Class
```
class MailManager {
public:
  //need to provide a default constructor and 4 member functions
  MailManager();
  void add(std::string path_to_file);
  void remove(int id);
  void longest();
  void query(Query q);
};
```
