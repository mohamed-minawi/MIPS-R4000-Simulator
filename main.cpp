//
//  main.cpp
//  MIPS_Pipelined_Simulator
//
//  Created by Mohamed A Tawfik on Mar/17/17.
//  Copyright © 2017 Mohamed A Tawfik. All rights reserved.
//

#include "MIPS_Assembler.hpp"
#include "MIPS_R4000_Hazard_Unit.hpp"
#include "Register.hpp"
#include "Pipeline_Buffers_Handler.hpp"
#include "Memory.hpp"
#include "MIPS_Decoder.hpp"
#include "MIPS_Control_Unit.hpp"
#include "RegisterFile.hpp"
#include "ALU.hpp"
#include "Buffers_Indices.h"
#include "pseudoconverter.hpp"

#include <iostream>
#include <iomanip>
#include <limits>
#include <stack>

#define PC_index 0
#define IS_index 1
#define RF_index 2
#define EX_index 3
#define DF_index 4
#define DS_index 5
#define TC_index 6
#define WB_index 7

const std::string Stages[8] = {"Program Counter", "IS Buffer", "RF Buffer", "EX Buffer", "DF Buffer", "DS Buffer", "TC Buffer", "WB Buffer"};

void testAssembler();

void topLevel();
void fillInstructions(Memory& I_cache);
void prepareSimulation(Memory& I_cache, Memory& D_cache, RegisterFile& register_file, MIPS_R4000_Hazard_Unit& hazard_unit, std::vector<Pipeline_Buffers_Handler::Register_Logic>& Pipelined_Registers, std::stack<uint32_t>& procedure_stack);
void nextCycle(Memory& I_cache, Memory& D_cache, RegisterFile& register_file, MIPS_R4000_Hazard_Unit& hazard_unit, std::vector<Pipeline_Buffers_Handler::Register_Logic>& Pipelined_Registers, bool& reset_branch, bool& should_stall, bool& reset_jump, std::stack<uint32_t>& procedure_stack);
uint32_t checkRealBranchDecision(bool branch, bool branch_on_equal, uint32_t RS_contents, uint32_t RT_contents);
void displayHeader();

void Wait();

void displayDebuggingData(std::vector<Pipeline_Buffers_Handler::Register_Logic>& Pipelined_Registers, bool& reset_branch, bool& should_stall, int i);

void printPC(std::vector<Pipeline_Buffers_Handler::Register_Logic>& Pipelined_Registers, int index);
void printInstruction(std::vector<Pipeline_Buffers_Handler::Register_Logic>& Pipelined_Registers, int index);
void printAddressesAndImmediate(std::vector<Pipeline_Buffers_Handler::Register_Logic>& Pipelined_Registers, int index);
void printCorrectBranchDecision(std::vector<Pipeline_Buffers_Handler::Register_Logic>& Pipelined_Registers, int index);
void printALUresult(std::vector<Pipeline_Buffers_Handler::Register_Logic>& Pipelined_Registers, int index);
void printMemory(std::vector<Pipeline_Buffers_Handler::Register_Logic>& Pipelined_Registers, int index);


int main(){
    topLevel();
    
    return 0;
}

void testAssembler(){
    std::string expr;
    uint32_t machine;
    
    getline(std::cin, expr);
    
    while (expr != "Terminate")
    {
        try {
            machine = MIPS_Assembler::exprToMachine(expr);
            std::cout << "0x" << std::hex << std::setfill('0') << std::setw(8);
            std::cout << machine << std::endl;
        }
        catch(std::invalid_argument& ia){
            std::cout << "Error: " << ia.what() << std::endl;
        }
        catch(...){
            std::cout << "Unidentified error" << std::endl;
            std::abort();
        }
        
        getline(std::cin, expr);
    }
}

void fillInstructions(Memory& I_cache){
    int n;
    std::cin >> n;
    if(n < 1)
        return;
    
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    I_cache.resize(0);
    
    std::string expr;
    uint32_t machine;
    vector<string> temp;
    for(int i = 0; i < n; i++){
        temp.clear();
        getline(std::cin, expr);
        try{
            temp.push_back(expr);
            PSEUDOCONVERTER::convert_to_true(temp);
            
            for (int j = 0; j < temp.size(); j++){
                machine = MIPS_Assembler::exprToMachine(temp[j]);
                I_cache.insert(machine, true);
            }
        }
        catch(std::invalid_argument& ia){
            std::cout << "Error: " << ia.what() << std::endl;
            i--;
        }
        catch(...){
            std::cout << "Unidentified error" << std::endl;
            i--;
            
        }
    }
}

void topLevel(){
    
    // NOTE : declare as global or in private of a class
    
    // declare instruction memory
    Memory I_cache;
    // declare data memory
    Memory D_cache;
    // declare register file
    RegisterFile register_file;
    // declare hazard unit
    MIPS_R4000_Hazard_Unit hazard_unit;
    // declare pipeline buffers
    std::vector<Pipeline_Buffers_Handler::Register_Logic> Pipelined_Registers;
    // declare procedure stack
    std::stack<uint32_t> procedure_stack;
    
    prepareSimulation(I_cache, D_cache, register_file, hazard_unit, Pipelined_Registers, procedure_stack);
    
    // loop termination logic
    int i(0);
    
    bool reset_branch(false), should_stall(false), reset_jump(false);
    
    while(i < 100){
        nextCycle(I_cache, D_cache, register_file, hazard_unit, Pipelined_Registers, reset_branch, should_stall, reset_jump, procedure_stack);
        
        // display data / debugging results
        /*
         std::cout << "0x" << std::hex << std::setfill('0') << std::setw(8);
         std::cout <<  Pipelined_Registers[IS_index].output[INDEX_pc] << ": ";
         std::cout << "0x" << std::hex << std::setfill('0') << std::setw(8);
         std::cout << Pipelined_Registers[IS_index].output[INDEX_instr_word] << std::endl;
         */
        
        displayDebuggingData(Pipelined_Registers, reset_branch, should_stall, i);
        
        Wait();
        
        // next iteration
        i++;
    }
}

void prepareSimulation(Memory& I_cache, Memory& D_cache, RegisterFile& register_file, MIPS_R4000_Hazard_Unit& hazard_unit, std::vector<Pipeline_Buffers_Handler::Register_Logic>& Pipelined_Registers, std::stack<uint32_t>& procedure_stack){
    // load the I_cache with the instructions
    I_cache.resize(0);
    fillInstructions(I_cache);
    
    // empty and resize the D_cache
    D_cache.resize(0);
    D_cache.resize(16);
    
    // empty and resize the register_file
    register_file.resizeAndReset(16);
    
    // empty the BTB
    hazard_unit.Branch_Predictor.clear();
    
    // empty and resize the pipeline buffers
    Pipelined_Registers.resize(0);
    Pipelined_Registers.resize(8);
    
    Pipeline_Buffers_Handler::enableBuffers(Pipelined_Registers, PC_index, WB_index, true);
    
    // resize outer wires to correct size
    Pipeline_Buffers_Handler::resizeBuffers(Pipelined_Registers, PC_index, WB_index, 0);
    
    Pipeline_Buffers_Handler::resizeBuffers(Pipelined_Registers, PC_index, PC_index, 2);
    Pipeline_Buffers_Handler::resizeBuffers(Pipelined_Registers, IS_index, RF_index, 3);
    Pipeline_Buffers_Handler::resizeBuffers(Pipelined_Registers, EX_index, EX_index, 22);
    Pipeline_Buffers_Handler::resizeBuffers(Pipelined_Registers, DF_index, DS_index, 24);
    Pipeline_Buffers_Handler::resizeBuffers(Pipelined_Registers, TC_index, WB_index, 25);
    
    // resize internal buffers to correct size
    Pipeline_Buffers_Handler::resetBuffers(Pipelined_Registers, PC_index, WB_index, false);
    Pipeline_Buffers_Handler::assignInputBuffers(Pipelined_Registers, PC_index, WB_index);
    Pipeline_Buffers_Handler::updateBuffers(Pipelined_Registers, PC_index, WB_index);
    
    // reset all buffers
    Pipeline_Buffers_Handler::resetBuffers(Pipelined_Registers, PC_index, WB_index, true);
    Pipeline_Buffers_Handler::updateBuffers(Pipelined_Registers, PC_index, WB_index);
    
    // prepare buffers for simulation
    Pipeline_Buffers_Handler::resetBuffers(Pipelined_Registers, PC_index, WB_index, false);
    Pipeline_Buffers_Handler::updateBuffers(Pipelined_Registers, PC_index, WB_index);
    
    while(!procedure_stack.empty())
        procedure_stack.pop();
}

void nextCycle(Memory& I_cache, Memory& D_cache, RegisterFile& register_file, MIPS_R4000_Hazard_Unit& hazard_unit, std::vector<Pipeline_Buffers_Handler::Register_Logic>& Pipelined_Registers, bool& reset_branch, bool& should_stall, bool& reset_jump, std::stack<uint32_t>& procedure_stack){
    
    // read data from pipeline buffers
    Pipeline_Buffers_Handler::readDataBuffers(Pipelined_Registers, PC_index, WB_index);
    
    // set flags to false
    reset_branch = should_stall = reset_jump = false;
    
    // detect load hazard
    should_stall = hazard_unit.getStallingDecision(Pipelined_Registers[EX_index].output[INDEX_RS], Pipelined_Registers[EX_index].output[INDEX_RT], Pipelined_Registers[DF_index].output[INDEX_RT], Pipelined_Registers[DF_index].output[INDEX_mem_to_reg], Pipelined_Registers[DS_index].output[INDEX_RT], Pipelined_Registers[DS_index].output[INDEX_mem_to_reg]);
    
    // ****** IF stage ******
    
    // get branch predictions on program counter
    Pipelined_Registers[PC_index].input[INDEX_branch_prediction] = hazard_unit.Branch_Predictor.shouldBranch(Pipelined_Registers[PC_index].output[INDEX_pc]);
    
    // decide next address of program counter
    if(Pipelined_Registers[PC_index].input[INDEX_branch_prediction])
        Pipelined_Registers[PC_index].input[INDEX_pc] = hazard_unit.Branch_Predictor.getSpeculatedAddress(Pipelined_Registers[PC_index].output[INDEX_pc]);
    else
        Pipelined_Registers[PC_index].input[INDEX_pc] = Pipelined_Registers[PC_index].output[INDEX_pc] + 4;
    
    // copy forward to IS_buffer from PC_register
    Pipeline_Buffers_Handler::copyForward(Pipelined_Registers[IS_index], Pipelined_Registers[PC_index]);
    
    // read next instruction
    Pipelined_Registers[IS_index].input[INDEX_instr_word] = I_cache.read(Pipelined_Registers[PC_index].output[INDEX_pc] >> 2);
    
    // ****** IS stage ******
    
    // copy forward to RF_buffer from IS_buffer
    Pipeline_Buffers_Handler::copyForward(Pipelined_Registers[RF_index], Pipelined_Registers[IS_index]);
    
    // ****** RF stage ******
    
    // copy forward to EX_buffer from RF_buffer
    Pipeline_Buffers_Handler::copyForward(Pipelined_Registers[EX_index], Pipelined_Registers[RF_index]);
    
    // decode current instruction
    MIPS_Decoder::decodeInstruction(Pipelined_Registers[RF_index].output[INDEX_instr_word], Pipelined_Registers[EX_index].input[INDEX_opcode], Pipelined_Registers[EX_index].input[INDEX_funct], Pipelined_Registers[EX_index].input[INDEX_RS], Pipelined_Registers[EX_index].input[INDEX_RT], Pipelined_Registers[EX_index].input[INDEX_RD], Pipelined_Registers[EX_index].input[INDEX_immi_ext]);
    
    // get control signals of decoded instruction
    MIPS_Control_Unit::getControlSignals(Pipelined_Registers[EX_index].input[INDEX_opcode], Pipelined_Registers[EX_index].input[INDEX_funct], Pipelined_Registers[EX_index].input[INDEX_mem_to_reg], Pipelined_Registers[EX_index].input[INDEX_mem_write], Pipelined_Registers[EX_index].input[INDEX_ALU_src], Pipelined_Registers[EX_index].input[INDEX_reg_dst], Pipelined_Registers[EX_index].input[INDEX_reg_write], Pipelined_Registers[EX_index].input[INDEX_branch], Pipelined_Registers[EX_index].input[INDEX_branch_on_equal], Pipelined_Registers[EX_index].input[INDEX_jump], Pipelined_Registers[EX_index].input[INDEX_link_on_jumping], Pipelined_Registers[EX_index].input[INDEX_stack_jumping], Pipelined_Registers[EX_index].input[INDEX_ALU_ctrl]);
    
    // (** WB stage **) write back data to register file
    uint32_t write_data = Pipelined_Registers[WB_index].output[INDEX_mem_to_reg] ? Pipelined_Registers[WB_index].output[INDEX_mem_data] : Pipelined_Registers[WB_index].output[INDEX_ALU_result];
    uint32_t write_address = Pipelined_Registers[WB_index].output[INDEX_ALU_src] ? Pipelined_Registers[WB_index].output[INDEX_RT] : Pipelined_Registers[WB_index].output[INDEX_RD];
    
    register_file.write(write_address, write_data, Pipelined_Registers[WB_index].output[INDEX_reg_write]);
    
    // read data from register file
    Pipelined_Registers[EX_index].input[INDEX_RS_data] = register_file.read(Pipelined_Registers[EX_index].input[INDEX_RS]);
    Pipelined_Registers[EX_index].input[INDEX_RT_data] = register_file.read(Pipelined_Registers[EX_index].input[INDEX_RT]);
    
    // adjust for jump instruction
    if(Pipelined_Registers[EX_index].input[INDEX_jump]){
        reset_jump = true;
        if(Pipelined_Registers[EX_index].input[INDEX_link_on_jumping]){
            Pipelined_Registers[PC_index].input[INDEX_pc] = (((Pipelined_Registers[EX_index].input[INDEX_instr_word] << 2) & 0x0FFFFFFF) | (Pipelined_Registers[EX_index].input[INDEX_pc] & 0xF0000000)) + 4;
            procedure_stack.push(Pipelined_Registers[EX_index].input[INDEX_pc] + 4);
        }
        else if(Pipelined_Registers[EX_index].input[INDEX_stack_jumping]){
            Pipelined_Registers[PC_index].input[INDEX_pc] = procedure_stack.top();
            procedure_stack.pop();
        }
        else{
            Pipelined_Registers[PC_index].input[INDEX_pc] = (((Pipelined_Registers[EX_index].input[INDEX_instr_word] << 2) & 0x0FFFFFFF) | (Pipelined_Registers[EX_index].input[INDEX_pc] & 0xF0000000)) + 4;
        }
    }
    
    // ****** EX stage ******
    
    // copy forward to DF_buffer from EX_buffer
    Pipeline_Buffers_Handler::copyForward(Pipelined_Registers[DF_index], Pipelined_Registers[EX_index]);
    
    uint32_t reg_addr_rs[3];
    for(int i = 0; i < 3; i++)
        reg_addr_rs[i] = Pipelined_Registers[DF_index + i].output[INDEX_ALU_src] ? Pipelined_Registers[DF_index + i].output[INDEX_RT] : Pipelined_Registers[DF_index + i].output[INDEX_RD];
    
    MIPS_R4000_Forwarding_Unit::Final_Source RS_source = hazard_unit.getForwardingDecision(Pipelined_Registers[EX_index].output[INDEX_RS], reg_addr_rs[0], Pipelined_Registers[DF_index].output[INDEX_reg_write], reg_addr_rs[1], Pipelined_Registers[DS_index].output[INDEX_reg_write], reg_addr_rs[2], Pipelined_Registers[TC_index].output[INDEX_reg_write], Pipelined_Registers[TC_index].output[INDEX_RT], Pipelined_Registers[TC_index].output[INDEX_mem_to_reg]);
    
    switch(RS_source){
        case MIPS_R4000_Forwarding_Unit::Final_Source::EX:
            Pipelined_Registers[DF_index].input[INDEX_RS_data] = Pipelined_Registers[EX_index].output[INDEX_RS_data];
            break;
        case MIPS_R4000_Forwarding_Unit::Final_Source::DF:
            Pipelined_Registers[DF_index].input[INDEX_RS_data] = Pipelined_Registers[DF_index].output[INDEX_ALU_result];
            break;
        case MIPS_R4000_Forwarding_Unit::Final_Source::DS:
            Pipelined_Registers[DF_index].input[INDEX_RS_data] = Pipelined_Registers[DS_index].output[INDEX_ALU_result];
            break;
        case MIPS_R4000_Forwarding_Unit::Final_Source::TC_mem:
            Pipelined_Registers[DF_index].input[INDEX_RS_data] = Pipelined_Registers[TC_index].output[INDEX_mem_data];
            break;
        case MIPS_R4000_Forwarding_Unit::Final_Source::TC_alu:
            Pipelined_Registers[DF_index].input[INDEX_RS_data] = Pipelined_Registers[TC_index].output[INDEX_ALU_result];
            break;
    }
    
    uint32_t reg_addr_rt[3];
    for(int i = 0; i < 3; i++)
        reg_addr_rt[i] = Pipelined_Registers[DF_index + i].output[INDEX_ALU_src] ? Pipelined_Registers[DF_index + i].output[INDEX_RT] : Pipelined_Registers[DF_index + i].output[INDEX_RD];
    
    MIPS_R4000_Forwarding_Unit::Final_Source RT_source = hazard_unit.getForwardingDecision(Pipelined_Registers[EX_index].output[INDEX_RT], reg_addr_rt[0], Pipelined_Registers[DF_index].output[INDEX_reg_write], reg_addr_rt[1], Pipelined_Registers[DS_index].output[INDEX_reg_write], reg_addr_rt[2], Pipelined_Registers[TC_index].output[INDEX_reg_write], Pipelined_Registers[TC_index].output[INDEX_RT], Pipelined_Registers[TC_index].output[INDEX_mem_to_reg]);
    
    switch(RT_source){
        case MIPS_R4000_Forwarding_Unit::Final_Source::EX:
            Pipelined_Registers[DF_index].input[INDEX_RT_data] = Pipelined_Registers[EX_index].output[INDEX_RS_data];
            break;
        case MIPS_R4000_Forwarding_Unit::Final_Source::DF:
            Pipelined_Registers[DF_index].input[INDEX_RT_data] = Pipelined_Registers[DF_index].output[INDEX_ALU_result];
            break;
        case MIPS_R4000_Forwarding_Unit::Final_Source::DS:
            Pipelined_Registers[DF_index].input[INDEX_RT_data] = Pipelined_Registers[DS_index].output[INDEX_ALU_result];
            break;
        case MIPS_R4000_Forwarding_Unit::Final_Source::TC_mem:
            Pipelined_Registers[DF_index].input[INDEX_RT_data] = Pipelined_Registers[TC_index].output[INDEX_mem_data];
            break;
        case MIPS_R4000_Forwarding_Unit::Final_Source::TC_alu:
            Pipelined_Registers[DF_index].input[INDEX_RT_data] = Pipelined_Registers[TC_index].output[INDEX_ALU_result];
            break;
    }
    
    // get correct branch decision
    Pipelined_Registers[DF_index].input[INDEX_correct_branch_decision] = checkRealBranchDecision(Pipelined_Registers[EX_index].output[INDEX_branch], Pipelined_Registers[EX_index].output[INDEX_branch_on_equal], Pipelined_Registers[DF_index].input[INDEX_RS_data], Pipelined_Registers[DF_index].input[INDEX_RT_data]);
    
    // check if this is a branch instruction and add it to the table
    if((Pipelined_Registers[EX_index].output[INDEX_branch]))
        hazard_unit.Branch_Predictor.addIfNotPresent(Pipelined_Registers[EX_index].output[INDEX_pc], Pipelined_Registers[EX_index].output[INDEX_pc] + 4 + (Pipelined_Registers[EX_index].output[INDEX_immi_ext] << 2));
    
    // update branch table
    hazard_unit.Branch_Predictor.updateDecision(Pipelined_Registers[EX_index].output[INDEX_pc], Pipelined_Registers[DF_index].input[INDEX_correct_branch_decision]);
    
    // deal with incorrect branch taken hazard
    if (Pipelined_Registers[EX_index].output[INDEX_branch_prediction] && !Pipelined_Registers[DF_index].input[INDEX_correct_branch_decision]){
        Pipelined_Registers[PC_index].input[INDEX_pc] = Pipelined_Registers[EX_index].output[INDEX_pc] + 4;
        // Pipelined_Registers[PC_index].input[INDEX_branch_prediction] = 0;
        reset_branch = true;
    }
    
    // deal with incorrect branch not taken hazard
    if (!Pipelined_Registers[EX_index].output[INDEX_branch_prediction] && Pipelined_Registers[DF_index].input[INDEX_correct_branch_decision]){
        Pipelined_Registers[PC_index].input[INDEX_pc] = Pipelined_Registers[EX_index].output[INDEX_pc] + 4 + (Pipelined_Registers[EX_index].output[INDEX_immi_ext] << 2);
        // Pipelined_Registers[PC_index].input[INDEX_branch_prediction] = 1;
        reset_branch = true;
    }
    
    // operand 2 input logic
    uint32_t ALU_operand2 = Pipelined_Registers[EX_index].output[INDEX_ALU_src] ? Pipelined_Registers[EX_index].output[INDEX_immi_ext] : Pipelined_Registers[DF_index].input[INDEX_RT_data];
    
    uint32_t shamt = (Pipelined_Registers[EX_index].output[INDEX_immi_ext] >> 6) & 0x1F;
    
    // ALU module
    ALU::ALUResult(Pipelined_Registers[DF_index].input[INDEX_RS_data], ALU_operand2, Pipelined_Registers[EX_index].output[INDEX_ALU_ctrl], shamt, Pipelined_Registers[DF_index].input[INDEX_ALU_result]);
    
    // **** DF stage ****
    
    // copy forward to DS_buffer from DF_buffer
    Pipeline_Buffers_Handler::copyForward(Pipelined_Registers[DS_index], Pipelined_Registers[DF_index]);
    
    // **** DS stage ****
    
    // copy forward to TC_buffer from DS_buffer
    Pipeline_Buffers_Handler::copyForward(Pipelined_Registers[TC_index], Pipelined_Registers[DS_index]);
    
    // write data to data cache
    D_cache.insert(Pipelined_Registers[DS_index].output[INDEX_ALU_result] >> 2, Pipelined_Registers[DS_index].output[INDEX_RT_data], Pipelined_Registers[DS_index].output[INDEX_mem_write]);
    
    // read data from data cache
    Pipelined_Registers[TC_index].input[INDEX_mem_data] = D_cache.read(Pipelined_Registers[DS_index].output[INDEX_ALU_result] >> 2);
    
    // **** TC stage ****
    
    // copy forward to WB_buffer from DT_buffer
    Pipeline_Buffers_Handler::copyForward(Pipelined_Registers[WB_index], Pipelined_Registers[TC_index]);
    
    // **** WB stage ****
    
    // see above (in RF_stage)
    
    // update pipeline buffers
    
    // stall PC register, IS_buffer, RF_buffer, EX_buffer - if needed
    Pipeline_Buffers_Handler::enableBuffers(Pipelined_Registers, PC_index, EX_index, !should_stall);
    // flush IS and RF buffers - if needed
    Pipeline_Buffers_Handler::resetBuffers(Pipelined_Registers, IS_index, RF_index, reset_branch || reset_jump);
    // flush EX_buffer - if needed
    Pipeline_Buffers_Handler::resetBuffers(Pipelined_Registers, EX_index, EX_index, reset_branch);
    // flush DF_buffer - if needed
    Pipeline_Buffers_Handler::resetBuffers(Pipelined_Registers, DF_index, DF_index, should_stall);
    
    Pipeline_Buffers_Handler::assignInputBuffers(Pipelined_Registers, PC_index, WB_index);
    Pipeline_Buffers_Handler::updateBuffers(Pipelined_Registers, PC_index, WB_index);
    
    // set reset to false and enable to true to prepare for next cycle
    Pipeline_Buffers_Handler::resetBuffers(Pipelined_Registers, PC_index, WB_index, false);
    Pipeline_Buffers_Handler::enableBuffers(Pipelined_Registers, PC_index, WB_index, true);
}

uint32_t checkRealBranchDecision(bool branch, bool branch_on_equal, uint32_t RS_contents, uint32_t RT_contents){
    if(!branch)
        return 0;
    if(branch_on_equal)
        return RS_contents == RT_contents;
    else
        return RS_contents != RT_contents;
}

void Wait(){
    char x;
    cin.get(x);
}

void displayDebuggingData(std::vector<Pipeline_Buffers_Handler::Register_Logic>& Pipelined_Registers, bool& reset_branch, bool& should_stall, int i){
    
    displayHeader();
    std::cout << std::endl <<"Results after clock cycle: " << i << std::endl << std::endl;
    
    for(int i = 0; i < 8; i++){
        std::cout << Stages[i] << std::endl;
        printPC(Pipelined_Registers, i);
        std::cout << std::endl;
        if(i > 0){
            printInstruction(Pipelined_Registers, i);
            std::cout << std::endl;
        }
        if(i > 2){
            printAddressesAndImmediate(Pipelined_Registers, i);
            std::cout << std::endl;
        }
        if(i > 3){
            printCorrectBranchDecision(Pipelined_Registers, i);
            std::cout << std::endl;
            printALUresult(Pipelined_Registers, i);
            std::cout << std::endl;
        }
        if(i > 5){
            printMemory(Pipelined_Registers, i);
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}

void displayHeader(){
    for(int i = 0; i < 40; i++)
        std::cout << '*';
    std::cout << std::endl;
}

void printInstruction(std::vector<Pipeline_Buffers_Handler::Register_Logic>& Pipelined_Registers, int index){
    std::cout << "Read instruction: " << "0x" << std::hex << std::setfill('0') << std::setw(8) << Pipelined_Registers[index].output[INDEX_instr_word];
}
void printPC(std::vector<Pipeline_Buffers_Handler::Register_Logic>& Pipelined_Registers, int index){
    std::cout << std::dec;
    std::cout << "PC value: " << Pipelined_Registers[index].output[PC_index] << " Was branched: " << Pipelined_Registers[index].output[INDEX_branch_prediction];
}
void printAddressesAndImmediate(std::vector<Pipeline_Buffers_Handler::Register_Logic>& Pipelined_Registers, int index){
    std::cout << std::dec;
    std::cout << "RS_addr: " << Pipelined_Registers[index].output[INDEX_RS];
    std::cout << " RT_addr: " << Pipelined_Registers[index].output[INDEX_RT];
    std::cout << " RD_addr: " << Pipelined_Registers[index].output[INDEX_RD];
    std::cout << " Immi: " << (Pipelined_Registers[index].output[INDEX_immi_ext]);
}

void printCorrectBranchDecision(std::vector<Pipeline_Buffers_Handler::Register_Logic>& Pipelined_Registers, int index){
    std::cout << "Is branch? " << Pipelined_Registers[index].output[INDEX_branch];
    std::cout << " On Equal ? " << Pipelined_Registers[index].output[INDEX_branch_on_equal];
    
    std::cout << " Correct branch decision: " << Pipelined_Registers[index].output[INDEX_correct_branch_decision];
}
void printALUresult(std::vector<Pipeline_Buffers_Handler::Register_Logic>& Pipelined_Registers, int index){
    std::cout << std::dec;
    std::cout << "ALU result: " << Pipelined_Registers[index].output[INDEX_ALU_result];
}
void printMemory(std::vector<Pipeline_Buffers_Handler::Register_Logic>& Pipelined_Registers, int index){
    std::cout << std::dec;
    std::cout << "mem_write: " << Pipelined_Registers[index].output[INDEX_mem_write];
    std::cout << " Memory write data: " << Pipelined_Registers[index].output[INDEX_RT_data] << std::endl;
    std::cout << "mem_to_reg: " << Pipelined_Registers[index].output[INDEX_mem_to_reg];
    std::cout << " Memory read data: " << Pipelined_Registers[index].output[INDEX_mem_data];
}


