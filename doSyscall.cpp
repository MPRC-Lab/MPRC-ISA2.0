#include "doSyscall.h"
#include <iostream>
#include <cstdio>
#include "Memory.h"
using namespace std;

void doSyscall(vector<unsigned int>& reg, Memory& memory){
	switch (reg[17]){
		case 64:
			printf("%c", memory.memory[reg[11]]);
			break;
	}
}