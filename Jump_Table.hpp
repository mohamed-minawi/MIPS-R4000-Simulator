//
//  Jump_Table.hpp
//  MIPS_Pipelined_Simulator
//
//  Created by Mohamed A Tawfik on May/15/17.
//  Copyright © 2017 Mohamed A Tawfik. All rights reserved.
//

#ifndef Jump_Table_hpp
#define Jump_Table_hpp

#include <vector>
#include <stdint.h>

class Jump_Table{
    
    
private:
    struct line{
        uint32_t from_address;
        uint32_t target_address;
    };
    
    std::vector<line> Table;
    
public:
    
    Jump_Table();
    Jump_Table(const Jump_Table&);
    Jump_Table(Jump_Table&&);
    
    ~Jump_Table();
    
    // Adds a new branch instruction, via its original pc and target pc, to the table.
    void addIfNotPresent(uint32_t pc, uint32_t target_pc);
    // Gets the target pc
    uint32_t getSpeculatedAddress(uint32_t pc) const;
    // Checks if this instruction is an identified branch instruction that exists in the table.
    bool exists(uint32_t pc) const;
    // Clears the table
    void clear();

    
};

#endif /* Jump_Table_hpp */
