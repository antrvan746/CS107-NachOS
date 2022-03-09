#include "syscall.h"

int main() {
    int num;
    num = ReadNum();
    PrintNum(num);

    Halt();
}