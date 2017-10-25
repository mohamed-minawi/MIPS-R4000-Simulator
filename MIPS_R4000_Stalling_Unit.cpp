//
//  MIPS_R4000_Stalling_Unit.cpp
//  MIPS_Pipelined_Simulator
//
//  Created by Mohamed A Tawfik on Apr/21/17.
//  Copyright © 2017 Mohamed A Tawfik. All rights reserved.
//

#include "MIPS_R4000_Stalling_Unit.hpp"

bool MIPS_R4000_Stalling_Unit::getDecision(uint32_t rs1_EX, uint32_t rs2_EX, uint32_t rs2_DF, bool memtoreg_DF, uint32_t rs2_DS, bool memtoreg_DS){
    if(memtoreg_DF){
        if(rs1_EX == rs2_DF || rs2_EX == rs2_DF)
            if(rs2_DF != 0)
                return true;
    }
    if(memtoreg_DS){
        if(rs1_EX == rs2_DS || rs2_EX == rs2_DS)
            if(rs2_DS != 0)
                return true;
    }
    
    return false;
}
