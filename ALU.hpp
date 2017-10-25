//
//  ALU.hpp
//  MIPS_Pipelined_Simulator
//
//  Created by Mohamed A Tawfik on May/11/17.
//  Copyright © 2017 Mohamed A Tawfik. All rights reserved.
//

#ifndef ALU_H
#define ALU_H

#include <stdint.h>

class ALU{
public:
    static void ALUResult(uint32_t a, uint32_t b, uint32_t function, uint32_t shamout, uint32_t& result, bool& zeroflag);
    static void ALUResult(uint32_t a, uint32_t b, uint32_t function, uint32_t shamout, uint32_t& result);
};

#endif /* ALU_hpp */
