#include "syscall.h"

int main()
{
  int size;
  char result[255];
  int fInId,fOutId;
  char* buffer = "abcdefgh";

  if(Create("fileTest.txt")==0){
    PrintString("Tao file thanh cong nhe!\n");
  }
  else{
    PrintString("Tao file that bai r :<\n");
  }

  // Mo file de ghi buffer vao
  fInId = Open("fileTest.txt", 2);
  Write(buffer,5,fInId);
  Close(fInId);
  // Mo file de doc
  fOutId = Open("fileTest.txt", 3);
  // doi den vi tri 1
  Seek(1,fOutId);

  // Doc toi da 255 ky tu
  size = Read(result,255,fOutId);
  
  PrintString("So ky tu doc duoc: \n");
  PrintNum(size);
  PrintChar('\n');
  PrintString(result);
  Close(fOutId);
  
  Halt();
}