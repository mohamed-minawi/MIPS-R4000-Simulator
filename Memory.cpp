//
//  Memory.cpp
//  MIPS_Pipelined_Simulator
//
//  Created by Mohamed A Tawfik on May/7/17.
//  Copyright © 2017 Mohamed A Tawfik. All rights reserved.
//

#include "Memory.hpp"

Memory::Memory(){
    mem_array.resize(0);
}
Memory::Memory(uint32_t size)
{
    mem_array.resize(size);
}

void Memory::resize(uint32_t size){
    mem_array.resize(size);
}
uint32_t Memory::read(uint32_t index)
{
    if(index < mem_array.size())
        return mem_array[index];
    else
        return 0;
}
bool Memory::insert(uint32_t index, uint32_t data, bool enable)
{
    if (index < mem_array.size() && enable){
        mem_array[index] = data;
        return true;
    }
    else
        return false;
}
bool Memory::insert(uint32_t data, bool enable)
{
    if (enable){
        mem_array.push_back(data);
        return true;
    }
    else
        return false;
}

