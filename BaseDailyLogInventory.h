#ifndef BASEDAILYLOGINVENTORY_H
#define BASEDAILYLOGINVENTORY_H

#include <chrono>
#include <ctime>
#include <vector>

#ifdef _WIN32
// Windows-specific includes and code
#include <windows.h>
#else
// Unix-like OS specific includes and code
#include <dirent.h>
#include <sys/stat.h>
#endif

class BaseDailyLogInventory: public Base, public Crud {
 std::string currentDate;
 public:
 std::string displayMenu() {
  // + concatenation is used to fix ANSI_COLOR_BLUE + std::string("\tMain Menu\n") 
  return ANSI_COLOR_CYAN + std::string("\tMenu\n") +
   "\t1. Log New Inventory\n"
   "\t2. Edit Inventory\n"
   "\t3. Back To Main Menu\n\n" + ANSI_COLOR_RESET;
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

 void displayBatchNumbers(std::string date){
  std::string row_data, strBatchNumber, tempBatchNumber;

  std::ifstream DailyLogInventory("daily-log-inventory/" + date);

   if(DailyLogInventory.is_open()){
    while(getline(DailyLogInventory, row_data)){
      std::istringstream scanner(row_data);
      getline(scanner, strBatchNumber, ',');
      if(strBatchNumber != tempBatchNumber && strBatchNumber != ""){
        std::cout << "\tBatch " << strBatchNumber << "\n";
        tempBatchNumber = strBatchNumber;
      }
    }
   }else{
    setErrorNotice("\nUnable to open the file with " + date + " filename.");
   }
    
   DailyLogInventory.close();
 }

 bool batchNumberExists(std::string date, int batch_number){
  std::string row_data, strBatchNumber;
  bool batch_number_exists = false;
  std::ifstream DailyLogInventory("daily-log-inventory/" + date);
  if(DailyLogInventory.is_open()){
    while(getline(DailyLogInventory, row_data)){
      std::istringstream scanner(row_data);
      getline(scanner, strBatchNumber, ',');
      if(strBatchNumber != "" && stoi(strBatchNumber) == batch_number){
        batch_number_exists = true;
        break;
      }
    }
  }

  return batch_number_exists;
 }

 void fetchAndUpdateBatchDailyLogs(std::string date, int batch_number){
  std::string row_data, strBatchNumber, strProductId, strProductName, strQuantity, strDate, strUpdatedLog, productQuantity;
  std::vector<std::string> updatedLogs;
  std::ifstream DailyLogInventory("daily-log-inventory/" + date);

  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  if(DailyLogInventory.is_open()){
    while(getline(DailyLogInventory, row_data)){
      std::istringstream scanner(row_data);
      getline(scanner, strBatchNumber, ',');
      if(strBatchNumber != "" && stoi(strBatchNumber) == batch_number){
        getline(scanner, strProductId, ',');
        getline(scanner, strProductName, ',');
        getline(scanner, strQuantity, ',');
        getline(scanner, strDate);

        std::cout << "\n\t" << strProductName << "[" << strProductId <<"](" << strQuantity <<")";
        std::cout << "\n\tEnter new quantity: ";
       
        getline(std::cin, productQuantity);
       
        
        if(productQuantity != "")
          strQuantity = productQuantity;

        strUpdatedLog = std::to_string(batch_number) + "," + strProductId + "," + strProductName + "," + strQuantity + "," + strDate;
        updatedLogs.push_back(strUpdatedLog);

        // std::cout << "\n" + strUpdatedLog;
        
      }else{
        updatedLogs.push_back(row_data);
      }
    }

  }else{
    setErrorNotice("\nUnable to open the file with " + date + " filename.");
  }

  DailyLogInventory.close();

  // create new file and load the new list
  std::ofstream WriteDailyLogInventory("daily-log-inventory/" + date);

  if(WriteDailyLogInventory.is_open()){
    for (size_t i = 0; i < updatedLogs.size(); ++i){
      if(updatedLogs[i] != "")
        WriteDailyLogInventory << updatedLogs[i] << "\n";
    }

    WriteDailyLogInventory.close();
  }else{
      setErrorNotice("\nUnable to create and open a file");
  }
 }



#ifdef _WIN32
  void listFilesInFolder(const std::wstring& folderPath) {
    WIN32_FIND_DATAW findData;
    HANDLE hFind = FindFirstFileW((folderPath + L"\\*").c_str(), &findData);
    if (hFind == INVALID_HANDLE_VALUE) {
        std::cerr << "Error opening directory " << folderPath << std::endl;
        return;
    }

    do {
        if ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
            std::wcout << findData.cFileName << std::endl;
        } else if (wcscmp(findData.cFileName, L".") != 0 && wcscmp(findData.cFileName, L"..") != 0) {
            // Recursively call listFilesInFolder for subdirectories
            listFilesInFolder(folderPath + L"\\" + findData.cFileName);
        }
    } while (FindNextFileW(hFind, &findData) != 0);

    FindClose(hFind);
  }
#else
  void listFilesInFolder(const std::string& folderPath) {
    DIR* dir = opendir(folderPath.c_str());
    if (dir == nullptr) {
        std::cerr << "Error opening directory " << folderPath << std::endl;
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (entry->d_type == DT_REG) {
            std::cout << "\t" << entry->d_name << std::endl;
        } else if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            // Recursively call listFilesInFolder for subdirectories
            listFilesInFolder(folderPath + "/" + entry->d_name);
        }
    }

    closedir(dir);
  }
#endif




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

 void editRecord() {
  std::string date;
  int batch_number;

  std::cout << "\n\tList of all Dates from Daily Logs:\n";
  listFilesInFolder("daily-log-inventory");

  do {
    std::cout << "\n\tDaily Log Inventory Date: ";
    std::cin >> date;

    resetNotice();

    displayBatchNumbers(date);
    
    if(hasNotice()){
      std::cout << ANSI_COLOR_RED << "\tInvalid date" << ANSI_COLOR_RESET << "\n";
    }

  } while(hasNotice());

  
  do {
    std::cout << "\n\tEnter Batch Number: ";

    if (!(std::cin >> batch_number)) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } else {
      fetchAndUpdateBatchDailyLogs(date, batch_number);
    }

    if(!batchNumberExists(date, batch_number)){
      std::cout << ANSI_COLOR_RED << "\tInvalid batch number" << ANSI_COLOR_RESET << "\n";
    }

  } while(!batchNumberExists(date, batch_number));
 }
 void deleteRecord() {}
 void saveRecord() {}
 int countRecord() {
  return 0;
 }
};
#endif