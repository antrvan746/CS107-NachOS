#include "syscall.h"

int main()
{
  int size;
  char result[255];
  int fInId,fOutId;
  char* buffer = "abcdefgh";

  if( Create("fileTest.txt") == 0) {
    PrintString("Tao file thanh cong nhe!\n");
  }
  else{
    PrintString("Tao file that bai r :<\n");
  }

  // Mo file de ghi buffer vao
  fInId = Open("fileTest.txt", READ_WRITE);
  PrintNum(fInId);
  PrintChar('\n');
  Write(buffer, 5, fInId);
  Close(fInId);
  // Mo file de doc
  fOutId = Open("fileTest.txt", READ_ONLY);
  PrintNum(fOutId);
  PrintChar('\n');
  // doi den vi tri 1
  Seek(1,fOutId);

  // Doc toi da 255 ky tu
  size = Read(result,255,fOutId);
  
  PrintString("\nSo ky tu doc duoc: ");
  PrintNum(size);
  PrintChar('\n');
  PrintString(result);
  Close(fOutId);
  
  Halt();
}