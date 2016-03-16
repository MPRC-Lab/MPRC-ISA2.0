#include "parseELF.h"
#include "Memory.h"
#include "cpu.h"

using namespace std;

int main(){
	char filename[50];
	cout << "Filename: ";
	cin >> filename;

	unsigned int bssBegin;						// the address of .bss area
	unsigned int bssSize;						// the size of .bss area
	parseELF(filename, bssBegin, bssSize);

	Memory memory;
	unsigned int entryPoint; 					// the entry point of program 
	entryPoint = loadELF(filename, memory);

///* Print entry Point, .bssBegin, .bssEnd
	cout << "entryPoint: 0x" << hex << entryPoint << endl;
	cout << ".bssBegin:  0x" << hex << bssBegin << endl;
	cout << ".bssSize:   "   << dec << bssSize << " bytes" << endl;
//*/
	//simulate the instruction excution
	unsigned int pc = entryPoint;
	int sstack = 1;
	DecodeRes decodeRes;
	Cpu cpu;
	unsigned int inst;

	cout << "-----------simulate the instruction excution---------" << endl; 
	while (sstack > 0){
		inst = cpu.fetch(pc, memory);
		cpu.decode(pc, inst, decodeRes);
		pc = cpu.excute(memory, decodeRes, sstack);
		cout << "sstack:	" << sstack << endl;
	}

	//output the bss segment
	unsigned char* res = new unsigned char[4];
	cout << "ZGW: bssBegin: 0x"<<bssBegin<<endl; 
	cout << "-----------Output the result------------" << endl;
	cout << "0x10074:	" << (unsigned int)memory.memory[0x10074] << endl;
	cout << "0x10075:	" << (unsigned int)memory.memory[0x10075] << endl;
	cout << "0x10076:	" << (unsigned int)memory.memory[0x10076] << endl;
	cout << "0x10077:	" << (unsigned int)memory.memory[0x10077] << endl;
	cout << "0x10078:	" << (unsigned int)memory.memory[0x10078] << endl;
	cout << "0x10079:	" << (unsigned int)memory.memory[0x10079] << endl;
	cout << "0x1007a:	" << (unsigned int)memory.memory[0x1007a] << endl;
	cout << "0x1007b:	" << (unsigned int)memory.memory[0x1007b] << endl;
	cout << "0xfffff800:	" << (unsigned int)memory.memory[0xfffff800] << endl;
	
	for (int i = 0; i < bssSize; i = i + 4){
		memory.memoryRead(bssBegin + i, res, 4);
		//unsigned int val = 0xffffffff;
		/*
		val &= res[0];
		val &= (res[1] << 8);
		val &= (res[2] << 16);
		val &= (res[3] << 24);
		cout << "result:	" << hex << val << endl;
		*/
		cout << hex << (unsigned int)res[0] << endl; 
		cout << hex << (unsigned int)res[1] << endl;
		cout << hex << (unsigned int)res[2] << endl;
		cout << hex << (unsigned int)res[3] << endl;
	}

	delete []res;

/* Test instructionRead()
	unsigned int instruction[20];
	unsigned addr = 0x00010000;
	memory.instructionRead(addr, instruction, 20);
	for(int i = 0; i < 20; i++){
		cout << "Addr: "   << hex << setw(8) << setfill('0') << addr+i << "    ";
		cout << "Inst: "   << hex << setw(8) << setfill('0') << instruction[i] << endl;
	}
*/

/* Test memoryRead()
	unsigned char bytes[20];
	unsigned addr = 0x00010000;
	memory.memoryRead(addr, bytes, 20);
	for(int i = 0; i < 20; i++){
		cout << "Addr: "   << hex << setw(8) << setfill('0') << addr+i << "    ";
		cout << "Byte: "   << hex << setw(2) << setfill('0') << (unsigned int)bytes[i] << endl;
	}
*/

	

 	return 0;
}