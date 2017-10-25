//
//  Memory.hpp
//  MIPS_Pipelined_Simulator
//
//  Created by Mohamed A Tawfik on May/7/17.
//  Copyright © 2017 Mohamed A Tawfik. All rights reserved.
//

#ifndef Memory_hpp
#define Memory_hpp

#include <vector>
#include <stdint.h>

using namespace std;

class Memory
{
public:
    Memory();
    Memory(uint32_t size);
    void resize(uint32_t size);
    uint32_t read(uint32_t index);
    bool insert(uint32_t index, uint32_t data, bool enable);
    bool insert(uint32_t data, bool enable);
    
private:
    vector <uint32_t> mem_array;
};

#endif /* Memory_hpp */
