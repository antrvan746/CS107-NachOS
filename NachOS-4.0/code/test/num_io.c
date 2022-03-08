#include "syscall.h"

int main() {
    SysPrintNum(SysReadNum());

    Halt();
}