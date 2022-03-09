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
      num = num * 10 + buffer - '0';
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
    while (buffer[len]) {
      kernel->synchConsoleOut->PutChar(buffer[len++]);
    }
}


#endif /* ! __USERPROG_KSYSCALL_H__ */
