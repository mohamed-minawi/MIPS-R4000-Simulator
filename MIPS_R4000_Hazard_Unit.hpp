//
//  MIPS_R4000_Hazard_Unit.hpp
//  MIPS_Pipelined_Simulator
//
//  Created by Mohamed A Tawfik on Apr/21/17.
//  Copyright © 2017 Mohamed A Tawfik. All rights reserved.
//

#ifndef MIPS_R4000_Hazard_Unit_hpp
#define MIPS_R4000_Hazard_Unit_hpp

#include <stdint.h>
#include "Branch_Predictor.hpp"
#include "MIPS_R4000_Forwarding_Unit.hpp"
#include "MIPS_R4000_Stalling_Unit.hpp"

/*
class MIPS_R4000_Hazard_Unit : public Branch_Predictor, public MIPS_R4000_Stalling_Unit, public MIPS_R4000_Forwarding_Unit{
};
 */

class MIPS_R4000_Hazard_Unit{
public:
    MIPS_R4000_Hazard_Unit();
    MIPS_R4000_Hazard_Unit(const MIPS_R4000_Hazard_Unit&);
    MIPS_R4000_Hazard_Unit(MIPS_R4000_Hazard_Unit&&);
    
    ~MIPS_R4000_Hazard_Unit();
    
    /*
    // Adds a new branch instruction, via its original pc and target pc, to the table.
    void addToBranchTableIfNotPresent(uint32_t pc, uint32_t target_pc);
    // Gets a branch decision based on current state.
    bool branch(uint32_t pc) const;
    // Gets a detailed branch decision based on current state.
    Branch_Predictor::Decision getBranchDecision(uint32_t pc) const;
    // Updates the branch decision based on new information.
    bool updateBranchDecision(uint32_t pc, bool flag);
    // Checks if this instruction is an identified branch instruction that exists in the table.
    bool exitsInBranchTable(uint32_t pc) const;
     */
    
    // Gets a forwarding decision for a specific rs.
    static MIPS_R4000_Forwarding_Unit::Final_Source getForwardingDecision(uint32_t rs_EX, uint32_t rd_DF, bool reg_we_DF, uint32_t rd_DS, bool reg_we_DS, uint32_t rd_TC, bool reg_we_TC, uint32_t rt_TC, bool mem_to_reg_TC);
    
    // Gets a stalling decision.
    static bool getStallingDecision(uint32_t rs1_EX, uint32_t rs2_EX, uint32_t rs2_DF, bool memtoreg_DF, uint32_t rs2_DS, bool memtoreg_DS);

    Branch_Predictor Branch_Predictor;
};

#endif /* MIPS_R4000_Hazard_Unit_hpp */
