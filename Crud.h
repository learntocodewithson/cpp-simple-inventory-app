#ifndef CRUD_H
#define CRUD_H
class Crud {
 public:
 virtual void addRecord() = 0;
 virtual void editRecord() = 0;
 virtual void deleteRecord() = 0;
 virtual void saveRecord() = 0;
 virtual void updateRecord() = 0;
};
#endif