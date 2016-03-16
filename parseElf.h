#ifndef __PARSERELF_H__
#define __PARSERELF_H__
#include "Memory.h"
void parseELF(char* filename, unsigned int &bssBegin, unsigned int &bssSize, unsigned int &gp, unsigned int &sp, unordered_map<string, pair<unsigned int, unsigned int> > &gVar);
unsigned int loadELF(char* filename, Memory &memory);
#endif