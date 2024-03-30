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

  int countRecord(){
    int products_count = 0;

    std::string row_data;

    std::ifstream ReadProducts("products.csv");

    while(getline(ReadProducts, row_data)) { products_count++; }

    ReadProducts.close();

    return products_count;
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

  void deleteRecord() {
    std::string deleteAnswer;
    
    searchProduct();

    if(hasNotice()){
      std::cout << "\t" << displayNotice() << "\n\n";

      std::cout << "Are you sure you want to delete[yes or no]? ";
      std::cin >> deleteAnswer;

      if(deleteAnswer == "Yes" || deleteAnswer == "yes"){
        // perform delete
        deleteProduct();

        setSuccessNotice("\n\tProduct has been successfull deleted.");
      }else{
         setErrorNotice("\n\tProduct deletion has been cancelled.");
      }

    }
  }

  void editRecord() {
    // productid: temporary variable to avoid overriding current searched product id
    std::string productId, productName, productCategory;

    searchProduct();
    
   if(hasNotice()){
    std::cout << "\t" << displayNotice() << "\n\n";

    std::cout << "\tEditing " << product_id;
    
    std::cout << "\n\tUpdate Product ID(" << product_id <<"): ";
    std::cin >> productId;

    std::cout << "\tUpdate Product Name(" << product_name <<"): ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    getline(std::cin, productName);

    std::cout << "\tUpdate Product Category(" << product_category <<"): ";
    getline(std::cin, productCategory);

    updateRecord(productId, productName, productCategory);
   }

  }
  
  void saveRecord(){
   std::ofstream Products("products.csv", std::ios::app);
   if(Products.is_open()){
    Products << product_id << "," << product_name << "," << product_category << std::endl;
    Products.close();
   }
   setSuccessNotice(product_name + " has been successfully added.");
  }

  void deleteProduct(){
    int products_count, counter;
    products_count = countRecord();
    counter = 0;

    std::string row_data, products[products_count], tempProductId;
    
    // read the file
    std::ifstream ReadProducts("products.csv");
    while(getline(ReadProducts, row_data)){
      std::istringstream scanner(row_data);
      getline(scanner, tempProductId, ',');
      if(tempProductId != product_id){
         products[counter] = row_data;

         counter++;
      }
    }
    ReadProducts.close();

    // create new file and load the new list
    std::ofstream WriteProducts("products.csv");

    if(WriteProducts.is_open()){
      for(int i = 0; i < products_count; i++){
        if(products[i] != "")
          WriteProducts << products[i] << "\n";
      }

      WriteProducts.close();
    }else{
       setErrorNotice("\nUnable to create and open a file");
    }
  }

  void updateRecord(std::string productId, std::string productName, std::string productCategory) {
    int products_count, counter;
    products_count = countRecord();
    counter = 0;

    std::string row_data, products[products_count], tempProductId, tempProductName, tempProductCategory, formattedProductData;
    
  
    // Reading all records and store to string array

    std::ifstream ReadProducts("products.csv");

    while(getline(ReadProducts, row_data)){

      std::istringstream scanner(row_data);

      getline(scanner, tempProductId, ',');
      getline(scanner, tempProductName, ',');
      getline(scanner, tempProductCategory);

     if(tempProductId == product_id){
      formattedProductData = "";

      if(productId != ""){
        formattedProductData += productId + ',';
      }else{
        formattedProductData += product_id + ',';
      }

      if(productName != ""){
        formattedProductData += productName;
      }else{
        formattedProductData += tempProductName;
      }

      if(productCategory != ""){
        formattedProductData += ',' + productCategory;
      }else if(tempProductCategory != ""){
        formattedProductData += ',' + tempProductCategory;
      }

      products[counter] = formattedProductData;
     }else {
      products[counter] = row_data;
     }

     counter++;
    }

    ReadProducts.close();

    // create a new csv file and store new list of products

    std::ofstream WriteProducts("products.csv");

    if(WriteProducts.is_open()){
      for(int i = 0; i < products_count; i++){
        WriteProducts << products[i] << "\n";
      }

      WriteProducts.close();
    }else{
       setErrorNotice("\nUnable to create and open a file");
    }

  }

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