#include "syscall.h"

int main() {
    SysPrintChar(SysReadChar());

    Halt();
}