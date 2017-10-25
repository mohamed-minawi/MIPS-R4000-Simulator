//
//  MIPS_Control_Unit.hpp
//  MIPS_Pipelined_Simulator
//
//  Created by Mohamed A Tawfik on May/7/17.
//  Copyright © 2017 Mohamed A Tawfik. All rights reserved.
//

#ifndef MIPS_Control_Unit_hpp
#define MIPS_Control_Unit_hpp

#include <stdint.h>

class MIPS_Control_Unit
{
public:
    static void getControlSignals(uint32_t OPCODE, uint32_t function, uint32_t& MemToReg, uint32_t& MemWrite, uint32_t& ALUSrc, uint32_t& RegDst, uint32_t& RegWrite, uint32_t& Branch, uint32_t& Branch_on_equal, uint32_t& Jump, uint32_t& Link_on_Jumping, uint32_t& Stack_Jumping, uint32_t& ALUcontrol);
    //static uint32_t getControlSignals(uint32_t OPCODE, uint32_t function);
};

#endif /* MIPS_Control_Unit_hpp */
