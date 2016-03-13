#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cstdio>
#include "Memory.h"
#include "Memory.cpp"
#include "Processor.h"
#include "Processor.cpp"
#include "elf.h"

#define EI_IDENT 16
#define PT_LOAD 1       //for elf_phdr.p_type
using namespace std;


int main(int argc, char** argv){
    //step 1: load code & data
    //step 1.1: get file name
    char filename[100];
    cout << "Please input the excute filename: " << endl;
    cin >> filename;
    cout << filename << endl;

    Processor processor;
  //  processor.load(filename);
  //
    ifstream fin(filename, ios::binary);
    //cout << fin << endl;
    if(!fin){
         cout << "File open failed. Please try again.";
         return 0;
    }

    Memory mem;

    Elf32_Ehdr elf_ehdr;
    fin.read((char*)&elf_ehdr, sizeof(elf_ehdr));
    cout << hex << elf_ehdr.e_ident << endl;
    cout << "e_phnum: " << elf_ehdr.e_phnum << endl;

    unsigned int entry;
    entry = elf_ehdr.e_entry;
    cout << "Entry: " << hex << entry << endl;

    Elf32_Phdr elf_phdr;
    for(int i = 0; i < elf_ehdr.e_phnum; i++){
        fin.seekg(elf_ehdr.e_phoff + i * elf_ehdr.e_phentsize, ios::beg);
        fin.read((char*)&elf_phdr, elf_ehdr.e_phentsize);
        cout << "p_vaddr: " << hex << elf_phdr.p_vaddr << endl;
        if(elf_phdr.p_type != PT_LOAD)
            continue;

        unsigned char one_byte;
        unsigned char get_byte;
        fin.seekg(elf_phdr.p_offset, ios::beg);
        for(int j = 0; j < elf_phdr.p_memsz; j++){
            if(j < elf_phdr.p_filesz){
                fin.read((char*)&one_byte, 1);
                cout << "one_byte: " << hex << one_byte << endl;
                mem.write(elf_phdr.p_vaddr + j, &one_byte, 1);
                mem.read( elf_phdr.p_vaddr + j, &get_byte, 1);
                cout << "read from memory: " << hex << get_byte << endl;
            } else {
                one_byte = 0;
                mem.write(elf_phdr.p_vaddr + j, &one_byte, 1);
                mem.read( elf_phdr.p_vaddr + j, &get_byte, 1);
                cout << "read from memory: " << hex << get_byte << endl;
            }
        }
    }

    //step 2: processor start
    //step 2.1: fetch
    //step 2.2: decode
    //step 2.3: excute
    //step 2.4: writeback
    //step 2.5: update pc

    return 0;
}
