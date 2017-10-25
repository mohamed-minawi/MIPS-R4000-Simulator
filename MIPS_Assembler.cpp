//
//  MIPS_Assembler.cpp
//  MIPS_Pipelined_Simulator
//
//  Created by Mohamed A Tawfik on Mar/17/17.
//  Copyright © 2017 Mohamed A Tawfik. All rights reserved.
//

#include "MIPS_Assembler.hpp"

uint32_t MIPS_Assembler::exprToMachine(const std::string& expression){
    std::stringstream e(expression);
    std::string instr;
    e >> instr;
    
    uint32_t machine_code, opcode, funct, rd, rt, rs, shamt, immi_16, immi_26;
    machine_code = opcode = funct = rd = rt = rs = shamt = immi_16 = immi_26 = 0x0;
    
    unsigned int j = INT_MAX;
    
    switch(getTypeAndIndex(instr, j)){
        case Type::R: // NOTE: check if added (or removed) any R instructions
            funct = R_functs[j];
            getNextInput(e, rd);
            switch(j){
                case 17: // rpr
                case 3: // jr
                    rs = rd;
                    rd = 0;
                    break;
                case 6: // sll
                case 10: // sra
                case 12: // srl
                    getNextInput(e, rt);
                    getNextInput(e, shamt);
                    break;
                case 7: // sllv
                case 11: // srav
                case 13: // srlv
                    getNextInput(e, rt);
					getNextInput(e, rs);
                    break;
                default: // add addu and nor or slt sltu sub subu xor
                    getNextInput(e, rs);
					getNextInput(e, rt);
            }
            break;
            
        case Type::I: // NOTE: check if added (or removed) any I instructions
            opcode = I_opcodes[j];
            getNextInput(e, rt);
            switch(j){
                case 6: // lui
                    getNextInput(e, immi_16);
                    break;
                case 5: // lw
                case 9: // sw
                    getNextInput(e, immi_16);
                    getNextInput(e, rs);
                    break;
                default: // addi addiu andi beq bne ori slti xori
                    getNextInput(e, rs);
                    getNextInput(e, immi_16);
            }
            break;
            
        case Type::J: // NOTE: check if added (or removed) any J instructions
            opcode = J_opcodes[j];
            if(j < 2)
                getNextInput(e, immi_26);
            break;
    }
    
    checkBounds(rd, Registers_num - 1, "Destination register");
    checkBounds(rt, Registers_num - 1, "Target register");
    checkBounds(rs, Registers_num - 1, "Source register");
    checkBounds(shamt, Bit_size - 1, "Shift amount");
    // checkBounds(immi_26, Max_26, "Jump target");
    // checkBounds(immi_16, Max_16, "Immediate");
    
    throwIfNotEmpty(e);
    
    immi_16 &= Max_16;
    immi_26 &= Max_26;
    
    opcode <<= 26; rs <<= 21; rt <<= 16; rd <<= 11; shamt <<= 6;
    
    machine_code = opcode | rs | rt | rd | shamt | funct | immi_16 | immi_26;
    
    return machine_code;
}

const MIPS_Assembler::Type MIPS_Assembler::getTypeAndIndex(const std::string& instr, unsigned int& j){
    for(int i = 0; i < R_num; ++i)
        if(R_instr[i] == instr){
            j = i;
            return Type::R;
        }
    for(int i = 0; i < I_num; ++i)
        if(I_instr[i] == instr){
            j = i;
            return Type::I;
        }
    for(int i = 0; i < J_num; ++i)
        if(J_instr[i] == instr){
            j = i;
            return Type::J;
        }
    throw std::invalid_argument("Unsupported or invalid instruction");
}

void MIPS_Assembler::getNextInput(std::stringstream& e, uint32_t& input){
    throwIfEmpty(e);
    int buffer;
    e >> buffer;
    input = buffer;
}

void MIPS_Assembler::throwIfNotEmpty(const std::stringstream& e){
    if(e.rdbuf()->in_avail())
        throw std::invalid_argument("Invalid syntax");
}
void MIPS_Assembler::throwIfEmpty(const std::stringstream& e){
    if(!e.rdbuf()->in_avail())
        throw std::invalid_argument("Invalid syntax");
}

void MIPS_Assembler::checkBounds(const uint32_t number, const uint32_t limit, const std::string& s){
    if(number > limit)
        throw std::invalid_argument(s + " out of bounds");
}

const std::string MIPS_Assembler::R_instr[R_num] = {"add", "addu", "and", "jr", "nor", "or", "sll", "sllv", "slt", "sltu", "sra", "srav", "srl", "srlv", "sub", "subu", "xor","rpr"};
const uint32_t MIPS_Assembler::R_functs[R_num] = {0x20, 0x21, 0x24, 0x8, 0x27, 0x25 ,0x0, 0x4, 0x2A, 0x2B, 0x3, 0x7, 0x2, 0x6, 0x22, 0x23, 0x26, 0x1B};

const std::string MIPS_Assembler::I_instr[I_num] = {"addi", "addiu", "andi", "beq", "bne", "lw", "lui", "ori", "slti", "sw", "xori"};
const uint32_t MIPS_Assembler::I_opcodes[I_num] = {0x8, 0x9, 0xC, 0x4, 0x5, 0x23, 0xF, 0xD, 0xA, 0x2B, 0xE};

const std::string MIPS_Assembler::J_instr[J_num] = {"j", "jal", "jpr"};
const uint32_t MIPS_Assembler::J_opcodes[J_num] = {0x2, 0x3, 0x1C};
