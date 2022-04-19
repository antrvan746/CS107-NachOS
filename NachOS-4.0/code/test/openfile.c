#include "syscall.h"

int main()
{
    char *filename;
    char *str;
    int type;
    int file_id;
    filename="fileydam.txt";
    type = 0;
    // file_id=2;
    
    file_id = Open(filename, type);
    if(file_id != -1){
        PrintString("Mo file thanh cong nhe!\n");
    }
    else{
        PrintString("Mo file that bai nhe!\n");
    }
    if (file_id < 0) PrintChar('-');
    PrintNum(file_id);
    PrintChar('\n');

    if(Close(file_id) != -1){
        PrintString("Dong File thanh cong\n");
    }
    else{
        PrintString("Dong file khong thanh cong\n");
    }
    PrintChar('\n');
    Halt();
}