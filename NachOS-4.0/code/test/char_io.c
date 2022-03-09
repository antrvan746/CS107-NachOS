#include "syscall.h"

int main() {
    char ch = ReadChar();
    PrintChar(ch);

    Halt();
}