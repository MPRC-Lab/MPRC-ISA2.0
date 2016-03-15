#include "../zlib/head.h"
#include "cpu.h"

#define DEBUG

unsigned int Cpu::fetch(unsigned int pc, Memory& memory){
	unsigned int* target = new unsigned int();
	memory.instructionRead(pc, target, 1);
	unsigned int inst = *target;
	delete target;

#ifdef DEBUG
	cout << "Fetch the instruction" << endl;
	cout << "pc:	" << hex << pc << "		instruction:	" << hex << inst << endl;
#endif
	return inst;
}


void Cpu::decode(unsigned int pc, unsigned int inst, DecodeRes& decodeRes){
	decodeRes.i_pc = pc;
	unsigned int imm_20, imm_10_1, imm_11, imm_19_12, imm_11_0, imm_11_5;
	unsigned int imm_12, imm_10_5, imm_4_1, imm_4_0;
	int opcode = inst & 0x7f;
	cout << hex << "opcode:	0x" << opcode << endl;
	int funct3 = (inst >> 12) & 0x7;
	cout << hex << "funct3:	0x" << funct3 << endl;

#ifdef DEBUG
	cout << "Decode the instruction" << endl;
#endif

	decodeRes.i_rd = (inst >> 7) & 0x1f;
	decodeRes.i_rs1 = (inst >> 15) & 0x1f;
	decodeRes.i_rs2 = (inst >> 20) & 0x1f;
	switch (opcode) {
		case 0x37:
			decodeRes.i_type = 0;
			decodeRes.i_imm = inst & 0xfff;
			break;

		case 0x17:
			decodeRes.i_type = 1;
			decodeRes.i_imm = inst & 0xfff;
			break;
		//signed-extend? how? 到底要用有符号立即数还是无符号立即数不大确定
		case 0x6f:
			decodeRes.i_type = 2;
			imm_20 = (inst >> 31) & 1;
			imm_10_1 = (inst >> 21) & 0x3ff;
			imm_11 = (inst >> 20) & 1;
			imm_19_12 = (inst >> 12) & 0xff;
			decodeRes.i_imm = (imm_20 << 20) | (imm_10_1 << 1) | (imm_11 << 11) | (imm_19_12 << 12);
			break;

		case 0x67:
			decodeRes.i_type = 3;
			decodeRes.i_imm = (inst >> 20) & 0xfff;
			break;

		case 0x63:
			imm_12 = (inst >> 31) & 1;
			imm_10_5 = (inst >> 25) & 0x3f;
			imm_4_1 = (inst >> 8) & 0xf;
			imm_11 = (inst >> 7) & 1;
			decodeRes.i_imm = (imm_12 << 12) | (imm_10_5 << 5) | (imm_4_1 << 1) | (imm_11 << 11);
			switch (funct3){
				case 0x0:
					decodeRes.i_type = 4;
					break;
				case 0x1:
					decodeRes.i_type = 5;
					break;
				case 0x4:
					decodeRes.i_type = 6;
					break;
				case 0x5:
					decodeRes.i_type = 7;
					break;
				case 0x6:
					decodeRes.i_type = 8;
					break;
				case 0x7:
					decodeRes.i_type = 9;
					break;

			}
			break;

		case 0x3:
			decodeRes.i_imm = (inst >> 20) & 0xfff;
			switch (funct3){
				case 0x0:
					decodeRes.i_type = 10;
					break;
				case 0x1:
					decodeRes.i_type = 11;
					break;
				case 0x2:
					decodeRes.i_type = 12;
					break;
				case 0x4:
					decodeRes.i_type = 13;
					break;
				case 0x5:
					decodeRes.i_type = 14;
					break;
			}
			break;

		case 0x23:
			imm_11_5 = (inst >> 25) & 0x3f;
			imm_4_0 = (inst >> 7) & 0x1f;
			decodeRes.i_imm = (imm_11_5 << 5) | imm_4_0;
			switch (funct3){
				case 0x0:
					decodeRes.i_type = 15;
					break;
				case 0x1:
					decodeRes.i_type = 16;
					break;
				case 0x2:
					decodeRes.i_type = 17;
					break;
			}
			break;

		case 0x13:
			decodeRes.i_imm = (inst >> 20) & 0xfff;
			switch (funct3){
				case 0x0:
					decodeRes.i_type = 18;
					break;
				case 0x2:
					decodeRes.i_type = 19;
					break;
				case 0x3:
					decodeRes.i_type = 20;
					break;
				case 0x4:
					decodeRes.i_type = 21;
					break;
				case 0x6:
					decodeRes.i_type = 22;
					break;
				case 0x7:
					decodeRes.i_type = 23;
					break;
				case 0x1:
					decodeRes.i_type = 24;
					break;
				case 0x5:
					if (((inst >> 30) & 1) == 0){
						decodeRes.i_type = 25;
					}
					else {
						decodeRes.i_type = 26;
					}	
					break;
			}
			break;

		case 0x33:
			switch(funct3){
				case 0x0:
					if (((inst >> 30) & 1) == 0){
						decodeRes.i_type = 27;
					}
					else {
						decodeRes.i_type = 28;
					}
					break;
				case 0x1:
					decodeRes.i_type = 29;
					break;
				case 0x2:
					decodeRes.i_type = 30;
					break;
				case 0x3:
					decodeRes.i_type = 31;
					break;
				case 0x4:
					decodeRes.i_type = 32;
					break;
				case 0x5:
					if (((inst >> 30) & 1) == 0){
						decodeRes.i_type = 33;
					}
					else {
						decodeRes.i_type = 34;
					}
					break;
				case 0x6:
					decodeRes.i_type = 35;
					break;
				case 0x7:
					decodeRes.i_type = 36;
					break;
			}
			break;
 	}
 	cout << "decode output" << endl;
 	cout << "Instruction type:	0x" << hex << decodeRes.i_type << endl;
 	cout << "Instruction rs1:	0x" << hex << decodeRes.i_rs1 << "	reg[rs1]	" << reg[decodeRes.i_rs1] << endl;
	cout << "Instruction rs2:	0x" << hex << decodeRes.i_rs2 << "	reg[rs2]	" << reg[decodeRes.i_rs2] << endl;
	cout << "Instruction rd:	0x" << hex << decodeRes.i_rd << "	reg[rd]	" << reg[decodeRes.i_rd] << endl;
	cout << "Instruction imm:	0x" << hex << decodeRes.i_imm << endl;
 	return;
}

unsigned int Cpu::excute(Memory& memory, const DecodeRes& decodeRes, int& sstack){
	//pc等于当前指令地址
	int pc = decodeRes.i_pc;
	int type = decodeRes.i_type;
	unsigned int uimm = 0;
	int imm = 0;
	unsigned int addr = 0;
	unsigned char* target = new unsigned char();
	unsigned char* target2 = new unsigned char[2];
	unsigned char* target4 = new unsigned char[4];
	unsigned char* source = new unsigned char();
	unsigned char* source2 = new unsigned char[2];
	unsigned char* source4 = new unsigned char[4];
	int val = 0;
	int uVal = 0;
	int left = 0;
	int right = 0;
	int amount = 0;
#ifdef DEBUG
	cout << "Excute the instruction" << endl;
#endif

	switch (type){
		case LUI:
			reg[decodeRes.i_rd] = (decodeRes.i_imm << 12); 
			pc += 4;
#ifdef DEBUG
	cout << "instruction:	" << "LUI" << endl;
#endif
			break;
		case AUIPC:
			reg[decodeRes.i_rd] = (decodeRes.i_imm << 12) + pc;
			pc += 4;
#ifdef DEBUG
	cout << "instruction:	" << "AUIPC" << endl;
#endif
			break;
		//有疑问，这里我对这句指令的模拟可能不对
		case JAL:
			if (decodeRes.i_rd != 0){
				reg[decodeRes.i_rd] = pc + 4;
			}
			pc = (decodeRes.i_imm << 11) >> 11 + pc;
			++sstack;
#ifdef DEBUG
	cout << "instruction:	" << "JAL" << endl;
#endif
			break;
		case JALR:
			if (decodeRes.i_rd != 0){
				reg[decodeRes.i_rd] = pc + 4;
			}
			pc = (decodeRes.i_imm << 20) >> 20 + pc;
			--sstack;
#ifdef DEBUG
			cout << "instruction:	" << "JALR" << endl;
#endif
			break;
		case BEQ:
			pc = (reg[decodeRes.i_rs1] == reg[decodeRes.i_rs2]) ? ((decodeRes.i_imm << 19) >> 19 + pc) : (pc+4);
#ifdef DEBUG
	cout << "instruction:	" << "BEQ" << endl;
#endif
			break;
		case BNE:
			pc = (reg[decodeRes.i_rs1] != reg[decodeRes.i_rs2]) ? ((decodeRes.i_imm << 19) >> 19 + pc) : (pc+4);
#ifdef DEBUG
	cout << "instruction:	" << "BNE" << endl;
#endif			
			break;
		case BLT:
			pc = (reg[decodeRes.i_rs1] < reg[decodeRes.i_rs2]) ? ((decodeRes.i_imm << 19) >> 19 + pc) : (pc+4);
#ifdef DEBUG
	cout << "instruction:	" << "BLT" << endl;
#endif
			break;
		case BLTU:
			pc = ((unsigned int)reg[decodeRes.i_rs1] < (unsigned int)reg[decodeRes.i_rs2]) ? ((decodeRes.i_imm << 19) >> 19 + pc) : (pc+4);
#ifdef DEBUG
	cout << "instruction:	" << "BLTU" << endl;
#endif
			break;
		case BGE:
			pc = (reg[decodeRes.i_rs1] > reg[decodeRes.i_rs2]) ? (decodeRes.i_imm+pc) : (pc+4);
#ifdef DEBUG
	cout << "instruction:	" << "BGE" << endl;
#endif
			break;
		case BGEU:
			pc = ((unsigned int)reg[decodeRes.i_rs1] > (unsigned int)reg[decodeRes.i_rs2]) ? (decodeRes.i_imm+pc) : (pc+4);
#ifdef DEBUG
	cout << "instruction:	" << "BGEU" << endl;
#endif
			break;
		case LB:
			addr = decodeRes.i_imm + reg[decodeRes.i_rs1];
			memory.memoryRead(addr, target, 1);
			val = 0xff;
			val &= target[0];
			reg[decodeRes.i_rd] = val;
			pc += 4;
#ifdef DEBUG
	cout << "instruction:	" << "LB" << endl;
#endif
			break;
		//从char到int和unsigned int 转换时如何处理符号位的，我的电脑是进行符号扩展的
		case LBU:
			addr = decodeRes.i_imm + reg[decodeRes.i_rs1];
			memory.memoryRead(addr, target, 1);
			uVal = 0xff;
			uVal &= target[0];
			reg[decodeRes.i_rd] = uVal;
			pc += 4;
#ifdef DEBUG
	cout << "instruction:	" << "LBU" << endl;
#endif
			break;
		case LH:
			addr = decodeRes.i_imm + reg[decodeRes.i_rs1];
			memory.memoryRead(addr, target2, 2);
			val = 0xffff;
			val &= target[0];
			val &= target[1] << 8;
			reg[decodeRes.i_rd] = val;
			pc += 4;
#ifdef DEBUG
	cout << "instruction:	" << "LH" << endl;
#endif
			break;
		case LHU:
			addr = decodeRes.i_imm + reg[decodeRes.i_rs1];
			memory.memoryRead(addr, target2, 2);
			val = 0xffff;
			val &= target[0];
			val &= target[1] << 8;
			reg[decodeRes.i_rd] = val;
			pc += 4;
#ifdef DEBUG
	cout << "instruction:	" << "LHU" << endl;
#endif
			break;
		case LW:
			addr = decodeRes.i_imm + reg[decodeRes.i_rs1];
			memory.memoryRead(addr, target4, 4);
			val = 0xffffffff;
			val &= target[0];
			val &= target[1] << 8;
			val &= target[2] << 16;
			val &= target[3] << 24;
			reg[decodeRes.i_rd] = val;
			pc += 4;
#ifdef DEBUG
	cout << "instruction:	" << "LW" << endl;
#endif
			break;
		case SB:
			addr = (unsigned int)reg[decodeRes.i_imm] + (unsigned int)reg[decodeRes.i_rs1];
			val = 0xff;
			val &= reg[decodeRes.i_rs2];
			source[0] = val;
			memory.memoryWrite(addr, source, 1);
			pc += 4;
#ifdef DEBUG
	cout << "instruction:	" << "SB" << endl;
#endif
			break;
		case SH:
			addr = (unsigned int)reg[decodeRes.i_imm] + (unsigned int)reg[decodeRes.i_rs1];
			val = 0xffff;
			val &= reg[decodeRes.i_rs2];
			source[0] = val & 0xff;
			source[1] = (val >> 8) & 0xff;
			memory.memoryWrite(addr, source2, 2);
			pc += 4;
#ifdef DEBUG
	cout << "instruction:	" << "SH" << endl;
#endif
			break;
		case SW:
			addr = (unsigned int)reg[decodeRes.i_imm] + (unsigned int)reg[decodeRes.i_rs1];
			val = 0xffffffff;
			val &= reg[decodeRes.i_rs2];
			source[0] = val & 0xff;
			source[1] = (val >> 8) & 0xff;
			source[2] = (val >> 16) & 0xff;
			source[3] = (val >> 24) & 0xff;
			memory.memoryWrite(addr, source4, 4);
			pc += 4;
#ifdef DEBUG
	cout << "instruction:	" << "SW" << endl;
#endif
			break;
		case ADDI:
			reg[decodeRes.i_rd] = reg[decodeRes.i_rs1] + decodeRes.i_imm;
			pc += 4;
#ifdef DEBUG
	cout << "instruction:	" << "ADDI" << endl;
#endif
			break;
		case SLTI:
			reg[decodeRes.i_rd] = (reg[decodeRes.i_rs1] < decodeRes.i_imm)? 1 : 0;
			pc += 4;
			break;
#ifdef DEBUG
	cout << "instruction:	" << "SLTI" << endl;
#endif
		case SLTIU:
			if (decodeRes.i_imm == 1){
				reg[decodeRes.i_rd] = (reg[decodeRes.i_rs1] == 0)? 1 : 0;
			}
			else {
				reg[decodeRes.i_rd] = ((unsigned int)reg[decodeRes.i_rs1] < (unsigned int)decodeRes.i_imm)? 1 : 0;
			}
			pc += 4;
#ifdef DEBUG
	cout << "instruction:	" << "SLTIU" << endl;
#endif
			break;
		case XORI:
			reg[decodeRes.i_rd] = reg[decodeRes.i_rs1] ^ decodeRes.i_imm;
			pc += 4;
#ifdef DEBUG
	cout << "instruction:	" << "XORI" << endl;
#endif
			break;
		case ORI:
			reg[decodeRes.i_rd] = reg[decodeRes.i_rs1] | decodeRes.i_imm;
			pc += 4;
#ifdef DEBUG
	cout << "instruction:	" << "ORI" << endl;
#endif
			break;
		case ANDI:
			reg[decodeRes.i_rd] = reg[decodeRes.i_rs1] & decodeRes.i_imm;
			pc += 4;
#ifdef DEBUG
	cout << "instruction:	" << "ANDI" << endl;
#endif
			break;
		case SLLI:
			left = 0x1f;
			left |= decodeRes.i_imm;
			reg[decodeRes.i_rd] = reg[decodeRes.i_rs1] << left;
			pc += 4;
#ifdef DEBUG
	cout << "instruction:	" << "SLLI" << endl;
#endif
			break;
		case SRLI:
		//逻辑右移，要补0，怎么确认高位补0,直接除以2咋样！
		//因为我测试过了，我的电脑是默认算术右移的
			right = 0x1f;
			right |= decodeRes.i_imm;
			reg[decodeRes.i_rd] = reg[decodeRes.i_rs1] / pow(2, right);
			pc += 4;
#ifdef DEBUG
	cout << "instruction:	" << "SRLI" << endl;
#endif
			break;
		case SRAI:
			right = 0x1f;
			right |= decodeRes.i_imm;
			reg[decodeRes.i_rd] = reg[decodeRes.i_rs1] >> right;
			pc += 4;
#ifdef DEBUG
	cout << "instruction:	" << "SRAI" << endl;
#endif
			break;
		case ADD:
			reg[decodeRes.i_rd] = reg[decodeRes.i_rs1] + reg[decodeRes.i_rs2];
			pc += 4;
#ifdef DEBUG
	cout << "instruction:	" << "ADD" << endl;
#endif
			break;
			//哪个减哪个？还是要注意的
		case SUB:
			reg[decodeRes.i_rd] = reg[decodeRes.i_rs1] - reg[decodeRes.i_rs2];
			pc += 4;
#ifdef DEBUG
	cout << "instruction:	" << "SUB" << endl;
#endif
			break;
		case SLL:
			amount = 0x1f;
			amount |= reg[decodeRes.i_rs2];
			reg[decodeRes.i_rd] = reg[decodeRes.i_rs1] << amount;
			pc += 4;
#ifdef DEBUG
	cout << "instruction:	" << "SLL" << endl;
#endif
			break;
		case SLT:
			reg[decodeRes.i_rd] = (reg[decodeRes.i_rs1] < reg[decodeRes.i_rs2])? 1 : 0;
			pc += 4;
#ifdef DEBUG
	cout << "instruction:	" << "SLT" << endl;
#endif
			break;
		case SLTU:
			reg[decodeRes.i_rd] = ((unsigned int)reg[decodeRes.i_rs1] < (unsigned int)reg[decodeRes.i_rs2])? 1 : 0;
			pc += 4;
#ifdef DEBUG
	cout << "instruction:	" << "SLTU" << endl;
#endif
			break;
		case XOR:
			reg[decodeRes.i_rd] = reg[decodeRes.i_rs1] ^ reg[decodeRes.i_rs2];
			pc += 4;
#ifdef DEBUG
	cout << "instruction:	" << "XOR" << endl;
#endif
			break;
		case OR:
			reg[decodeRes.i_rd] = reg[decodeRes.i_rs1] | reg[decodeRes.i_rs2];
			pc += 4;
#ifdef DEBUG
	cout << "instruction:	" << "OR" << endl;
#endif
			break;
		case AND:
			reg[decodeRes.i_rd] = reg[decodeRes.i_rs1] & reg[decodeRes.i_rs2];
			pc += 4;
#ifdef DEBUG
	cout << "instruction:	" << "AND" << endl;
#endif
			break;
		case SRL:
			amount = 0x1f;
			amount |= reg[decodeRes.i_rs2];
			reg[decodeRes.i_rd] = reg[decodeRes.i_rs1] / pow(2, amount);
			pc += 4;
#ifdef DEBUG
	cout << "instruction:	" << "SRL" << endl;
#endif
			break;
		case SRA:
			amount = 0x1f;
			amount |= reg[decodeRes.i_rs2];
			reg[decodeRes.i_rd] = reg[decodeRes.i_rs1] >> amount;
			pc += 4;
#ifdef DEBUG
	cout << "instruction:	" << "SRA" << endl;
#endif
			break;
	}
//	cout << "Instruction type:	" << decodeRes.i_type << endl;
	cout << "Instruction rs1:	0x" << decodeRes.i_rs1 << "	reg[rs1]:	" << reg[decodeRes.i_rs1] << endl;
	cout << "Instruction rs2:	0x" << decodeRes.i_rs2 << "	reg[rs2]:	" << reg[decodeRes.i_rs2] << endl;
	cout << "Instruction rd:	0x" << decodeRes.i_rd << "	reg[rd]:	" << reg[decodeRes.i_rd] << endl;
	cout << "Instruction imm:	0x" << decodeRes.i_imm << endl;
	delete target;
	delete []target2;
	delete []target4;
	delete source;
	delete []source2;
	delete []source4;
	return pc;
}