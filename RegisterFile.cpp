//
//  RegisterFile.cpp
//  MIPS_Pipelined_Simulator
//
//  Created by Mohamed A Tawfik on May/8/17.
//  Copyright © 2017 Mohamed A Tawfik. All rights reserved.
//

#include "RegisterFile.hpp"

#include <stdexcept>

RegisterFile::RegisterFile(){
    array.resize(0);
}

RegisterFile::RegisterFile(uint32_t size){
    array.resize(size);
    for (int i = 0; i < array.size(); i++)
        array[i] = 0;
}

RegisterFile::RegisterFile(const RegisterFile& other):array(other.array){
}

void RegisterFile::resizeAndReset(uint32_t size){
    array.resize(size);
    for (int i = 0; i < array.size(); i++)
        array[i] = 0;
}

void RegisterFile::write(uint32_t index, uint32_t value, bool RegWrite){
    if (RegWrite && index != 0 && index < array.size())
        array[index] = value;
}

uint32_t RegisterFile::read(uint32_t index){
    if(index < array.size())
        return array[index];
    else
        return 0;
}

RegisterFile::~RegisterFile(){
}
