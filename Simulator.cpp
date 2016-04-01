/**
 ***********************************************
 * Author     : Bao Chuquan, Zhang Guowei
 * Date       : 2016-03-16
 * Descrption :
 *   A Simulator of RISC-V Instruction Level.
 *   With RISC-V 32 Base Integer Instruction Complemented.
 ***********************************************
 */

#include "parseELF.h"
#include "Memory.h"
#include "cpu.h"
#include <iomanip>
#include <bitset>
#include <unordered_map>
#include <iostream>
#include <time.h>

using namespace std;
//#define DEBUG
int main(int argc, char** argv){
	char filename[50];
	cout << "Filename: ";
	cin >> filename;

	unsigned int bssBegin;						// the address of .bss area
	unsigned int bssSize;						// the size of .bss area
	unsigned int gp;
	unsigned int sp;
	unordered_map<unsigned int, string> symbolFunc;
	vector<string> rodata;
	string originRodata;

	unordered_map<string, pair<unsigned int, unsigned int> > gVar;
	parseELF(filename, bssBegin, bssSize, gp, sp, gVar, symbolFunc, rodata, originRodata);

	Memory memory;
	unsigned int entryPoint; 					// the entry point of program
	entryPoint = loadELF(filename, memory);
#ifdef DEBUG
	cout << "+++++++++++++++++++++++Key Value+++++++++++++++++++++++" << endl;
	cout << "entryPoint: 0x" << hex << entryPoint << endl;
	cout << ".bssBegin:  0x" << hex << bssBegin   << endl;
	cout << ".bssSize:   "   << dec << bssSize    << " bytes"         << endl;
	cout << "sp[R2]:     0x" << hex << setw(8)    << setfill('0')     << sp   << endl;
	cout << "gp[R3]:     0x" << hex << setw(8)    << setfill('0')     << gp   << endl;
	cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl << endl;
#endif
	//simulate the instruction excution
	unsigned int pc = entryPoint;
	int sstack = 1;
	DecodeRes decodeRes;
	Cpu cpu(gp, sp);
	unsigned int inst;
#ifdef DEBUG
	cout << "-----------simulate the instruction excution---------" << endl;
#endif
	int count = 1;
	while (sstack > 0){
#ifdef DEBUG
		cout << "+++++++++++++++++++++++ No." << dec << count << " +++++++++++++++++++++++" << endl;
#endif
		inst = cpu.fetch(pc, memory);
		cpu.decode(pc, inst, decodeRes);
		pc = cpu.excute(memory, decodeRes, sstack, symbolFunc, rodata, originRodata);
		count++;
//		cout << "sstack: " << sstack << endl;
	}
	cout << "Instruction Number: " << dec << count << endl;
/*
#ifdef DEBUG
	cout << endl;
	cout << "----------- Output the result ------------" << endl;
#endif

#ifdef DEBUG
	int res;
	for (unordered_map<string, pair<unsigned int, unsigned int> >::iterator it = gVar.begin(); it != gVar.end(); ++it){
		cout << it->first << "		";
		for (int i = 0; i < (it->second).second; i = i + 4){
			memory.memoryRead((it->second).first + i, (unsigned char*)&res, 4);
			cout << dec << res << "		";
		}
		cout << endl;
	}
#endif
*/
//debug
/*
	cout << endl;
	cout << "----------- Print Memory -----------" << endl;
	for (unordered_map<unsigned int, unsigned char>::iterator it = memory.memory.begin(); it != memory.memory.end(); ++it){
		unsigned char data;
		memory.memoryRead(it->first, (unsigned char*)&data, 1);
		cout << "  addr: 0x" << hex << setw(0) << setfill('0') << it->first << "    0x" << hex << setw(2) << setfill('0') << (unsigned int)data << endl;
	}
*/
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

	//debug
/*
#ifdef DEBUG
	cout << "+++++++++++++++++++++++" << endl;
	cout << "    a0: 0x" << hex << setw(0) << setfill('0') << cpu.reg[10] << endl;
	cout << "    a1: 0x" << hex << setw(0) << setfill('0') << cpu.reg[11] << endl;
#endif
*/
 	return 0;
}
