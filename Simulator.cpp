#include "parseELF.h"
#include "parseELF.cpp"
#include "Memory.h"
#include "Memory.cpp"

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