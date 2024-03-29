#ifndef BASEDAILYLOGINVENTORY_H
#define BASEDAILYLOGINVENTORY_H

class BaseDailyLogInventory: public Base, public Crud {
 public:
 std::string displayMenu() {
  // + concatenation is used to fix ANSI_COLOR_BLUE + std::string("\tMain Menu\n") 
  return ANSI_COLOR_CYAN + std::string("\tMenu\n") +
   "\t1. Log New Inventory\n"
   "\t2. Edit Inventory\n"
   "\t3. Delete Inventory\n"
   "\t4. Back To Main Menu\n\n" + ANSI_COLOR_RESET;
 }

 void addRecord() {}
 void editRecord() {}
 void deleteRecord() {}
 void saveRecord() {}
 int countRecord() {
  return 0;
 }
};
#endif