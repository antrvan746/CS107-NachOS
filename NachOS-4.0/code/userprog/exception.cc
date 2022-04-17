// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "main.h"
#include "syscall.h"
#include "ksyscall.h"

#define MaxFileLength 32

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// If you are handling a system call, don't forget to increment the pc
// before returning. (Or else you'll loop making the same system call forever!)
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	is in machine.h.
//----------------------------------------------------------------------


// Increase PC
void IncreasePC() {
    /* set previous program counter (debugging only)*/
    kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

    /* set program counter to next instruction (all Instructions are 4 byte wide)*/
    kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);

    /* set next program counter for brach execution */
    kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg) + 4);
}



void
ExceptionHandler(ExceptionType which)
{
    int type = kernel->machine->ReadRegister(2);

    DEBUG(dbgSys, "Received Exception " << which << " type: " << type << "\n");

    switch (which) {
		case NoException:
			return;
		case PageFaultException:
			DEBUG('a', "\n No valid translation found");
			printf("\n No valid translation found");
			SysHalt();
			break;
		case ReadOnlyException:
			DEBUG('a', "\n Write attempted to page marked \"read-only\"");
			printf("\n Write attempted to page marked \"read-only\"");
			SysHalt();
			break;
		case BusErrorException:
			DEBUG('a', "\n Translation resulted in an invalid physical address");
			printf("\n Translation resulted in an invalid physical address");
			SysHalt();
			break;
		case AddressErrorException:
			DEBUG('a', "\n Unaligned reference or one that was beyond the end of the address space");
			printf("\n Integer overflow in add or sub.");
			SysHalt();
			break;
		case OverflowException:
			DEBUG('a', "\n Integer overflow in add or sub.");
			printf("\n Integer overflow in add or sub.");
			SysHalt();
			break;
		case IllegalInstrException:
			DEBUG('a', "\n No valid translation found");
			printf("\n\n No valid translation found");
			SysHalt();
			break;
		case NumExceptionTypes:
			DEBUG('a', "\n NumException found");
			printf("\n\n NumException found");
			SysHalt();
			break;
		case SyscallException:
      		switch(type) {
      			case SC_Halt:
					DEBUG(dbgSys, "Shutdown, initiated by user program.\n");

					SysHalt();

					ASSERTNOTREACHED();
					break;

      			case SC_Add:
					DEBUG(dbgSys, "Add " << kernel->machine->ReadRegister(4) << " + " << kernel->machine->ReadRegister(5) << "\n");
					/* Process SysAdd Systemcall*/
					int result;
					result = SysAdd(/* int op1 */(int)kernel->machine->ReadRegister(4),
					/* int op2 */(int)kernel->machine->ReadRegister(5));
					DEBUG(dbgSys, "Add returning with " << result << "\n");
					/* Prepare Result */
					kernel->machine->WriteRegister(2, (int)result);
					/* Modify return point */
					{
	  				/* set previous programm counter (debugging only)*/
	  				kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));
	  				/* set programm counter to next instruction (all Instructions are 4 byte wide)*/
	  				kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
	  				/* set next programm counter for brach execution */
	  				kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
					}
					return;
					ASSERTNOTREACHED();
					break;
				
				case SC_ReadNum: {
					int num;
					num = SysReadNum();
					kernel->machine->WriteRegister(2, (int)num);

					IncreasePC();
					return;
					
					ASSERTNOTREACHED();
            		break;
				}
				case SC_PrintNum: {
					int num2;
					num2 = (int)kernel->machine->ReadRegister(4);
					SysPrintNum(num2);

					IncreasePC();
					return;
					
					ASSERTNOTREACHED();
            		break;
				}
				case SC_RandomNum: {
					kernel->machine->WriteRegister(2, SysRandomNum());

					IncreasePC();
					return;
					ASSERTNOTREACHED();
					break;
				}
				case SC_ReadChar: {
					char ch;
					ch = SysReadChar();
					kernel->machine->WriteRegister(2, ch);

					IncreasePC();
					return;

					ASSERTNOTREACHED();
            		break; 
				}
				case SC_PrintChar: {
					char ch2;
					ch2 = (char)kernel->machine->ReadRegister(4);
					SysPrintChar(ch2);

					IncreasePC();
					return;

					ASSERTNOTREACHED();
            		break;
				}

				case SC_ReadString: {
					int virAddr = kernel->machine->ReadRegister(4);
					int len = kernel->machine->ReadRegister(5);
					char* buffer = User2System(virAddr, len);
					SysReadString(buffer, len);
					System2User(virAddr, len, buffer);
					delete buffer;

					IncreasePC();
					return;

					ASSERTNOTREACHED();
            		break;
				}

				case SC_PrintString: {
					int virtualAddr2 = kernel->machine->ReadRegister(4); // get address of buffer
            		char *buffer2 = User2System(virtualAddr2, 255);       // copy string (max 255 byte) from User space to Kernel space
            		SysPrintString(buffer2);                             // print string
            		delete[] buffer2;

            		IncreasePC();
            		return;

            		ASSERTNOTREACHED();
            		break;
				}

				// Xu ly Syscall CreateFile tao file
				case SC_Create: {

					// Tao ra file voi tham so la ten file
					// Doc dia chi ten file tu thanh ghi r4 
					int virtAddr = kernel->machine->ReadRegister(4);

					// Chuyen dia chi ten file tu UserSpace sang SystemSpace
					char* filename = User2System(virtAddr, MaxFileLength + 1);
					SysCreateFile(filename);
					delete[] filename;

					IncreasePC();
					return;
					// Tao file thanh cong

				}

				case SC_Open: {
					
					int virtAddr = kernel->machine->ReadRegister(4);
					int type = kernel->machine->ReadRegister(5);
				
					char* filename = User2System(virtAddr, MaxFileLength + 1);
					SysOpen(filename, type);
					// Mo file, tra ve id neu thanh cong, tra ve -1 neu that bai
					delete[] filename;
					IncreasePC();
					return;
					
				}

				case SC_Close: {
					OpenFileID fid = kernel->machine->ReadRegister(4);
					int res = SysClose(fid);
					kernel->machine->WriteRegister(2, res);
					IncreasePC();
					return;
					
				}
				case SC_Read: {
					int virtAddr = kernel->machine->ReadRegister(4);
					int size = kernel->machine->ReadRegister(5);
					OpenFileID id = kernel->machine->ReadRegister(6);

					char* buffer = new char[size];

					SysRead(virtAddr, buffer, size, id);

					delete buffer;
					IncreasePC();
					return;

				}

				case SC_Write: {
					// Doc cac tham so dau vao
					int virtAddr = kernel->machine->ReadRegister(4);
					int size = kernel->machine->ReadRegister(5);
					int id = kernel->machine->ReadRegister(6);

					// Lay du lieu
					char* buffer = new char[size];

					// Ghi vao file
					SysWrite(virtAddr, buffer, size, id);

					delete buffer;
					IncreasePC();
					return;
				}

				case SC_Seek: {
					int position = kernel->machine->ReadRegister(4);
					int id = kernel->machine->ReadRegister(5);

					// Tim den vi tri va nhan ket qua tra ve
					SysSeek(position, id);

					IncreasePC();
					return;

				}

				case SC_Remove: {
					int virtAddr = kernel->machine->ReadRegister(4);

					char* filename = User2System(virtAddr, MaxFileLength + 1);
					int res = SysRemove(filename);

					kernel->machine->WriteRegister(2, res);
					IncreasePC();
					return;					
				}

      			default:
					cerr << "Unexpected system call " << type << "\n";
					break;
    		}
      		break;
    	default:
      		cerr << "Unexpected user mode exception" << (int)which << "\n";
      		break;
    }
	ASSERTNOTREACHED()
}


