#include "Memory.h"

void Memory::memoryClear(){
	memory.clear();
};
	
void Memory::memoryWrite(unsigned int addr, unsigned char* source, unsigned int bytes){
	for (unsigned int i = 0; i < bytes; i++){
		memory[addr+i] = source[i];
	}
};

void Memory::memoryRead( unsigned int addr, unsigned char* target, unsigned int bytes){
	for (unsigned int i = 0; i < bytes; i++){
		if(memory.find(addr+i) != memory.end()){
			target[i] = memory[addr+i];
		} else {
			target[i] = 0;
			break;
		}
	}
};

void Memory::instructionRead( unsigned int addr, unsigned int* target, unsigned int num){
	unsigned int instruction = 0;
	unsigned int onebyte;
	for(unsigned int i = 0; i < num * 4; i++){
		if(memory.find(addr+i) != memory.end()){
			onebyte = (unsigned int)memory[addr+i];
		} else {
			onebyte = 0;
			break;
		}

		if(i % 4 != 3){
			instruction = instruction | (onebyte << (i%4 * 8));
		} else {
			target[i/4] = instruction;
			instruction = 0;
			onebyte = 0;
		}
	}
};