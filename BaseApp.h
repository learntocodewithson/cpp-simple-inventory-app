#ifndef BASEAPP_H
#define BASEAPP_H

// ANSI escape codes for text color
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_ORANGE  "\x1b[38;5;214m" // ANSI escape code for approximate orange color
#define ANSI_COLOR_RESET   "\x1b[0m"        // Reset to default color

#include "Base.h"
#include "BaseProduct.h"
#include "BaseDailyLogInventory.h"

class BaseApp: public Base {
 // default as private access
 std::string title;
 int choice;
 bool exit_choice, back_to_main_menu;

 // set as public access
 public:
 
 // constructor
 BaseApp(std::string t = "") : title(t) {}
 /* same to this snippet
  BaseApp(std::string t = ""){
   title = t;
  }
 */

 // void methods
 void call(){

  do {
   system("clear");

   displayHeader();
   getChoiceInput();
   
   switch (choice){
   case 1:
    goToDailyLogInventoryMenu();
    break;
   case 2:
    goToProductsMenu();
    break;
   default:
    exit_choice = 1;
    std::cout << "\nThank your for using this app.";
    break;
   }

  } while(!exit_choice);
  
 }

 void displayHeader(){
  std::cout << displayTitle();
  std::cout << displayMenu();
 }

 void getChoiceInput(){
  std::cout << "\tEnter your choice: ";
  std::cin >> choice;
 }

 void goToDailyLogInventoryMenu(){
  BaseDailyLogInventory base_daily_log_inventory;
  
  do {
   system("clear");

   std::cout << displayTitle();
   std::cout << base_daily_log_inventory.displayMenu();

   getChoiceInput();
  
   switch (choice){
   case 1:
    //
    break;
   case 2:
    //
    break;
   default:
    back_to_main_menu = 1;
    break;
   }

  } while(!back_to_main_menu);

  if(back_to_main_menu){
   call();
  }
 }

 void goToProductsMenu(){
  BaseProduct base_product;

  do {
   system("clear");

   std::cout << displayTitle();
   std::cout << base_product.displayMenu();
   if(base_product.hasNotice()){
    std::cout << "\t" << base_product.displayNotice() << "\n\n";
   }
   getChoiceInput();
   
   switch (choice){
   case 1:
    base_product.addRecord();
    break;
   case 2:
    base_product.editRecord();
    break;
   case 3:
    base_product.deleteRecord();
    break;
   case 4:
    base_product.searchProduct();
    break;
   default:
    back_to_main_menu = 1;
    break;
   }

  } while(!back_to_main_menu);

  if(back_to_main_menu){
   call();
  }
  
 }

 // with return type methods
 std::string displayLine(){
  return "========================";
 }

 std::string displayTitle(){
  // "\n\t" to std::string("\n\t") is replaced to fix the issue about 
  // invalid operands to binary expression ('const char[3]' and 'const char[12]')
  return std::string("\n\t") + ANSI_COLOR_ORANGE + displayLine() + "\n\t" + title + "\n\t" + displayLine() + "\n" + ANSI_COLOR_RESET;
 }

 std::string displayMenu(){
  // + concatenation is used to fix ANSI_COLOR_BLUE + std::string("\tMain Menu\n") 
  return ANSI_COLOR_CYAN + std::string("\tMain Menu\n") +
   "\t1. Daily Log Inventory\n"
   "\t2. Products\n"
   "\t3. Exit\n\n" + ANSI_COLOR_RESET;
 }
};
#endif
