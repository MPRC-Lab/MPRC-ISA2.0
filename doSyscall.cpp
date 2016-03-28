#include "doSyscall.h"
#include <iostream>
#include <cstdio>
#include "Memory.h"
#include "syscall.h"
using namespace std;

void doSyscall(vector<unsigned int>& reg, Memory& memory){
	switch (reg[17]){
		case SYS_write:
			cout << reg[12] << endl;
			printf("%c", memory.memory[reg[11]]);
			cout << "##########" << endl;
			break;
		/*
		case SYS_fstat:

		case SYS_brk:

		case SYS_close:

		case 
		*/
	}
}