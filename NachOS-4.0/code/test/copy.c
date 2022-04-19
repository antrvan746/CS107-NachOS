#include "syscall.h"
#define MaxLength 32

int main() {
    int src;
    int dst;

    int fileSize;
    int i;
    char c;

    char source[MaxLength];
    char dest[MaxLength];

    PrintString("\n-- Sao chep file----\n");
    PrintString("- Nhap ten file nguong: ");
    ReadString(source, MaxLength); // Nhap vao ten file nguon

    PrintString("- Nhap ten file dich: ");
    ReadString(dest, MaxLength); // Nhap vao ten file dich

    src = Open(source, 1);

    if (src != -1) {
        dst = Create(dest);
        Close(dst);

        dst = Open(dest, 0);
        if (dst != -1) {
            fileSize = Seek(-1, src);

            Seek(0, src);
            Seek(0, dst);

            i = 0;
            for (i = 0; i < fileSize; i++) {
                Read(&c, 1, src);
                Write(&c, 1, dst);

            }

            PrintString("Copy thanh cong \n");
            Close(dst);
        }
        else {
            PrintString("Tao file dich khong thanh cong\n");
        }

        Close(src);
    }
    else {
        PrintString("Loi khi mo file\n");
    }

    Halt();
}