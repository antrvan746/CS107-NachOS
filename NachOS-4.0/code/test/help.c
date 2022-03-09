// Help Program introduce group and describe sort program and ascii program
#include"syscall.h"

int main(){
    // in thong tin nhom
    PrintString("=================================================================\n");
    PrintString("Group Member: \n");
    PrintString("20127435 - Tran Van An\n");
    PrintString("2012xxxx - Cai Minh Chanh\n");
    PrintString("2012xxxx - Nguyen Vinh Quang\n");
    PrintString("=================================================================\n");

// gioi thieu ve chuong trinh ascii va sort
    PrintString("command '-x ascii' : ascii program print the ascii character table.\n");
    PrintString("command '-x sort'  : sort program sort an integer array by using bubble sort.\n");
    Halt();
}