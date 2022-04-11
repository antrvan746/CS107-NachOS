/**************************************************************
 *
 * userprog/ksyscall.h
 *
 * Kernel interface for systemcalls 
 *
 * by Marcus Voelp  (c) Universitaet Karlsruhe
 *
 **************************************************************/

#ifndef __USERPROG_KSYSCALL_H__ 
#define __USERPROG_KSYSCALL_H__ 

#include "kernel.h"
#include "machine.h"
#include "synchconsole.h"
#include "filesys.h"


void SysHalt()
{
  kernel->interrupt->Halt();
}


int SysAdd(int op1, int op2)
{
  return op1 + op2;
}

/// Part 1
int SysReadNum() {
    long long num = 0;
    char buffer;
    bool isNegative = false;
    bool isInteger = true;


    do {
        buffer = kernel->synchConsoleIn->GetChar();
    } while (buffer == ' ' || buffer == '\n');

    if (buffer == '-') {
      isNegative = true;
      buffer = kernel->synchConsoleIn->GetChar();
      if (buffer == '\n' || buffer == ' ')
        return 0;
    }

    if (!(buffer >= '0' && buffer <= '9')) {
      isInteger = false;
    }

    do {
      num = num * 10 + (buffer - '0');
      buffer = kernel->synchConsoleIn->GetChar();

      if (!(buffer >= '0' && buffer <= '9') && buffer != '\n' && buffer != ' ') {
        isInteger = false;
      }
    } while (buffer != '\n' && buffer != ' ');


    if (!isInteger) return 0;
    if (isNegative) num = -num;

    if (num > INT32_MAX || num < INT32_MIN) return 0;

    return num;
    
}

void SysPrintNum(int num) {
    char arr[10];
    int i = 0;
    int tmp, j;

    if (num == 0) {
      kernel->synchConsoleOut->PutChar('0');
      return;
    }

    if (num < 0) {
      num = -num;
    }

    while (num != 0) {
      tmp = num % 10;
      arr[i] = tmp;
      i++;
      num /= 10;
    }

    for (j = i - 1; j >= 0; j--) {
      kernel->synchConsoleOut->PutChar('0' + arr[j]);
    }

}

char SysReadChar() {
    char buffer;
    buffer = kernel->synchConsoleIn->GetChar();
    return buffer;
}

void SysPrintChar(char c) {
    kernel->synchConsoleOut->PutChar(c);
}

void SysReadString(char* buffer, int len) {
    int i = 0;
    char ch;
    if (len <= 0) return;
    
    for (i = 0; i < len; ++i) {
      buffer[i] = 0;
    }
    for (i = 0; i < len;) {
      do {
      ch = kernel->synchConsoleIn->GetChar();
      } while (ch == EOF);
      
      if (ch == '\001' || ch == '\n') break;
      buffer[i++] = ch; 
    }
}
void SysPrintString(char* buffer) {
    int len = 0;
    while (buffer[len] != '\0')
    {
      kernel->synchConsoleOut->PutChar(buffer[len]);
      len++;
    } 
}

unsigned int SysRandomNum() {
  RandomInit(time(0));
  return RandomNumber();
}


/// Part 2


// 1. Syscall Create file

void SysCreateFile(char* filename) {

  // Truong hop ten file khong hop le
  if (strlen(filename) <= 0) {
    printf("Invalid file name!\n");
    DEBUG('a', "Invalid file name!\n");

    // Tra ve gia tri -1 vao thanh ghi R2
    kernel->machine->WriteRegister(2, -1);
    return;
  }

  if (filename == NULL) {
    printf("Not enough memory in system\n");
    DEBUG('a', "Not enough memory in system\n");

    // Tra ve gia tri -1 vao thanh ghi R2
    kernel->machine->WriteRegister(2, -1);
    return;
  }

  
  // Tao file bang ham Create cua fileSystem, tra ve ket qua 
  if (!kernel->fileSystem->Create(filename, 0)) {
    // Tao file that bai

    printf("Error create file '%s'", filename);
    kernel->machine->WriteRegister(2, -1);
    return;
  }

  // Tao file thanh cong
  kernel->machine->WriteRegister(2, 0);
  return;
}


// 2.1 Syscall Open file
OpenFileId SysOpen(char* filename, int type) {
  int freeSlot;

  OpenFile *file;
  freeSlot = kernel->fileSystem->FindFreeSlot();
  if (type == INPUT_TYPE) {
    kernel->machine->WriteRegister(2, 0);
  } 
  else if (type == OUTPUT_TYPE) {
    kernel->machine->WriteRegister(2, 1);
  }
  else if (freeSlot == -1) {
    file = kernel->fileSystem->Open(filename, type);

    if (file != NULL) {
      kernel->fileSystem->openf[freeSlot] = file;
      return freeSlot;
    }
  } else {
    return -1;    
  }  
} 

// 2.2 Syscall Close file
int SysClose(OpenFileId id) {
    if (id >= 0 && id <= 14) {
      if (kernel->fileSystem->openf[id]) {
        delete kernel->fileSystem->openf[id];
        kernel->fileSystem->openf[id] = NULL;
        return 0;
      }
    }
    return -1;
}


int SysRead(char* buffer, int charcount, OpenFileID id) {
   int OldPos;
   int NewPos;

   if (id < 0 || id > 14) {
     printf("\nKhong the doc vi id nam ngoai bang mo ta file.");
     kernel->machine->WriteRegister(2, -1);
     IncreasePC();
     return;
   }
   if (kernel->fileSystem->openf[id] == NULL) {
      printf("\nKhong the doc vi file nay khong can ton tai.");
      return -1;
   }
   if (kernel->fileSystem->openf[id]->type == 3) {
      printf("\nKhong the doc file stdout.");
      return -1;
   }
   OldPos = kernel->fileSystem->openf[id]->GetCurrentPos();
   if (kernel->fileSystem->openf[id]->type == 2) {
      int size = kernel->synchConsoleIn->Read(buffer, charcount);
      return size;
   }
   if ((kernel->fileSystem->openf[id]->Read(buffer, charcount)) > 0) {
      NewPos = kernel->fileSystem->openf[id]->GetCurrentPos();
      return NewPos - OldPos;
   }
   else {
      return -2;
   }
}

int SysWrite(char* buffer, int charcount, OpenFileID id) {

}

int SysSeek(int pos, OpenFileID id) {

}

int SysRemove(char* filename) {

}



#endif /* ! __USERPROG_KSYSCALL_H__ */
