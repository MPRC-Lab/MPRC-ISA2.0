#include <stdio.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <bitset>
#include <vector>
#include "elf.h"
#include "parseELF.h"
#include <unordered_map>
#include "Memory.h"

#define PT_LOAD 1

using namespace std;

/**
 * pareseELF 
 * @param filename : input filename
 * @param bssBegin : function will return the .bss begin address
 * @param bssSize  : function will return the .bss area size (bytes)
 */
void parseELF(char* filename, unsigned int &bssBegin, unsigned int &bssSize, unsigned int &gp, unsigned int &sp, unordered_map<string, pair<unsigned int, unsigned int> > &gVar, unordered_map<unsigned int, string> &symbolFunc, vector<string> &rodata, string &originRodata){
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
        formatInfo = elf_ehdr.e_machine < 9 && elf_ehdr.e_machine >= 0 ? sz_desc_e_machine[(unsigned int)elf_ehdr.e_machine] : "<unknown>"; 
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

    cout << "---------------------------------------------------------" << endl;

    cout << "ELF Program Headers: (" << dec << (unsigned int)elf_ehdr.e_phnum << " headers)" << endl;
    Elf32_Phdr elf_phdr;
    for(int i = 0; i < elf_ehdr.e_phnum; i++){
        fin.seekg(elf_ehdr.e_phoff + i * elf_ehdr.e_phentsize, ios::beg);
        fin.read((char*)&elf_phdr, elf_ehdr.e_phentsize);

        cout << "    Entry " << dec << i << endl;
        cout << "        p_type:        0x" << hex << (unsigned int)elf_phdr.p_type   << endl;
        cout << "        p_offset:      0x" << hex << (unsigned int)elf_phdr.p_offset << endl;
        cout << "        p_vaddr:       0x" << hex << (unsigned int)elf_phdr.p_vaddr  << endl;
        cout << "        p_paddr:       0x" << hex << (unsigned int)elf_phdr.p_paddr  << endl;
        cout << "        p_filesz:      0x" << hex << (unsigned int)elf_phdr.p_filesz << "(";
            cout << dec << (unsigned int)elf_phdr.p_filesz  << " bytes)" << endl;
        cout << "        p_memsz:       0x" << hex << (unsigned int)elf_phdr.p_memsz  << "(";
            cout << dec << (unsigned int)elf_phdr.p_memsz   << " bytes)" << endl;
        cout << "        p_flags:       0x" << hex << (unsigned int)elf_phdr.p_flags  << endl;
        cout << "        p_align:       0x" << hex << (unsigned int)elf_phdr.p_align  << endl;
        cout << endl;   

        // set sp[R2] value
        if(elf_phdr.p_type == PT_LOAD){
            if(i == 0)
                sp = elf_phdr.p_vaddr;
            else 
                sp = sp < elf_phdr.p_vaddr ? sp : elf_phdr.p_vaddr;
        }
    }

    cout << "---------------------------------------------------------" << endl;
    cout << "ELF Section Headers: (" << dec << (unsigned int)elf_ehdr.e_shnum << " headers)" << endl;
    Elf32_Shdr elf_shdr;
    Elf32_Shdr elf_shdr_shstrtab;
    Elf32_Shdr elf_shdr_sym;
    Elf32_Shdr elf_shdr_strtab; 
    Elf32_Shdr elf_shdr_rodata;
    Elf32_Shdr elf_shdr_sdata;

    string sectionName = "";
    string sectionType = "";

    fin.seekg(elf_ehdr.e_shoff + elf_ehdr.e_shstrndx * elf_ehdr.e_shentsize, ios::beg);
    fin.read((char*)&elf_shdr_shstrtab, elf_ehdr.e_shentsize);

    for(int i = 0; i < elf_ehdr.e_shnum; i++){
        fin.seekg(elf_ehdr.e_shoff + i * elf_ehdr.e_shentsize, ios::beg);
        fin.read((char*)&elf_shdr, elf_ehdr.e_shentsize);

        // get name
        char nameChar;
        sectionName = "";
        sectionType = "";
        fin.seekg(elf_shdr_shstrtab.sh_offset + elf_shdr.sh_name, ios::beg);
        fin.read((char*)&nameChar, sizeof(char));
        int j = 0;
        while(nameChar != '\0'){
            j++;
            sectionName += nameChar;
            fin.seekg(elf_shdr_shstrtab.sh_offset + elf_shdr.sh_name + j, ios::beg);
            fin.read((char*)&nameChar, sizeof(char));
        }
        //bcq

        cout << "    Entry " << dec << i << ": " << endl;
        cout << "        Name:          "   << sectionName << endl;
        sectionType = elf_shdr.sh_type < 12 ? sz_desc_sh_type[(unsigned int)elf_shdr.sh_type] : "Unknown";
        cout << "        Type:          "   << sectionType   << endl;
        cout << "        Addr:          0x" << hex << setw(8) << setfill('0') << (unsigned int)elf_shdr.sh_addr   << endl;
        cout << "        Off:           0x" << hex << setw(8) << setfill('0') << (unsigned int)elf_shdr.sh_offset << endl;
        cout << "        Size:          "   << dec << (unsigned int)elf_shdr.sh_size      << endl;
        cout << "        Entry Size:    "   << dec << (unsigned int)elf_shdr.sh_entsize   << endl;
        formatInfo = elf_shdr.sh_flags < 8 ? sz_desc_sh_flags[(unsigned int)elf_shdr.sh_flags] : "Unknown";
        cout << "        Flags:         "   << formatInfo << endl;
        cout << "        Link:          "   << dec << (unsigned int)elf_shdr.sh_link      << endl;
        cout << "        Info:          "   << dec << (unsigned int)elf_shdr.sh_info      << endl;
        cout << "        Align:         "   << dec << (unsigned int)elf_shdr.sh_addralign << endl;

        if(sectionName == ".bss"){
            bssBegin = (unsigned int)elf_shdr.sh_addr;
            bssSize  = (unsigned int)elf_shdr.sh_size;
            //cout << "bssBegin: 0x" << hex << bssBegin << "    bssSize: " << dec << bssSize << endl;
        }

        if(sectionName == ".symtab"){
            elf_shdr_sym = elf_shdr;
        }
        if(sectionName == ".strtab"){
            elf_shdr_strtab = elf_shdr;
        }
        if(sectionName == ".rodata"){
            elf_shdr_rodata = elf_shdr;
        }
        if(sectionName == ".sdata"){
            elf_shdr_sdata = elf_shdr;
        }
    }

    cout << "---------------------------------------------------------" << endl;
    cout << "ELF Symbol Table:" << endl;
    Elf32_Sym elf_sym;
    string tagName = "";
    for (int i = 0; i < elf_shdr_sym.sh_size / elf_shdr_sym.sh_entsize; i++){
        fin.seekg(elf_shdr_sym.sh_offset + i * elf_shdr_sym.sh_entsize, ios::beg);
        fin.read((char*)&elf_sym, elf_shdr_sym.sh_entsize);

        // get name
        char nameChar;
        tagName = "";
        fin.seekg(elf_shdr_strtab.sh_offset + elf_sym.st_name, ios::beg);
        fin.read((char*)&nameChar, sizeof(char));
        int j = 0;
        while(nameChar != '\0'){
            j++;
            tagName += nameChar;
            fin.seekg(elf_shdr_strtab.sh_offset + elf_sym.st_name + j, ios::beg);
            fin.read((char*)&nameChar, sizeof(char));
        }
        cout << "    Entry " << dec << i << ": " << endl;
        cout << "        Name:          "   << tagName << endl;
        cout << "        value:         0x" << hex << setw(8) << setfill('0') << elf_sym.st_value << endl;
        cout << "        Size:          "   << dec << elf_sym.st_size << endl;
        unsigned int index = (elf_sym.st_info & 0x0000000f);
        cout << "        Info(Type):    "   << syb_type[index] << endl;
        index =              (elf_sym.st_info & 0xfffffff0);
        cout << "        Info(bind):    "   << syb_bind[index] << endl;

        // Store name & address to symbolFunc
        symbolFunc[elf_sym.st_value] = tagName;

        if(tagName == "_gp"){
            gp = elf_sym.st_value;
        }

        if (elf_sym.st_value >= bssBegin && elf_sym.st_value < bssBegin + bssSize && tagName != ""){
            gVar[tagName] = make_pair(elf_sym.st_value, elf_sym.st_size);
        }
    }
    cout << "======================parse ELF end======================" << endl << endl;

    cout << "==================== print symbolFunc ===================" << endl;
    for(unordered_map<unsigned int, string>:: iterator it = symbolFunc.begin(); it != symbolFunc.end(); ++it){
        cout << "    0x" << hex << setw(8) << setfill('0') << it->first << ":  ";
        cout << it->second << endl; 
    }
    cout << "======================== end print ======================" << endl << endl;

    cout << "====================== print rodata =====================" << endl;
    // get rodata
    
    char rodataChar;
    string rodataName = "";

    fin.seekg(elf_shdr_rodata.sh_offset, ios::beg);
    fin.read((char*)&rodataChar, sizeof(char));
    int j = 0;
    while(j < elf_shdr_rodata.sh_size){//rodataChar != '\0' && 
        j++;
        rodataName += rodataChar;
        fin.seekg(elf_shdr_rodata.sh_offset + j, ios::beg);
        fin.read((char*)&rodataChar, sizeof(char));
    }
    cout << "rodata:  " << rodataName << endl;
    originRodata = rodataName;

    // store %s
    char flagChar;
    string flagName = "";
    fin.seekg(elf_shdr_rodata.sh_offset, ios::beg);
    fin.read((char*)&flagChar, sizeof(char));
    j = 0;
    while(j < elf_shdr_rodata.sh_size){
        j++;
        fin.seekg(elf_shdr_rodata.sh_offset + j, ios::beg);
        if(flagChar == '%'){
            flagName = "";
            fin.read((char*)&flagChar, sizeof(char));
            flagName += flagChar;
            rodata.push_back(flagName);
        } else {
            fin.read((char*)&flagChar, sizeof(char));
        }
    }
    cout << "vector<string> rodata: ";
    for(int i = 0; i < rodata.size(); i++){
        cout << rodata[i] << " ";
    }
    cout << endl;
    cout << "======================== end print ======================" << endl << endl;

    cout << "======================= print sdata =====================" << endl;
    // get rodata
    char sdataChar;
    string sdataName = "";

    fin.seekg(elf_shdr_sdata.sh_offset, ios::beg);
    fin.read((char*)&sdataChar, sizeof(char));
    j = 0;
    while(j < elf_shdr_sdata.sh_size){//rodataChar != '\0' && 
        j++;
        sdataName += sdataChar;
        fin.seekg(elf_shdr_sdata.sh_offset + j, ios::beg);
        fin.read((char*)&sdataChar, sizeof(char));
    }
    cout << "sdata:  " << sdataName << endl;
    cout << "======================== end print ======================" << endl << endl;

    fin.close();
}

/**
 * Load elf to memory
 * @param  filename : the input filename
 * @param  memory   : memory
 * @return entry    : entry point (the main address)
 */
unsigned int loadELF(char* filename, Memory &memory){
    unsigned int entry;
    string formatInfo = "";
    ifstream fin(filename, ios::binary);

    Elf32_Ehdr elf_ehdr;
    fin.read((char*)&elf_ehdr, sizeof(elf_ehdr));
    entry = elf_ehdr.e_entry;

    cout << "********************* ELF loading *********************" << endl;
    cout << "    Entry point:                  0x" << hex << entry << endl;
    cout << "    Program Headers parsing..."       << endl;
    cout << "    Sections loading..."          << endl;

    Elf32_Phdr elf_phdr;
    string typeName = "";
    for(int i = 0; i < elf_ehdr.e_phnum; i++){
        fin.seekg(elf_ehdr.e_phoff + i * elf_ehdr.e_phentsize, ios::beg);
        fin.read((char*)&elf_phdr, elf_ehdr.e_phentsize);

        typeName = "";
        cout << "    -----------------------------------------------"      << endl;
        cout << "    Sections " << dec << i << endl;
        typeName = elf_phdr.p_type < 7 ? sz_desc_p_type[(unsigned int)elf_phdr.p_type] : "Unknown";
        cout << "        p_type:         "   << typeName << endl;
        cout << "        p_offset:       0x" << hex << elf_phdr.p_offset << endl;
        cout << "        p_vaddr:        0x" << hex << elf_phdr.p_vaddr  << endl;
        cout << "        p_paddr:        0x" << hex << elf_phdr.p_paddr  << endl;
        cout << "        p_filesz:       0x" << hex << elf_phdr.p_filesz << endl;
        cout << "        p_memsz:        0x" << hex << elf_phdr.p_memsz  << endl;
        cout << "        p_flags:        0x" << hex << elf_phdr.p_flags  << endl;
        cout << "        p_align:        0x" << hex << elf_phdr.p_align  << endl;

        if(elf_phdr.p_type != PT_LOAD){
            cout << "        ** skip this section." << endl;
            continue;
        }

        unsigned int instruction = 0;
        unsigned char one_byte   = 0;
        fin.seekg(elf_phdr.p_offset, ios::beg);
        for(int j = 0; j < elf_phdr.p_memsz; j++){
            if(j < elf_phdr.p_filesz){
                fin.read((char*)&one_byte, 1);
                memory.memoryWrite(elf_phdr.p_vaddr + j, &one_byte, 1);
            } else {
                one_byte = 0;
                memory.memoryWrite(elf_phdr.p_vaddr + j, &one_byte, 1);
            }

            if(j % 4 == 0){
                cout << "        Addr: 0x" << hex << setw(8) << setfill('0') << (unsigned int)elf_phdr.p_vaddr + j;
            }
            instruction = instruction | (one_byte << (j%4 * 8));
            if(j % 4 == 3){
                cout << "        hex: "    << hex << setw(8) << setfill('0') << instruction;
                cout << "        bin: "    << bitset<32>(instruction) << endl;
                instruction = 0;
                one_byte    = 0;
            }
        }
    }
    fin.close();
    cout << "****************** ELF load complete ******************" << endl << endl;
    return entry;
}