#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <map>
#include "elf.h"

#define EI_IDENT 16
#define PT_LOAD 1       //for elf_phdr.p_type
using namespace std;

int main(int argc, char** argv){
    // initial memory

    // get filename
    char filename[50];
    cout << "Filename: " << endl;
    cin >> filename;
    cout << "------------------------" << endl;

/*
    // read elf file
    Elf32_Ehdr elf_ehdr;
    fin.read((char*)&elf_ehdr, sizeof(elf_ehdr));
    cout << "e_ident:  " << elf_ehdr.e_ident   << endl;
    cout << "e_phnum:   " << elf_ehdr.e_phnum   << endl;

    unsigned int entry;
    entry = elf_ehdr.e_entry;
    cout << "Entry:     " << hex << entry << endl;

    Elf32_Phdr elf_phdr;
    for(int i = 0; i < elf_ehdr.e_phnum; i++){
        fin.seekg(elf_ehdr.e_phoff + i * elf_ehdr.e_phentsize, ios::beg);
        fin.read((char*)&elf_phdr, elf_ehdr.e_phentsize);
        cout << "----------------------------" << endl;
        cout << "p_vaddr: " << hex << elf_phdr.p_vaddr << endl;
        cout << "----------------------------" << endl;
        if(elf_phdr.p_type != PT_LOAD)
            continue;

        unsigned char set_byte;
        unsigned char get_byte;
        /*unsigned int  oneinst;
        string insts = "";
        */
        fin.seekg(elf_phdr.p_offset, ios::beg);
        for(int j = 0; j < elf_phdr.p_memsz; j++){
            if(j < elf_phdr.p_filesz){
                fin.read((char*)&set_byte, 1);
                cout << "write to   memory: " << hex << set_byte << endl;
                memoryWrite(elf_phdr.p_vaddr + j, &set_byte, 1);
                memoryRead( elf_phdr.p_vaddr + j, &get_byte, 1);
                cout << "read  from memory: " << hex << get_byte << endl;
                /*
                insts = insts + (char)get_byte[j%4];
                if(j%4 == 3){
                    oneinst = (unsigned int)stoi(insts);
                    cout << "read  from memory: " << hex << oneinst << endl;

                }
*/

            } else {
                set_byte = 0;
                memoryWrite(elf_phdr.p_vaddr + j, &set_byte, 1);
                cout << "write to   memory: " << hex << set_byte << "  set 0" << endl;
                memoryRead( elf_phdr.p_vaddr + j, &get_byte, 1);
                cout << "read  from memory: " << hex << get_byte << "  set 0" << endl;
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
