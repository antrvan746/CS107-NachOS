#include "syscall.h"
#define MaxLength 32

int main() {
    int src;
    int src2;

    int fileSize;
    int fileSize2;
    int i;
    char c;

    char source[MaxLength];
    char source2[MaxLength];

    PrintString("\n-- Noi file----\n");
    PrintString("- Nhap ten file nguong: ");
    ReadString(source, MaxLength); // Nhap vao ten file nguon

    PrintString("- Nhap ten file dich: ");
    ReadString(source2, MaxLength); // Nhap vao ten file dich

    src = Open(source, 0);
    src2 = Open(source2, 1);

    if (src != -1) {
        if (src2 != -1) {
            fileSize = Seek(-1, src);
            fileSize2 = Seek(-1, src2);

            Seek(fileSize, src);
            Seek(0, src2);

            i = 0;
            for (i = 0; i < fileSize; i++) {
                Read(&c, 1, src2);
                Write(&c, 1, src);
            }

            PrintString("Noi file thanh cong \n");
            Close(src2);
        }
        PrintString("Loi khi mo file nguon 2\n");
        Close(src);
    }
    else {
        PrintString("Loi khi mo file nguon 1\n");
    }

    Halt();
}