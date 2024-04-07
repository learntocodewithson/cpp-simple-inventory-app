#ifndef BASE_H
#define BASE_H
class Base {
 std::string message;

 void setNotice(std::string m, int status = 200){
  if(status == 200){
    message = ANSI_COLOR_GREEN + m + ANSI_COLOR_RESET;
  }else{
    message = ANSI_COLOR_RED + m + ANSI_COLOR_RESET;
  }
 }
 
 public:
  virtual std::string displayMenu() = 0;

  std::string displayNotice(){
   return message;
  }

  bool hasNotice(){
   return message != "";
  }

  void setSuccessNotice(std::string m){
    setNotice(m);
  }

  void setErrorNotice(std::string m){
    setNotice(m, 500);
  }

  void resetNotice(){
    message = "";
  }
};
#endif