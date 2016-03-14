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

/*
	cout << endl << endl;
	cout << "entryPoint: 0x"   << hex << entryPoint << endl;
	cout << ".bssBegin:  0x" << hex << bssBegin << endl;
	cout << ".bssSize:   "   << dec << bssSize << " bytes" << endl;
*/

 	return 0;
}