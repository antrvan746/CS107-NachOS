#include "syscall.h"


int main()
{
  char *filename = "fileydam.txt";
  
  if(Create(filename) == 0){
    PrintString("Tao file thanh cong nhe!\n");
  }
  else{
    PrintString("Tao file that bai r :<\n");
  }
  Halt();
}