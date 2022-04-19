#include "syscall.h"


int main()
{
  char *filename = "filetam2.txt";
  
  if(Create(filename) == 0){
    PrintString("Tao file thanh cong nhe!\n");
  }
  else{
    PrintString("Tao file that bai r :<\n");
  }

  if (Remove(filename) == 1) {
    PrintString("Xoa file thanh cong!\n"); 
  }
  else {
    PrintString("Xoa file that bai!!\n");
  }
  Halt();
}