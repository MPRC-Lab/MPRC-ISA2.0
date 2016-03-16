#include "parseELF.h"
#include "Memory.h"
#include "cpu.h"
#include <iomanip>
#include <bitset>
#include <unordered_map>
#include <iostream>

using namespace std;

int main(int argc, char** argv){
	char filename[50];
	cout << "Filename: ";
	cin >> filename;

	unsigned int bssBegin;						// the address of .bss area
	unsigned int bssSize;						// the size of .bss area
	unsigned int gp;
	unsigned int sp;
	unordered_map<string, pair<unsigned int, unsigned int> > gVar;
	parseELF(filename, bssBegin, bssSize, gp, sp, gVar);

	Memory memory;
	unsigned int entryPoint; 					// the entry point of program 
	entryPoint = loadELF(filename, memory);

	cout << "+++++++++++++++++++++++Key Value+++++++++++++++++++++++" << endl;
	cout << "entryPoint: 0x" << hex << entryPoint << endl;
	cout << ".bssBegin:  0x" << hex << bssBegin   << endl;
	cout << ".bssSize:   "   << dec << bssSize    << " bytes"         << endl;
	cout << "sp[R2]:     0x" << hex << setw(8)    << setfill('0')     << sp   << endl;
	cout << "gp[R3]:     0x" << hex << setw(8)    << setfill('0')     << gp   << endl;
	cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl << endl;
	//simulate the instruction excution
	unsigned int pc = entryPoint;
	int sstack = 1;
	DecodeRes decodeRes;
	Cpu cpu(gp, sp);
	unsigned int inst;

	cout << "-----------simulate the instruction excution---------" << endl; 
	while (sstack > 0){
		inst = cpu.fetch(pc, memory);
		cpu.decode(pc, inst, decodeRes);
		pc = cpu.excute(memory, decodeRes, sstack);
	}

	cout << "-----------Output the result------------" << endl;
	int res;
	for (unordered_map<string, pair<unsigned int, unsigned int> >::iterator it = gVar.begin(); it != gVar.end(); ++it){
		cout << it->first << "		";
		for (int i = 0; i < (it->second).second; i = i + 4){
			memory.memoryRead((it->second).first + i, (unsigned char*)&res, 4);
			cout << dec << res << "		";
		}
		cout << endl;
	}
	memory.memoryClear();
	/*
	cout << "bss:	" << (unsigned int)memory.memory[0x10074] << endl;
	cout << "bss:	" << (unsigned int)memory.memory[0x10075] << endl;
	cout << "bss:	" << (unsigned int)memory.memory[0x10076] << endl;
	cout << "bss:	" << (unsigned int)memory.memory[0x10077] << endl;
	cout << "bss:	" << (unsigned int)memory.memory[0x10078] << endl;
	cout << "bss:	" << (unsigned int)memory.memory[0x10079] << endl;
	cout << "bss:	" << (unsigned int)memory.memory[0x1007a] << endl;
	cout << "bss:	" << (unsigned int)memory.memory[0x1007b] << endl;
	*/
	/*
	for (int i = 0; i < bssSize; i = i + 4){
		memory.memoryRead(bssBegin + i, (unsigned char*)&res, 4);
		cout << dec << res << endl; 
	}*/	
 	return 0;
}