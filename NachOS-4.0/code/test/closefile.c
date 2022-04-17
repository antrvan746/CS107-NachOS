#include "syscall.h"

int main()
{
    char *filename;
    
    filename = "fileydam.txt";
    if(Close(filename) != -1){
        PrintString("Dong file thanh cong!\n");
    }
    else {
        PrintString("Khong the dong file!\n");
    }
    Halt();
}