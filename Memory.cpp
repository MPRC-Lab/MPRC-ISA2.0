#include "Memory.h"

void Memory::memoryClear(){
	memory.clear();
};

/**
 * Write bytes to memory, default 1 bytes. Do: [addr] = source[0], [addr+1] = source[1]...
 * @param addr   : The addr in memory begin to write.
 * @param source : The data to be written into memory.
 * @param bytes  : How many bytes will be written.
 */
void Memory::memoryWrite(unsigned int addr, unsigned char* source, unsigned int bytes){
	for (unsigned int i = 0; i < bytes; i++){
		memory[addr+i] = source[i];
	}
};

/**
 * Read bytes from memory, default 1 bytes. D0: [target] = [addr], [target+1] = [addr+1]...
 * @param addr   : The addr in memory begin to read.
 * @param target : The recevier buf to gain data.
 * @param bytes  : How many bytes will be read.
 */
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

/**
 * Read one instruction frommemory.
 * @param addr   : The instruction addr in memory.
 * @param target : The receiver buf to gain the instruction.
 * @param num    : How many instruction will be read.
 */
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

		instruction = instruction | (onebyte << (i%4 * 8));

		if(i % 4 == 3){
			target[i/4] = instruction;
			instruction = 0;
			onebyte = 0;
		}
	}
};