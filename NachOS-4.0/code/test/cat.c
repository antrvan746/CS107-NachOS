#include "syscall.h"
#define MaxLength 32


int main() {
    int openFileId;
    int fileSize;
    char c; // Ky tu de in ra
    char filename[MaxLength];
    int i; // Bien long

    PrintString("----Hien thi noi dung file ----\n");
    PrintString(" - Nhap ten file can doc: ");

    ReadString(filename, MaxLength);

    openFileId = Open(filename, 1); // Goi ham Open de mo file
    if (openFileId != -1) {
        fileSize = Seek(-1, openFileId);
        // Seek den cuoi file de tim duoc do dai noi dung file

        i = 0;
        Seek(0, openFileId);

        PrintString("Noi dung file la: \n");
        for (i = 0; i < fileSize; i++) { // Chay vong lap tu 0 -> fileSize
            Read(&c, 1, openFileId);
            PrintChar(c); // Goi ham PrintChar de in tung ki tu ra man hinh
        }

        Close(openFileId); // Goi ham Close de dong file
    }

    else {
        PrintString("Mo file khong thanh cong!!\n\n"); 
    }

    Halt();
}