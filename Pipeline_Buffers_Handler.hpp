//
//  Pipeline_Buffers_Handler.hpp
//  MIPS_Pipelined_Simulator
//
//  Created by Mohamed A Tawfik on May/7/17.
//  Copyright © 2017 Mohamed A Tawfik. All rights reserved.
//

#ifndef Pipeline_Buffers_Handler_hpp
#define Pipeline_Buffers_Handler_hpp

#include <vector>
#include <stdint.h>
#include "Register.hpp"

class Pipeline_Buffers_Handler{
public:
    struct Register_Logic{
        std::vector<uint32_t> input;
        std::vector<uint32_t> output;
        Register buffer;
    };
    
    static void assignInputBuffers(std::vector <Register_Logic>& Pipelined_Registers, int s, int e);
    static void resetBuffers(std::vector <Register_Logic>& Pipelined_Registers, int s, int e, bool should_reset);
    static void enableBuffers(std::vector <Register_Logic>& Pipelined_Registers, int s, int e, bool should_enable);
    static void updateBuffers(std::vector <Register_Logic>& Pipelined_Registers, int s, int e);
    static void readDataBuffers(std::vector <Register_Logic>& Pipelined_Registers, int s, int e);
    static void resizeBuffers(std::vector <Register_Logic>& Pipelined_Registers, int s, int e, int size);
    
    static void copyForward(Register_Logic& to, Register_Logic& from);

};

#endif /* Pipeline_Buffers_Handler_hpp */
