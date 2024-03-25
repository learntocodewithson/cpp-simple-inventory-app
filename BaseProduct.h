#ifndef BASEPRODUCT_H
#define BASEPRODUCT_H
#include "Crud.h"

class BaseProduct: public Base, public Crud {
 std::string product_id, product_name;

 public:
  std::string displayMenu() {
   // + concatenation is used to fix ANSI_COLOR_BLUE + std::string("\tMain Menu\n") 
   return ANSI_COLOR_CYAN + std::string("\tMenu\n") +
    "\t1. Add New Product\n"
    "\t2. Edit Product\n"
    "\t3. Delete Product\n"
    "\t4. Search Product\n"
    "\t5. Back To Main Menu\n\n" + ANSI_COLOR_RESET;
  }

  void addRecord() {
   std::cout << "Adding New Record";
   
   std::cout << "\nEnter Product ID: ";
   std::cin >> product_id;
   
   std::cout << "Enter Product Name: ";
   std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
   getline(std::cin, product_name);

   saveRecord();
  }
  void editRecord() {}
  void deleteRecord() {}

  void saveRecord(){
   std::cout << "Product Id: " << product_id << "\nProduct Name: " << product_name;
   std::getchar();
  }

  void updateRecord() {}
};
#endif