//
//  MIPS_Decoder.hpp
//  MIPS_Pipelined_Simulator
//
//  Created by Mohamed A Tawfik on May/7/17.
//  Copyright © 2017 Mohamed A Tawfik. All rights reserved.
//

#ifndef MIPS_Decoder_hpp
#define MIPS_Decoder_hpp

#include <stdint.h>

class MIPS_Decoder{
public:
    static void decodeInstruction(uint32_t instruction, uint32_t& OpCode, uint32_t& function, uint32_t& RS, uint32_t& RT, uint32_t& RD, uint32_t& immi_ext);
};
#endif /* MIPS_Decoder_hpp */
