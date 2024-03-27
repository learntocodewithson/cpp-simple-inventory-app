#ifndef BASE_H
#define BASE_H
class Base {
 std::string message;
 
 public:
  virtual std::string displayMenu() = 0;

  std::string displayNotice(){
   return message;
  }

  bool hasNotice(){
   return message != "";
  }

  void setNotice(std::string m){
   message = m;
  }
};
#endif