//
//  MIPS_Assembler.hpp
//  MIPS_Pipelined_Simulator
//
//  Created by Mohamed A Tawfik on Mar/17/17.
//  Copyright © 2017 Mohamed A Tawfik. All rights reserved.
//

#ifndef MIPS_ASSEMBLER_HPP
#define MIPS_ASSEMBLER_HPP

#include <stdint.h>
#include <string>
#include <sstream>

class MIPS_Assembler{
public:
    // Translates a MIPS true instruction to its equivalent machine code. Throws an std::invalid_argument exception if instruction is invalid or unsupported.
    static uint32_t exprToMachine(const std::string&);
private:
    enum Type {R, I, J};
    
    static const unsigned int Registers_num = 32;
    static const unsigned int Bit_size = 32;
    static const unsigned int Max_26 = 0x3FFFFFF;
    static const unsigned int Max_16 = 0xFFFF;
    
    static const Type getTypeAndIndex(const std::string&, unsigned int&);
    
    static void getNextInput(std::stringstream&, uint32_t&);
    
    static void throwIfNotEmpty(const std::stringstream&);
    static void throwIfEmpty(const std::stringstream&);
    
    static void checkBounds(const uint32_t, const uint32_t, const std::string&);
    
    static const unsigned int R_num = 18;
    static const std::string R_instr[R_num];
    static const uint32_t R_functs[R_num];
    
    static const unsigned int I_num = 11;
    static const std::string I_instr[I_num];
    static const uint32_t I_opcodes[I_num];
    
    static const unsigned int J_num = 3;
    static const std::string J_instr[J_num];
    static const uint32_t J_opcodes[J_num];
};

#endif
