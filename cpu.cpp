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
			imm_11_5 = (inst >> 25) & 0x7f;
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
	unsigned int pc = decodeRes.i_pc;
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
	int BVal = 0;
	int HVal = 0;
	int WVal = 0;
	int val = 0;
	int left = 0;
	int right = 0;
	int amount = 0;
	int offset = 0;
#ifdef DEBUG
	cout << "Excute the instruction" << endl;
#endif

	switch (type){
		case LUI:
			reg[decodeRes.i_rd] = (decodeRes.i_imm << 12); 
			pc += 4;
			break;
		case AUIPC:
			reg[decodeRes.i_rd] = (decodeRes.i_imm << 12) + pc;
			pc += 4;
			break;
		//有疑问，这里我对这句指令的模拟可能不对
		case JAL:
			if (decodeRes.i_rd != 0){
				reg[decodeRes.i_rd] = pc + 4;
			}
			pc = ((decodeRes.i_imm << 11) >> 11) + pc;
			++sstack;		
			cout << "sstack:	" << sstack << endl;
			cout << "pc:	" << pc << endl;
			break;
		case JALR:
			if (decodeRes.i_rd != 0){
				reg[decodeRes.i_rd] = pc + 4;
			}
			pc = ((decodeRes.i_imm << 20) >> 20) + reg[decodeRes.i_rs1];
			--sstack;
			cout << "sstack:	" << sstack << endl;
			cout << "pc:	" << pc << endl;
			break;
		case BEQ:
			offset = (decodeRes.i_imm << 19) >> 19;
			pc = (reg[decodeRes.i_rs1] == reg[decodeRes.i_rs2]) ? (offset + pc) : (pc+4);
			break;
		case BNE:
			offset = (decodeRes.i_imm << 19) >> 19;
			pc = (reg[decodeRes.i_rs1] != reg[decodeRes.i_rs2]) ? (offset + pc) : (pc+4);			
			break;
		case BLT:
			offset = (decodeRes.i_imm << 19) >> 19;
			pc = ((int)reg[decodeRes.i_rs1] < (int)reg[decodeRes.i_rs2]) ? (offset + pc) : (pc+4);			
			break;
		case BLTU:
			offset = (decodeRes.i_imm << 19) >> 19;
			pc = (reg[decodeRes.i_rs1] < reg[decodeRes.i_rs2]) ? (offset + pc) : (pc+4);			
			break;
		case BGE:
			offset = (decodeRes.i_imm << 19) >> 19;
			pc = ((int)reg[decodeRes.i_rs1] >= (int)reg[decodeRes.i_rs2]) ? (offset + pc) : (pc+4);			
			break;
		case BGEU:
			offset = (decodeRes.i_imm << 19) >> 19;
			pc = (reg[decodeRes.i_rs1] >= reg[decodeRes.i_rs2]) ? (offset + pc) : (pc+4);			
			break;
		case LB:
			offset = (decodeRes.i_imm << 20) >> 20;
			addr = offset + reg[decodeRes.i_rs1];
			memory.memoryRead(addr, (unsigned char*)(&BVal), 1);
			reg[decodeRes.i_rd] = BVal;
			pc += 4;
			break;
		//从char到int和unsigned int 转换时如何处理符号位的，我的电脑是进行符号扩展的
		case LBU:
			offset = (decodeRes.i_imm << 20) >> 20;
			addr = offset + reg[decodeRes.i_rs1];
			memory.memoryRead(addr, (unsigned char*)(&BVal), 1);
			reg[decodeRes.i_rd] = BVal;
			pc += 4;
			break;
		case LH:
			offset = (decodeRes.i_imm << 20) >> 20;
			addr = offset + reg[decodeRes.i_rs1];
			memory.memoryRead(addr, (unsigned char*)(&HVal), 2);
			reg[decodeRes.i_rd] = HVal;
			pc += 4;
			break;
		case LHU:
			offset = (decodeRes.i_imm << 20) >> 20;
			addr = offset + reg[decodeRes.i_rs1];
			memory.memoryRead(addr, (unsigned char*)(&HVal), 2);
			reg[decodeRes.i_rd] = HVal;
			pc += 4;
			break;
		case LW:
			offset = (decodeRes.i_imm << 20) >> 20;
			addr = offset + reg[decodeRes.i_rs1];
			memory.memoryRead(addr, (unsigned char *)&WVal, 4);
			cout << "22222222222:	" << addr << endl;
			cout << "22222222222:	" << WVal << endl;
			cout << "0x10074:	" << (unsigned int)memory.memory[0x10074] << endl;
	cout << "0x10075:	" << (unsigned int)memory.memory[0x10075] << endl;
	cout << "0x10076:	" << (unsigned int)memory.memory[0x10076] << endl;
	cout << "0x10077:	" << (unsigned int)memory.memory[0x10077] << endl;
	cout << "0x10078:	" << (unsigned int)memory.memory[0x10078] << endl;
	cout << "0x10079:	" << (unsigned int)memory.memory[0x10079] << endl;
	cout << "0x1007a:	" << (unsigned int)memory.memory[0x1007a] << endl;
	cout << "0x1007b:	" << (unsigned int)memory.memory[0x1007b] << endl;
	cout << "0xfffff800:	" << (unsigned int)memory.memory[0xfffff800] << endl;
			reg[decodeRes.i_rd] = WVal;
			pc += 4;
			break;
		case SB:
			offset = (decodeRes.i_imm << 20) >> 20;
			addr = offset + (unsigned int)reg[decodeRes.i_rs1];
			WVal = reg[decodeRes.i_rs2];
			memory.memoryWrite(addr, (unsigned char *)&WVal, 1);
			pc += 4;
			break;
		case SH:
			offset = (decodeRes.i_imm << 20) >> 20; 
			addr = offset + (unsigned int)reg[decodeRes.i_rs1];
			WVal = reg[decodeRes.i_rs2];
			memory.memoryWrite(addr, (unsigned char *)&WVal, 2);
			pc += 4;
			break;
		case SW:
			offset = (decodeRes.i_imm << 20) >> 20;
			cout << offset << endl;
			addr = offset + (unsigned int)reg[decodeRes.i_rs1];			
			WVal = reg[decodeRes.i_rs2];
			cout << "11111111:	" << reg[decodeRes.i_rs2] << endl;
			cout << "addr:	" << addr << endl;
			cout << "WVal:	" << WVal << endl;
			memory.memoryWrite(addr, (unsigned char *)&WVal, 4);
			cout << (unsigned int)memory.memory[addr] << " ";
			cout << (unsigned int)memory.memory[addr + 1] << " ";
			cout << (unsigned int)memory.memory[addr + 2] << " ";
			cout << (unsigned int)memory.memory[addr + 3] << endl;
			pc += 4;
			break;
		case ADDI:
			offset = (decodeRes.i_imm << 20) >> 20;
			reg[decodeRes.i_rd] = reg[decodeRes.i_rs1] + offset;
			pc += 4;
			cout << "0x10074:	" << (unsigned int)memory.memory[0x10074] << endl;
	cout << "0x10075:	" << (unsigned int)memory.memory[0x10075] << endl;
	cout << "0x10076:	" << (unsigned int)memory.memory[0x10076] << endl;
	cout << "0x10077:	" << (unsigned int)memory.memory[0x10077] << endl;
	cout << "0x10078:	" << (unsigned int)memory.memory[0x10078] << endl;
	cout << "0x10079:	" << (unsigned int)memory.memory[0x10079] << endl;
	cout << "0x1007a:	" << (unsigned int)memory.memory[0x1007a] << endl;
	cout << "0x1007b:	" << (unsigned int)memory.memory[0x1007b] << endl;
	cout << "0xfffff800:	" << (unsigned int)memory.memory[0xfffff800] << endl;
			break;
		case SLTI:
			reg[decodeRes.i_rd] = (reg[decodeRes.i_rs1] < decodeRes.i_imm)? 1 : 0;
			pc += 4;
			break;
		case SLTIU:
			if (decodeRes.i_imm == 1){
				reg[decodeRes.i_rd] = (reg[decodeRes.i_rs1] == 0)? 1 : 0;
			}
			else {
				reg[decodeRes.i_rd] = ((unsigned int)reg[decodeRes.i_rs1] < (unsigned int)decodeRes.i_imm)? 1 : 0;
			}
			pc += 4;
			break;
		case XORI:
			reg[decodeRes.i_rd] = reg[decodeRes.i_rs1] ^ decodeRes.i_imm;
			pc += 4;
			break;
		case ORI:
			reg[decodeRes.i_rd] = reg[decodeRes.i_rs1] | decodeRes.i_imm;
			pc += 4;
			break;
		case ANDI:
			reg[decodeRes.i_rd] = reg[decodeRes.i_rs1] & decodeRes.i_imm;
			pc += 4;
			break;
		case SLLI:
			left = 0x1f;
			left |= decodeRes.i_imm;
			reg[decodeRes.i_rd] = reg[decodeRes.i_rs1] << left;
			pc += 4;
			break;
		case SRLI:
		//逻辑右移，要补0，怎么确认高位补0,直接除以2咋样！
		//因为我测试过了，我的电脑是默认算术右移的
			right = 0x1f;
			right |= decodeRes.i_imm;
			reg[decodeRes.i_rd] = reg[decodeRes.i_rs1] / pow(2, right);
			pc += 4;
			break;
		case SRAI:
			right = 0x1f;
			right |= decodeRes.i_imm;
			reg[decodeRes.i_rd] = reg[decodeRes.i_rs1] >> right;
			pc += 4;
			break;
		case ADD:
			reg[decodeRes.i_rd] = reg[decodeRes.i_rs1] + reg[decodeRes.i_rs2];
			pc += 4;
			break;
			//哪个减哪个？还是要注意的
		case SUB:
			reg[decodeRes.i_rd] = reg[decodeRes.i_rs1] - reg[decodeRes.i_rs2];
			pc += 4;
			break;
		case SLL:
			amount = 0x1f;
			amount |= reg[decodeRes.i_rs2];
			reg[decodeRes.i_rd] = reg[decodeRes.i_rs1] << amount;
			pc += 4;
			break;
		case SLT:
			reg[decodeRes.i_rd] = (reg[decodeRes.i_rs1] < reg[decodeRes.i_rs2])? 1 : 0;
			pc += 4;
			break;
		case SLTU:
			reg[decodeRes.i_rd] = ((unsigned int)reg[decodeRes.i_rs1] < (unsigned int)reg[decodeRes.i_rs2])? 1 : 0;
			pc += 4;
			break;
		case XOR:
			reg[decodeRes.i_rd] = reg[decodeRes.i_rs1] ^ reg[decodeRes.i_rs2];
			pc += 4;
			break;
		case OR:
			reg[decodeRes.i_rd] = reg[decodeRes.i_rs1] | reg[decodeRes.i_rs2];
			pc += 4;
			break;
		case AND:
			reg[decodeRes.i_rd] = reg[decodeRes.i_rs1] & reg[decodeRes.i_rs2];
			pc += 4;
			break;
		case SRL:
			amount = 0x1f;
			amount |= reg[decodeRes.i_rs2];
			reg[decodeRes.i_rd] = reg[decodeRes.i_rs1] / pow(2, amount);
			pc += 4;
			break;
		case SRA:
			amount = 0x1f;
			amount |= reg[decodeRes.i_rs2];
			reg[decodeRes.i_rd] = reg[decodeRes.i_rs1] >> amount;
			pc += 4;
			break;
	}
	cout << "Instruction type:	" << decodeRes.i_type << endl;
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
