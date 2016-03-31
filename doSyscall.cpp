#include "doSyscall.h"
#include <iostream>
#include <cstdio>
#include <stdio.h>
#include "Memory.h"
#include "syscall.h"
#include <sys/time.h>
using namespace std;

timeval sTime;
timeval eTime;
int s = 0;
long int timedif;
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
			if (s == 0){
				gettimeofday(&sTime, NULL);
				/*
				cout << "start" << endl;
				cout << sTime.tv_sec << endl;
				*/
				s = 1;
			}
			else {
				gettimeofday(&eTime, NULL);
//				cout << eTime.tv_sec << endl;
				timedif = eTime.tv_sec - sTime.tv_sec;
//				cout << timedif << endl;
			}
			/*
			gettimeofday(&tTime, NULL);
			source = tTime.tv_sec;
			memory.memoryWrite(0x1df58, (unsigned char*)&source, 4);
			*/
			break;

		case SYS_exit:
			printf("Microseconds for one run through Dhrystone: %f\n", timedif*1000000.0/50000);
			printf("Dhrystones per Second:                      %f\n", 50000.0/(timedif));
			cout << "exit" << endl;
			sstack = 0;
			break;
	}
	return;
}