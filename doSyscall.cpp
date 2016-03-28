#include "doSyscall.h"
#include <iostream>
#include <cstdio>
using namespace std;

void doSyscall(const vector<unsigned int>& reg, Memory& memory){
	switch (reg[17]){
		case 64:
			printf("%c", memory.memory[reg[11]]);
			break;
	}
}