#include <stdio.h>
#include <iostream>
#include <fstream>
#include "elf.h"

using namespace std;

void parseELF(char* filename){
    ifstream fin(filename, ios::binary);
    cout << "=====================parse ELF begin=====================" << endl;

    // parse ELF Header
    cout << "ELF Header:" << endl;
    Elf32_Ehdr elf_ehdr;
    string formatInfo;
    fin.read((char*)&elf_ehdr, sizeof(Elf32_Ehdr));
    cout << "  Magic:   ";
    cout << " " << hex << (unsigned int)elf_ehdr.e_ident[0]  << " " << (unsigned int)elf_ehdr.e_ident[1];
    cout << " " << hex << (unsigned int)elf_ehdr.e_ident[2]  << " " << (unsigned int)elf_ehdr.e_ident[3];
    cout << " " << hex << (unsigned int)elf_ehdr.e_ident[4]  << " " << (unsigned int)elf_ehdr.e_ident[5];
    cout << " " << hex << (unsigned int)elf_ehdr.e_ident[6]  << " " << (unsigned int)elf_ehdr.e_ident[7];
    cout << " " << hex << (unsigned int)elf_ehdr.e_ident[8]  << " " << (unsigned int)elf_ehdr.e_ident[9];
    cout << " " << hex << (unsigned int)elf_ehdr.e_ident[10] << " " << (unsigned int)elf_ehdr.e_ident[11];
    cout << " " << hex << (unsigned int)elf_ehdr.e_ident[12] << " " << (unsigned int)elf_ehdr.e_ident[13];
    cout << " " << hex << (unsigned int)elf_ehdr.e_ident[14] << " " << (unsigned int)elf_ehdr.e_ident[15];
    cout << endl;

    cout << "  Class:                                  0x" << hex << (unsigned int)elf_ehdr.e_ident[4]  << "; ";
        formatInfo = elf_ehdr.e_ident[4] == 1 ? "ELF32" : "ELF64";
        cout << formatInfo << endl; 
    cout << "  Data:                                   0x" << hex << (unsigned int)elf_ehdr.e_ident[5] << "; ";
        formatInfo = elf_ehdr.e_ident[5] == 0 ? "Invalid" : elf_ehdr.e_ident[5] == 1 ? "Little End" : "Big End";
        cout << formatInfo << endl;
    cout << "  Version:                                "   << dec << (unsigned int)elf_ehdr.e_ident[6]  << "; ";
        formatInfo = "(current)";
        cout << formatInfo << endl;
    cout << "  OS/ABI:                                 0x" << hex << (unsigned int)elf_ehdr.e_ident[7]  << "; ";
        formatInfo = "";
        cout << formatInfo << endl;
    cout << "  ABI Version:                            "   << dec << (unsigned int)elf_ehdr.e_ident[8]  << "; ";
        formatInfo = "";
        cout << formatInfo << endl;
    cout << "  Type:                                   0x" << hex << (unsigned int)elf_ehdr.e_type      << "; ";
        formatInfo = sz_desc_e_type[elf_ehdr.e_type]; 
        cout << formatInfo << endl;
    cout << "  Machine:                                0x" << hex << (unsigned int)elf_ehdr.e_machine   << "; ";
        formatInfo = elf_ehdr.e_machine < 9 && elf_ehdr.e_machine >=0 ? sz_desc_e_machine[(unsigned int)elf_ehdr.e_machine] : "<unknown>"; 
        cout << formatInfo << endl;
    cout << "  Version:                                "   << dec << (unsigned int)elf_ehdr.e_version   << "; " << endl;
    cout << "  Entry point Address:                    0x" << hex << (unsigned int)elf_ehdr.e_entry     << "; " << endl;
    cout << "  Start of program headers:               0x" << hex << (unsigned int)elf_ehdr.e_phoff     << "; (";
        formatInfo = " bytes into file)";
        cout << dec << (unsigned int)elf_ehdr.e_phoff      << formatInfo << endl;
    cout << "  Start of section headers:               0x" << hex << (unsigned int)elf_ehdr.e_shoff     << "; (";
        formatInfo = " bytes into file)";
        cout << dec << (unsigned int)elf_ehdr.e_shoff      << formatInfo << endl;
    cout << "  Flags                                   0x" << hex << (unsigned int)elf_ehdr.e_flags     << "; " << endl;
    cout << "  Size of this header:                    0x" << hex << (unsigned int)elf_ehdr.e_ehsize    << "; (";
        formatInfo = " bytes)";
        cout << dec << (unsigned int)elf_ehdr.e_ehsize     << formatInfo << endl;
    cout << "  Size of program headers:                0x" << hex << (unsigned int)elf_ehdr.e_phentsize << "; (";
        formatInfo = " bytes)";
        cout << dec << (unsigned int)elf_ehdr.e_phentsize  << formatInfo << endl;
    cout << "  Number of program headers:              "   << dec << (unsigned int)elf_ehdr.e_phnum     << "; " << endl;
    cout << "  Size of section headers:                0x" << hex << (unsigned int)elf_ehdr.e_shentsize << "; (";
        formatInfo = " bytes)";
        cout << dec << (unsigned int)elf_ehdr.e_shentsize << formatInfo << endl;
    cout << "  Number of section headers:              "   << dec << (unsigned int)elf_ehdr.e_shnum     << "; " << endl;
    cout << "  Section header string table index:      "   << dec << (unsigned int)elf_ehdr.e_shstrndx  << "; " << endl;
    cout << endl;

    cout << "---------------------------------------------------------" << endl << endl;

    cout << "ELF Program Headers:" << endl;
    Elf32_Phdr elf_phdr;
    for(int i = 0; i < elf_ehdr.e_phnum; i++){
        fin.seekg(elf_ehdr.e_phoff + i * elf_ehdr.e_phentsize, ios::beg);
        fin.read((char*)&elf_phdr, elf_ehdr.e_phentsize);

        cout << "  Entry " << dec << i << endl;
        cout << "      p_type:        0x" << hex << (unsigned int)elf_phdr.p_type   << endl;
        cout << "      p_offset:      0x" << hex << (unsigned int)elf_phdr.p_offset << endl;
        cout << "      p_vaddr:       0x" << hex << (unsigned int)elf_phdr.p_vaddr  << endl;
        cout << "      p_paddr:       0x" << hex << (unsigned int)elf_phdr.p_paddr  << endl;
        cout << "      p_filesz:      0x" << hex << (unsigned int)elf_phdr.p_filesz << "(";
            cout << dec << (unsigned int)elf_phdr.p_filesz  << " bytes)" << endl;
        cout << "      p_memsz:       0x" << hex << (unsigned int)elf_phdr.p_memsz  << "(";
            cout << dec << (unsigned int)elf_phdr.p_memsz   << " bytes)" << endl;
        cout << "      p_flags:       0x" << hex << (unsigned int)elf_phdr.p_flags  << endl;
        cout << "      p_align:       0x" << hex << (unsigned int)elf_phdr.p_align  << endl;
        cout << endl;  
    }

    cout << "======================parse ELF end======================" << endl << endl;
}
