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


/*
Input: - User space address (int)
- Limit of buffer (int)
Output:- Buffer (char*)
Purpose: Copy buffer from User memory space to System memory space
*/


char* User2System(int virtAddr, int limit) {
	int i;
	int oneChar;
	char* kernelBuf = NULL;

	kernelBuf = new char[limit + 1]; // need for terminal string
	if (kernelBuf == NULL) {
		return kernelBuf;
	}

	memset(kernelBuf, 0, limit + 1);

	for (i = 0; i < limit; i++) {
		kernel->machine->ReadMem(virtAddr + i, 1, &oneChar);
		kernelBuf[i] = (char)oneChar;
		if (oneChar == 0) break;
	}

	return kernelBuf;
}


/*
Input: - User space address (int)
- Limit of buffer (int)
Output:- Buffer (char*)
Purpose: Copy buffer from System memory space to User memory space
*/

int System2User(int virtAddr, int len, char* buffer) {
	if (len < 0) return -1;
	if (len == 0) return len;
	int i = 0;
	int oneChar = 0;

	do {
		oneChar = (int) buffer[i];
		kernel->machine->WriteMem(virtAddr+i, 1, oneChar);
		i++;
	} while (i < len && oneChar != 0);

	return i;
	
}


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
    printf("Khong ton tai file!\n");
    DEBUG('a', "Khong ton tai file!\n");

    // Tra ve gia tri -1 vao thanh ghi R2
    kernel->machine->WriteRegister(2, -1);
    return;
  }

  if (filename == NULL) {
    printf("Khong du bo nho\n");
    DEBUG('a', "Khong du bo nho\n");

    // Tra ve gia tri -1 vao thanh ghi R2
    kernel->machine->WriteRegister(2, -1); // tao file that bai
    return;
  }

  
  // Tao file bang ham Create cua fileSystem, tra ve ket qua 
  if (!kernel->fileSystem->Create(filename)) {
    printf("Error create file '%s'", filename);
    kernel->machine->WriteRegister(2, -1);
    return;
  }

  // Tao file thanh cong
  kernel->machine->WriteRegister(2, 0);
  return;
}


// 2.1 Syscall Open file
void SysOpen(char* filename, int type) {
  int freeSlot;

  OpenFile *file;
  freeSlot = kernel->fileSystem->FindFreeSlot();
  
  if (type == INPUT_TYPE) {
    // input co nsole stdout => he thong se doc gia tri tren man hinh console
    kernel->machine->WriteRegister(2, 1);
    return;
  } 
  else if (type == OUTPUT_TYPE) {
    // input co nsole stdout => he thong se doc gia tri tren man hinh console
    kernel->machine->WriteRegister(2, 1);
    return;
  }
  else if (freeSlot == -1) {
    file = kernel->fileSystem->Open(filename, type);

    if (file != NULL) {
      kernel->fileSystem->openf[freeSlot] = file;
      kernel->machine->WriteRegister(2, freeSlot); // mo file thanh cong
    }
  } else {
    kernel->machine->WriteRegister(2, -1);    
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


void SysRead(int virtAddr, char*& buffer, int charcount, OpenFileID id) {
   int curPos;
   int newPos;

    // Kiem tra id co nam trong bang mo ta file
   if (id < 0 || id > MAX_FILE_OPEN - 1) {
     printf("Khong the doc vi id nam ngoai bang mo ta file.\n");
     kernel->machine->WriteRegister(2, -1);
     return;
   }

   // Kiem tra file co ton tai hay khong
   if (kernel->fileSystem->openf[id] == NULL) {
      printf("\nKhong the doc vi file nay khong can ton tai.\n");
      kernel->machine->WriteRegister(2, -1);
      return;
   }

   // Kiem tra truong hop doc file stdout
   if (id == INDEX_STDOUT) {
      printf("Khong the doc file stdout.\n");
      kernel->machine->WriteRegister(2, -1);
      return;
   }

   curPos = kernel->fileSystem->openf[id]->GetCurrentPos();
   if (id == INDEX_STDIN) {
      int size = kernel->synchConsoleIn->Read(buffer, charcount);
      System2User(virtAddr, size, buffer);
      kernel->machine->WriteRegister(2, size);
      return;
   }
   if ((kernel->fileSystem->openf[id]->Read(buffer, charcount)) > 0) {
      newPos = kernel->fileSystem->openf[id]->GetCurrentPos();
      kernel->machine->WriteRegister(2, newPos - curPos);
      System2User(virtAddr, newPos - curPos, buffer);
   }
   else {
      kernel->machine->WriteRegister(2, -2);
      return;
   }
}

void SysWrite(int virtAddr, char*& buffer, int charcount, OpenFileID id) {
  int curPos;
  int newPos;

  if (id < 0 || id > MAX_FILE_OPEN - 1) {
    printf("Khong the ghi vi id nam ngoai bang mo ta file.\n");
    kernel->machine->WriteRegister(2, -1);
    return;
  }

  if (kernel->fileSystem->openf[id] == NULL) {
    printf("Khong the ghi vi file nay khong ton tai.\n");
    kernel->machine->WriteRegister(2, -1);
		return;
  }

  if (kernel->fileSystem->openf[id]->type == READONLY_TYPE || kernel->fileSystem->openf[id]->type == INDEX_STDIN) {
    printf("Khong the write file stdin hoac file only read.\n");
    kernel->machine->WriteRegister(2, -1);
		return;
  }

  curPos = kernel->fileSystem->openf[id]->GetCurrentPos();
  buffer = User2System(virtAddr, charcount);

  if (kernel->fileSystem->openf[id]->type == READWRITE_TYPE) {
    if (kernel->fileSystem->openf[id]->Write(buffer, charcount) > 0) {
        newPos = kernel->fileSystem->openf[id]->GetCurrentPos();
        kernel->machine->WriteRegister(2, newPos - curPos);
        return;
    }
  }
  if (id == INDEX_STDOUT) {
    int pos = 0;
    while (buffer[pos] != '\0') {
      kernel->synchConsoleOut->PutChar((buffer + pos)[0]);
      pos++;
    }
    kernel->machine->WriteRegister(2, pos -1);
    return;
  }
}

void SysSeek(int pos, OpenFileID id) {
  if (id < 0 || id > MAX_FILE_OPEN - 1) {
    printf("Khong the seek vi id nam ngoai bang mo ta file.\n");
    kernel->machine->WriteRegister(2, -1);
    return;
    
  }
  if (kernel->fileSystem->openf[id] == NULL) {
    printf("Khong the seek vi file nay khong ton tai.\n");
    kernel->machine->WriteRegister(2, -1);
    return;
  }
  if (id == 0 || id == 1) {
    printf("Khong the seek tren file console.\n");
    kernel->machine->WriteRegister(2, -1);
    return;
  }
  pos = (pos == -1) ? kernel->fileSystem->openf[id]->Length() : pos;
  if (pos > kernel->fileSystem->openf[id]->Length() || pos < 0) {
    printf("Khong the seek den vi tri nay.\n");
    kernel->machine->WriteRegister(2, -1);
    return;
  }
  else {
    kernel->fileSystem->openf[id]->Seek(pos);
    kernel->machine->WriteRegister(2, pos);
    return;
  }
}

int SysRemove(char* filename) {
  int res;
  res = kernel->fileSystem->Remove(filename);
  return res;
}



#endif /* ! __USERPROG_KSYSCALL_H__ */
