#include "doSyscall.h"
#include <iostream>
#include <cstdio>
#include <stdio.h>
#include "Memory.h"
#include "syscall.h"
#include <sys/time.h>
using namespace std;

extern timeval tTime;
//#define DEBUG
void doSyscall(vector<unsigned int>& reg, Memory& memory, int& sstack){
	char input;
	int input2;
	int source = 0;
	switch (reg[17]){

		case SYS_write:
			printf("%s", &(memory.memory[reg[11]]));
//			cout << "###########" << endl;
//			printf("%s", &(memory.memory[reg[11]]));
//			cout << "###########" << endl;

			break;
		
		case SYS_read:
			cout << "SYS_read" << endl;
			cin >> input;
/*
#ifdef DEBUG
			cout << "reg:" << reg[11] << endl;
			memory.memory[reg[11]] = input;
			cout << "############" << endl;
			printf("%c", memory.memory[0x24048]);
			printf("%c", memory.memory[reg[11]]);
			cout << "############" << endl;
#endif
*/
			/*
			unsigned char temp1 = (unsigned char)input;
			unsigned char temp2 = (unsigned char)(input >> 8);
			unsigned char temp3 = (unsigned char)(input >> 16);
			unsigned char temp4 = (unsigned char)(input >> 24);
			memory.memory[reg[11]] = temp1;
			memory.memory[reg[11] + 1] = temp2;
			memory.memory[reg[11] + 2] = temp3;
			memory.memory[reg[11] + 3] = temp4;
			*/
			break;
		case SYS_gettimeofday:
			gettimeofday(&tTime, NULL);
			source = tTime.tv_usec;
			memory.memoryWrite(0x1df58, (unsigned char*)&source, 4);
			break;

		case SYS_exit:
		cout << "eeeeexxxxxxiiiiiitttttt" << endl;
			sstack = 0;
			break;
	}
	return;
}