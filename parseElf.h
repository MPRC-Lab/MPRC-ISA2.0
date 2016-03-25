#ifndef __PARSERELF_H__
#define __PARSERELF_H__
#include "Memory.h"

#include <vector>
#include <string>
#include <unordered_map>

/*
extern unordered_map<unsigned int, string> symbolFunc;
extern vector<string> rodata;
*/
void parseELF(char* filename, unsigned int &bssBegin, unsigned int &bssSize, unsigned int &gp, unsigned int &sp, unordered_map<string, pair<unsigned int, unsigned int> > &gVar, unordered_map<unsigned int, string> &symbolFunc, vector<string> &rodata, string &originRodata);
unsigned int loadELF(char* filename, Memory &memory);
#endif