#ifndef __MEMORY_H__
#define __MEMORY_H__
#include <unordered_map>

using namespace std;

class Memory{
public:
	unordered_map<unsigned int, unsigned char> memory;
public:
	void memoryClear();
	void memoryWrite(unsigned int addr, unsigned char* source, unsigned int bytes = 1);
	void memoryRead( unsigned int addr, unsigned char* target, unsigned int bytes = 1);
	void instructionRead(  unsigned int addr, unsigned int* target, unsigned int num = 1);
};

#endif