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

#include <regex>
#ifdef _WIN32
// Windows-specific includes and code
#include <windows.h>
#else
// Unix-like OS specific includes and code
#include <termios.h>
#include <unistd.h>
#include <cstdio>
#endif

#include "Base.h"
#include "BaseProduct.h"
#include "BaseDailyLogInventory.h"

class BaseApp: public Base {
 // default as private access
 std::string title;
 int choice;
 bool exit_choice, back_to_main_menu, access_granted;

 // set as public access
 public:
 
 // constructor
 BaseApp(std::string t = "") : title(t), access_granted(false) {}
 /* same to this snippet
  BaseApp(std::string t = ""){
   title = t;
  }
 */

 // void methods
 void call(){
  std::string master_password;

  while(!access_granted) {
    system("clear");

    std::cout << displayTitle();
    
    if(hasNotice())
      std::cout << "\t" << displayNotice() << "\n";
    
    master_password = scanMasterPassword();

    if(master_password == getEnv("master_password")){
      access_granted = true;
    } else {
      setErrorNotice("Invalid Master Password");
    }
  }

  if(access_granted)
   mainProgram();
 }

 void mainProgram(){
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
   
   if(base_daily_log_inventory.hasNotice())
    std::cout << "\t" << base_daily_log_inventory.displayNotice() << "\n\n";
   
   getChoiceInput();
  
   switch (choice){
   case 1:
      base_daily_log_inventory.addRecord();
    break;
   case 2:
      base_daily_log_inventory.editRecord();
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

   if(base_product.hasNotice())
    std::cout << "\t" << base_product.displayNotice() << "\n\n";
   
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

 std::string getEnv(std::string key){
  std::string row_data, tmp_key, tmp_value;
  bool keyFound = false;

  std::ifstream EnvironmentalVariables(".env");

  if(EnvironmentalVariables.is_open()){
    while(getline(EnvironmentalVariables, row_data)){
      std::istringstream scanner(row_data);
      getline(scanner, tmp_key, ':');
      getline(scanner, tmp_value);
      if(tmp_key == key){
        keyFound = true;
        break;
      }
    }
  }

  EnvironmentalVariables.close();

  if(!keyFound)
    tmp_value = "";

  removeExtraSpaces(tmp_value);

  return tmp_value;
 }

 void removeExtraSpaces(std::string& str) {
  // Replace consecutive spaces with a single space
  std::regex regex("\\s+");
  str = std::regex_replace(str, regex, " ");

  // Remove leading and trailing spaces
  str = std::regex_replace(str, std::regex("^\\s+|\\s+$"), "");
 }
 
 #ifdef _WIN32
  std::string scanMasterPassword(){
   std::string password;
   
   HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
   DWORD mode;
   GetConsoleMode(hStdin, &mode);
   SetConsoleMode(hStdin, mode & (~ENABLE_ECHO_INPUT));

   std::cout << "\tEnter Master Password: ";
   std::cin >> password;

   SetConsoleMode(hStdin, mode); // Restore console mode

   return password;
  }
 #else
  std::string scanMasterPassword(){
   std::string password;
   
   echoOff();
   std::cout << "\tEnter Master Password: ";
   std::cin >> password;
   echoOn();

   return password;
  }
 #endif

 // Function to turn off echoing
  void echoOff() {
    termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    tty.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &tty);
  }

  // Function to turn on echoing
  void echoOn() {
    termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    tty.c_lflag |= ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &tty);
  }


};
#endif
