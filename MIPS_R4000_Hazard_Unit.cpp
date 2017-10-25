//
//  MIPS_R4000_Hazard_Unit.cpp
//  MIPS_Pipelined_Simulator
//
//  Created by Mohamed A Tawfik on Apr/21/17.
//  Copyright © 2017 Mohamed A Tawfik. All rights reserved.
//

#include "MIPS_R4000_Hazard_Unit.hpp"

MIPS_R4000_Hazard_Unit::MIPS_R4000_Hazard_Unit(){
}
MIPS_R4000_Hazard_Unit::MIPS_R4000_Hazard_Unit(const MIPS_R4000_Hazard_Unit& other):Branch_Predictor(other.Branch_Predictor){
}
MIPS_R4000_Hazard_Unit::MIPS_R4000_Hazard_Unit(MIPS_R4000_Hazard_Unit&& other):Branch_Predictor(other.Branch_Predictor){
}

MIPS_R4000_Hazard_Unit::~MIPS_R4000_Hazard_Unit(){
}
/*
// Adds a new branch instruction, via its original pc and target pc, to the table.
void MIPS_R4000_Hazard_Unit::addToBranchTableIfNotPresent(uint32_t pc, uint32_t target_pc){
    Branch_Predictor.addIfNotPresent(pc, target_pc);
}
// Gets a branch decision based on current state.
bool MIPS_R4000_Hazard_Unit::branch(uint32_t pc) const{
    return Branch_Predictor.branch(pc);
}
// Gets a detailed branch decision based on current state.
Branch_Predictor::Decision MIPS_R4000_Hazard_Unit::getBranchDecision(uint32_t pc) const{
    return Branch_Predictor.getDecision(pc);
}
// Updates the branch decision based on new information
bool MIPS_R4000_Hazard_Unit::updateBranchDecision(uint32_t pc, bool flag){
    return Branch_Predictor.updateDecision(pc, flag);
}
// Checks if this instruction is an identified branch instruction that exists in the table.
bool MIPS_R4000_Hazard_Unit::exitsInBranchTable(uint32_t pc) const{
    return Branch_Predictor.exists(pc);
}
 */

MIPS_R4000_Forwarding_Unit::Final_Source MIPS_R4000_Hazard_Unit::getForwardingDecision(uint32_t rs_EX, uint32_t rd_DF, bool reg_we_DF, uint32_t rd_DS, bool reg_we_DS, uint32_t rd_TC, bool reg_we_TC, uint32_t rt_TC, bool mem_to_reg_TC){
    return MIPS_R4000_Forwarding_Unit::getDecision(rs_EX, rd_DF, reg_we_DF, rd_DS, reg_we_DS, rd_TC, reg_we_TC, rt_TC, mem_to_reg_TC);
}

bool MIPS_R4000_Hazard_Unit::getStallingDecision(uint32_t rs1_EX, uint32_t rs2_EX, uint32_t rs2_DF, bool memtoreg_DF, uint32_t rs2_DS, bool memtoreg_DS){
    return MIPS_R4000_Stalling_Unit::getDecision(rs1_EX, rs2_EX, rs2_DF, memtoreg_DF, rs2_DS, memtoreg_DS);
}
