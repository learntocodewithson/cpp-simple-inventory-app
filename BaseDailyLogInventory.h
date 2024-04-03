#ifndef BASEDAILYLOGINVENTORY_H
#define BASEDAILYLOGINVENTORY_H

#include <chrono>
#include <ctime>

class BaseDailyLogInventory: public Base, public Crud {
 std::string currentDate;
 public:
 std::string displayMenu() {
  // + concatenation is used to fix ANSI_COLOR_BLUE + std::string("\tMain Menu\n") 
  return ANSI_COLOR_CYAN + std::string("\tMenu\n") +
   "\t1. Log New Inventory\n"
   "\t2. Edit Inventory\n"
   "\t3. Delete Inventory\n"
   "\t4. Back To Main Menu\n\n" + ANSI_COLOR_RESET;
 }

 std::string getCurrentDate(bool timestamp = false) {
  // get the raw current date
  std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
  
  // convert raw to time = 120382302
  std::time_t time_now = std::chrono::system_clock::to_time_t(now);

  // convert to local time
  std::tm* time_info = std::localtime(&time_now);
  
  char buffer[20]; // Buffer to hold the formatted date
  if(timestamp){
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", time_info);
  }else{
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d", time_info);
  }
  
  return std::string(buffer);
 }

 std::string generateInventoryId(){
  std::string prefix = "KPIN";
  std::string inventoryId = prefix;

  const char alphanum[] =
    "0123456789"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  
  srand(time(0));

  for (int i = 0; i < 5; ++i)
    inventoryId += alphanum[rand() % (sizeof(alphanum) - 1)];
  
  return inventoryId;
 }

 int generateBatchNumber(std::string date){
  int batchNumber = 1;
  std::string row_data, strBatchNumber, tempBatchNumber;

  std::ifstream DailyLogInventory("daily-log-inventory/" + date);
  
  if(DailyLogInventory.is_open()){
    while(getline(DailyLogInventory, row_data)){
      std::istringstream scanner(row_data);
      getline(scanner, strBatchNumber, ',');
      if(strBatchNumber != "")
        tempBatchNumber = strBatchNumber;
    }
  }

  DailyLogInventory.close();

  if(tempBatchNumber != "")
    batchNumber = std::stoi(tempBatchNumber) + 1;

  return batchNumber;
 }

 void addRecord() {
  currentDate = getCurrentDate();
  std::cout << "\n\tAdding New Log Inventory for " << currentDate;
  
  std::string row_data;
  
  int batch_number = generateBatchNumber(currentDate);
 
  std::ifstream Products("products.csv");

  while(getline(Products, row_data)){
   std::istringstream scanner(row_data);

   std::string productid, productname;
   int quantity;

   getline(scanner, productid, ',');
   getline(scanner, productname, ',');

   std::cout << "\n\tProduct " << productname << "[" << productid << "]";
   std::cout << "\n\tEnter Quantity: ";
   std::cin >> quantity;

   saveToDailyLog(batch_number, productid, productname, quantity);
  }
  Products.close();
 }

 void saveToDailyLog(int batch_number, std::string productid, std::string productname, int quantity){
  std::string timestamp = getCurrentDate(true);
  std::string daily_log_inventory_title = "daily-log-inventory/" + currentDate; 
  std::ofstream DailyLogInventory(daily_log_inventory_title, std::ios::app);
  if(DailyLogInventory.is_open()){
   DailyLogInventory << std::to_string(batch_number) << "," << productid << "," << productname << "," << std::to_string(quantity) << "," << timestamp << std::endl;
   DailyLogInventory.close(); 
  }
 }

 void editRecord() {}
 void deleteRecord() {}
 void saveRecord() {}
 int countRecord() {
  return 0;
 }
};
#endif