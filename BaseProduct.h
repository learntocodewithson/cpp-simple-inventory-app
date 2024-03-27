#ifndef BASEPRODUCT_H
#define BASEPRODUCT_H
#include "Crud.h"

class BaseProduct: public Base, public Crud {
 std::string product_id, product_name, product_category;
 bool searchFound;

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

  std::string displayProducts(){
    std::string row_data, products;

    std::ifstream Products("products.csv");
    
    std::cout << "\n\tProducts";
    
    products = "\n\tProduct ID\tProduct Name\tProduct Category";

    while(getline(Products, row_data)){
      // row_data = KP01,Pizza Dough
      std::istringstream scanner(row_data);

      std::string productid, productname, productcategory;

      getline(scanner, productid, ',');
      getline(scanner, productname, ',');
      getline(scanner, productcategory);

      products += "\n\t" + productid + "\t\t" + productname + "\t\t" + productcategory;
    }

    Products.close();

    return products;
  }

  std::string formattedProductDetails(){
    return "\n\tSearch Found: \n\tProduct ID: " + product_id +
      "\n\tProduct Name: " + product_name +
      "\n\tProduct Category: " + product_category;
  }

  void addRecord() {
   std::cout << "Adding New Record";
   
   std::cout << "\nEnter Product ID: ";
   std::cin >> product_id;
   
   std::cout << "Enter Product Name: ";
   std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
   getline(std::cin, product_name);

   std::cout << "Enter Product Category: ";
   getline(std::cin, product_category);

   saveRecord();
  }
  void editRecord() {}
  void deleteRecord() {}

  void saveRecord(){
   std::ofstream Products("products.csv", std::ios::app);
   if(Products.is_open()){
    Products << product_id << "," << product_name << "," << product_category << std::endl;
    Products.close();
   }
   setSuccessNotice(product_name + " has been successfully added.");
  }

  void updateRecord() {}

  void searchProduct(){
    std::cout << displayProducts();

    std::cout << "\n\n\tSearch Product Id: ";
    std::cin >> product_id;

    searchRecord();

    if(searchFound){
      setSuccessNotice(formattedProductDetails());
    }else{
      setErrorNotice(product_id + " Not Found.");
    }
  }

  void searchRecord(){
    std::string row_data, products;

    searchFound = false;
    
    std::ifstream Products("products.csv");

    while(getline(Products, row_data)){
      // row_data = KP01,Pizza Dough
      std::istringstream scanner(row_data);

      std::string productid, productname, productcategory;

      getline(scanner, productid, ',');
      getline(scanner, productname, ',');
      getline(scanner, productcategory);

      // this code match the entered product id in the database 
      if(productid == product_id){
        product_id = productid;
        product_name = productname;
        product_category = productcategory;
        searchFound = true;
      }
    }
    
    Products.close();
  }
};
#endif