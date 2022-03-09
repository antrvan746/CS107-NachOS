#include "syscall.h"

int main() {
    char buffer[100];
    int len = 100;
    ReadString(buffer, len);
    PrintString(buffer);

    Halt();
}