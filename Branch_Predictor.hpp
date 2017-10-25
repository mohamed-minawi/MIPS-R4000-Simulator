//
//  Branch_Predictor.hpp
//  MIPS_Pipelined_Simulator
//
//  Created by Mohamed A Tawfik on Apr/21/17.
//  Copyright © 2017 Mohamed A Tawfik. All rights reserved.
//

#ifndef Branch_Predictor_hpp
#define Branch_Predictor_hpp

#include <stdint.h>
#include <vector>

class Branch_Predictor{
public:
    enum Decision{Definitely_NO, NO, YES, Definitely_YES};
    
    Branch_Predictor();
    Branch_Predictor(const Branch_Predictor&);
    Branch_Predictor(Branch_Predictor&&);
    
    ~Branch_Predictor();
    
    // Adds a new branch instruction, via its original pc and target pc, to the table. 
    void addIfNotPresent(uint32_t pc, uint32_t target_pc);
    // Gets a branch decision based on current state.
    bool shouldBranch(uint32_t pc) const;
    // Gets the target pc
    uint32_t getSpeculatedAddress(uint32_t pc) const;
    // Gets a detailed branch decision based on current state.
    Decision getDecision(uint32_t pc) const;
    // Updates the branch decision based on new information
    bool updateDecision(uint32_t pc, bool taken);
    // Checks if this instruction is an identified branch instruction that exists in the table.
    bool exists(uint32_t pc) const;
    // Clears the table
    void clear();
    
private:
    struct line{
        uint32_t from_address;
        uint32_t target_address;
        Decision decision;
    };
    
    std::vector<line> Table;
};

#endif /* Branch_Predictor_hpp */
