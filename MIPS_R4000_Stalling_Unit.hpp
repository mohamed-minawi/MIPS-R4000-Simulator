//
//  MIPS_R4000_Stalling_Unit.hpp
//  MIPS_Pipelined_Simulator
//
//  Created by Mohamed A Tawfik on Apr/21/17.
//  Copyright © 2017 Mohamed A Tawfik. All rights reserved.
//

#ifndef MIPS_R4000_Stalling_Unit_hpp
#define MIPS_R4000_Stalling_Unit_hpp

#include <stdint.h>

class MIPS_R4000_Stalling_Unit{
public:
    static bool getDecision(uint32_t rs1_EX, uint32_t rs2_EX, uint32_t rs2_DF, bool memtoreg_DF, uint32_t rs2_DS, bool memtoreg_DS);
};

#endif /* MIPS_R4000_Stalling_Unit_hpp */
