#pragma once
#include <map>

using namespace std;

class Memory{
private:
	unordered_map<unsigned int, unsigned char> memory;
public:
	void clear(){};
	void memoryWrite(unsigned int addr, unsigned char* source, unsigned int bytes = 1){};
	void memoryRead( unsigned int addr, unsigned char* target, unsigned int bytes = 1){};
	unsigned int instructionRead( unsigned int addr, unsigned int num = 1){};
};