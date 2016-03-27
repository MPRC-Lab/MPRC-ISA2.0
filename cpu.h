#ifndef __CPU_H__
#define __CPU_H__
#include "Memory.h"
#include <vector>
#include <string>
#include <unordered_map>




class DecodeRes {
public:
	DecodeRes():i_type(0), i_rs1(0), i_rs2(0), i_rd(0), i_imm(0), i_pc(0){};
	unsigned int i_type;
	unsigned int i_rs1;
	unsigned int i_rs2;
	unsigned int i_rd;
	int i_imm;
	unsigned int i_pc;
};

class Cpu {
public:
	std::vector<unsigned int> reg;
	Cpu(unsigned int gp, unsigned int sp){
		reg.resize(32, 0);
		reg[2] = sp;
		reg[3] = gp;
	};
	unsigned int fetch(unsigned int, Memory&);
	void decode(unsigned int, unsigned int, DecodeRes&);
	unsigned int excute(Memory&, const DecodeRes&, int&, unordered_map<unsigned int, string>&, vector<string>&, string&);
	void printReg();
	bool detect(unsigned int, Memory&, unordered_map<unsigned int, string>&, vector<string>&, string&);
	void callPrint(const vector<string>&, unsigned int, Memory&, string&);
	//simulate the registers,下面这个为什么错误？
	//vector<int> reg(32, 0);
};


//encode the type of opration
#define LUI			0
#define AUIPC		1
#define JAL			2
#define JALR		3
#define BEQ			4
#define BNE			5
#define BLT			6
#define BGE			7
#define BLTU		8
#define BGEU		9
#define LB			10
#define LH			11
#define LW			12
#define LBU			13
#define LHU			14
#define SB			15
#define SH			16
#define SW			17
#define ADDI		18
#define SLTI		19
#define SLTIU		20
#define XORI		21
#define ORI			22
#define ANDI		23
#define SLLI		24
#define SRLI		25
#define SRAI		26
#define ADD			27
#define SUB			28
#define SLL			29
#define SLT			30
#define SLTU		31
#define XOR			32
#define SRL			33
#define SRA			34
#define OR			35
#define AND			36
#define SCALL		37

#endif