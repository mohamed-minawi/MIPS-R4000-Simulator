//
//  Pipeline_Buffers_Handler.cpp
//  MIPS_Pipelined_Simulator
//
//  Created by Mohamed A Tawfik on May/7/17.
//  Copyright © 2017 Mohamed A Tawfik. All rights reserved.
//

#include "Pipeline_Buffers_Handler.hpp"

void Pipeline_Buffers_Handler::assignInputBuffers(std::vector <Register_Logic>& Pipelined_Registers, int s, int e){
    for(int i = s; i <= e; i++)
        Pipelined_Registers[i].buffer.setD(Pipelined_Registers[i].input);
}

void Pipeline_Buffers_Handler::resetBuffers(std::vector <Register_Logic>& Pipelined_Registers, int s, int e, bool should_reset){
    for(int i = s; i <= e; i++)
        Pipelined_Registers[i].buffer.setReset(should_reset);
}

void Pipeline_Buffers_Handler::enableBuffers(std::vector <Register_Logic>& Pipelined_Registers, int s, int e, bool should_enable){
    for(int i = s; i <= e; i++)
        Pipelined_Registers[i].buffer.setEnable(should_enable);
}

void Pipeline_Buffers_Handler::updateBuffers(std::vector <Register_Logic>& Pipelined_Registers, int s, int e){
    for(int i = s; i <= e; i++)
        Pipelined_Registers[i].buffer.update();
    
}

void Pipeline_Buffers_Handler::readDataBuffers(std::vector <Register_Logic>& Pipelined_Registers, int s, int e){
    for(int i = s; i <= e; i++)
        Pipelined_Registers[i].buffer.getQ(Pipelined_Registers[i].output);
}

void Pipeline_Buffers_Handler::resizeBuffers(std::vector <Register_Logic>& Pipelined_Registers, int s, int e, int size){
    for(int i = s; i <= e; i++){
        Pipelined_Registers[i].input.resize(size);
        Pipelined_Registers[i].output.resize(size);
    }
}

void Pipeline_Buffers_Handler::copyForward(Register_Logic& to, Register_Logic& from){
    for(int i = 0; i < from.output.size() && i < to.input.size(); i++)
        to.input[i] = from.output[i];
}


