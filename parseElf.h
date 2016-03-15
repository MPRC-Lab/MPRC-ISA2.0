#ifndef __PARSERELF_H__
#define __PARSERELF_H__
#include "Memory.h"
void parseELF(char* filename, unsigned int &bssBegin, unsigned int &bssSize);
unsigned int loadELF(char* filename, Memory &memory);
#endif