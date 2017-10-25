//
//  MIPS_Control_Unit.cpp
//  MIPS_Pipelined_Simulator
//
//  Created by Mohamed A Tawfik on May/7/17.
//  Copyright © 2017 Mohamed A Tawfik. All rights reserved.
//

#include "MIPS_Control_Unit.hpp"
#include "Macros.h"
#include <iostream>

void MIPS_Control_Unit::getControlSignals(uint32_t OPCODE, uint32_t function, uint32_t& MemToReg, uint32_t& MemWrite, uint32_t& ALUSrc, uint32_t& RegDst, uint32_t& RegWrite, uint32_t& Branch, uint32_t& Branch_on_equal, uint32_t& Jump, uint32_t& Link_on_Jumping, uint32_t& Stack_Jumping, uint32_t& ALUcontrol){
    
    switch (OPCODE){
        case R_OPCODE:
            switch (function){
                case ADD_FUNC:
                case ADDU_FUNC: ALUcontrol = ADD;  break;
                case SUB_FUNC:
                case SUBU_FUNC: ALUcontrol = SUB;  break;
                case AND_FUNC:  ALUcontrol = AND;  break;
                case NOR_FUNC:  ALUcontrol = NOR;  break;
                case OR_FUNC:   ALUcontrol = OR;   break;
                case SLL_FUNC:  ALUcontrol = SLL;  break;
                case SLLV_FUNC: ALUcontrol = SLLV; break;
                case SLT_FUNC:
                case SLTU_FUNC: ALUcontrol = SLT;  break;
                case SRA_FUNC:  ALUcontrol = SRA;  break;
                case SRAV_FUNC: ALUcontrol = SRAV; break;
                case SRL_FUNC: ALUcontrol = SRL; break;
                case SRLV_FUNC: ALUcontrol = SRLV; break;
                case XOR_FUNC: ALUcontrol = XOR; break;
                case JR_FUNC:
                case RPR_FUNC: ALUcontrol = ADD; break;
            }
            RegWrite = RegDst = true;
            ALUSrc = Branch = MemWrite = MemToReg = Jump = Branch_on_equal = Link_on_Jumping = Stack_Jumping = false;
            break;
            
        case LW_OP:
            ALUcontrol = ADD;
            RegWrite = ALUSrc = MemToReg = true;
            RegDst = Branch = MemWrite = Jump = Branch_on_equal = Link_on_Jumping = Stack_Jumping = false;
            break;
            
        case SW_OP:
            ALUcontrol = ADD;
            ALUSrc = MemWrite = true;
            RegWrite = RegDst = Branch = Jump = MemToReg = Branch_on_equal = Link_on_Jumping = Stack_Jumping= false;
            break;
            
        case BEQ_OP:
            ALUcontrol = SUB;
            Branch = Branch_on_equal = true;
            RegWrite = RegDst = ALUSrc = MemWrite = MemToReg = Jump = Link_on_Jumping =Stack_Jumping = false;
            break;
            
        case BNE_OP:
            ALUcontrol = SUB;
            Branch = true;
            RegWrite = RegDst = Branch_on_equal = ALUSrc = MemWrite = MemToReg = Jump = Link_on_Jumping = Stack_Jumping = false;
            break;
            
        case ADDI_OP:
        case ADDIU_OP:
            ALUcontrol = ADD;
            RegWrite = ALUSrc = true;
            RegDst = Branch = MemWrite = MemToReg = Jump = Branch_on_equal = Link_on_Jumping = Stack_Jumping= false;
            break;
            
        case ANDI_OP:
            ALUcontrol = AND;
            RegWrite = ALUSrc = true;
            RegDst = Branch = MemWrite = MemToReg = Jump = Branch_on_equal = Link_on_Jumping = Stack_Jumping =false;
            break;
            
        case ORI_OP:
            ALUcontrol = OR;
            RegWrite = ALUSrc = true;
            RegDst = Branch = MemWrite = MemToReg = Jump = Branch_on_equal = Link_on_Jumping = Stack_Jumping =false;
            break;
            
        case XORI_OP:
            ALUcontrol = XOR;
            RegWrite = ALUSrc = true;
            RegDst = Branch = MemWrite = MemToReg = Jump = Branch_on_equal = Link_on_Jumping = Stack_Jumping =false;
            break;
            
        case SLTI_OP:
            ALUcontrol = SLT;
            RegWrite = ALUSrc = true;
            RegDst = Branch = MemWrite = MemToReg = Jump = Branch_on_equal = Link_on_Jumping = Stack_Jumping =false;
            break;
            
        case LUI_OP:
            ALUcontrol = LUI;
            RegWrite = ALUSrc = true;
            RegDst = Branch = MemWrite = MemToReg = Jump = Branch_on_equal = Link_on_Jumping = Stack_Jumping =false;
            break;
            
        case J_OP:
            ALUcontrol = ADD;
            Jump = true;
            RegDst = Branch = MemWrite = MemToReg = Branch_on_equal = RegWrite = ALUSrc = Link_on_Jumping =Stack_Jumping =false;
            break;
            
        case JAL_OP:
            ALUcontrol = ADD;
            Jump = Link_on_Jumping = true;
            RegDst = Branch = MemWrite = MemToReg = RegWrite = ALUSrc =Stack_Jumping =false;
            break;
            
        case JPR_OP:
            ALUcontrol = ADD;
            Jump = Stack_Jumping  = true;
            RegDst = Branch = MemWrite = MemToReg = RegWrite = ALUSrc = Link_on_Jumping= false;
            break;
    }
}

/*
uint32_t MIPS_Control_Unit::getControlSignals(uint32_t OPCODE, uint32_t function){
    
    static uint32_t control_signals = 0;
    
    switch (OPCODE){
        case R_OPCODE:
            switch (function){
                case ADD_FUNC:
                case ADDU_FUNC: control_signals |= ADD << 16;  break;
                case SUB_FUNC:
                case SUBU_FUNC: control_signals |= SUB << 16;  break;
                case AND_FUNC:  control_signals |= AND << 16;  break;
                case NOR_FUNC:  control_signals |= NOR << 16;  break;
                case OR_FUNC:   control_signals |= OR << 16;   break;
                case SLL_FUNC:  control_signals |= SLL << 16;  break;
                case SLLV_FUNC: control_signals |= SLLV << 16; break;
                case SLT_FUNC:
                case SLTU_FUNC: control_signals |= SLT << 16;  break;
                case SRA_FUNC:  control_signals |= SRA << 16;  break;
                case SRAV_FUNC: control_signals |= SRAV << 16; break;
                case SRL_FUNC: control_signals |= SRL << 16; break;
                case SRLV_FUNC: control_signals |= SRLV << 16; break;
                case XOR_FUNC: control_signals |= XOR << 16; break;
                case JR_FUNC: control_signals |= ADD << 16; break;
            }
            control_signals |= 0x3;
            break;
            
        case LW_OP:
            control_signals |= ADD << 16;
            control_signals |= 0xD;
            break;
            
        case SW_OP:
            control_signals |= ADD << 16;
            control_signals |= 0x14;
            break;
            
        case BEQ_OP:
            control_signals |= SUB << 16;
            control_signals |= 0x20;
            break;
            
        case BNE_OP:
            control_signals |= SUB << 16;
            control_signals |= 0x60;
            break;
            
        case ADDI_OP:
        case ADDIU_OP:
            control_signals |= ADD << 16;
            control_signals |= 0x5;
            break;
            
        case ANDI_OP:
            control_signals |= AND << 16;
            control_signals |= 0x5;
            break;
            
        case ORI_OP:
            control_signals |= OR << 16;
            control_signals |= 0x5;
            break;
            
        case XORI_OP:
            control_signals |= XOR << 16;
            control_signals |= 0x5;
            break;
            
        case SLTI_OP:
            control_signals |= SLT << 16;
            control_signals |= 0x5;
            break;
            
        case LUI_OP:
            control_signals |= LUI << 16;
            control_signals |= 0x5;
            break;
            
        case J_OP:
            control_signals |= ADD << 16;
            control_signals |= 0x80;
            break;
            
        case JAL_OP:
            control_signals |= ADD << 16;
            control_signals |= 0x180;
    }
    
    return control_signals;
}
*/
