#include "syscall.h"

int main() {
    SysPrintString(SysReadString());

    Halt();
}