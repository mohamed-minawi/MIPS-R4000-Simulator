//
//  Branch_Predictor.cpp
//  MIPS_Pipelined_Simulator
//
//  Created by Mohamed A Tawfik on Apr/21/17.
//  Copyright © 2017 Mohamed A Tawfik. All rights reserved.
//

#include "Branch_Predictor.hpp"

Branch_Predictor::Branch_Predictor(){
    Table.resize(0);
}
Branch_Predictor::Branch_Predictor(const Branch_Predictor& other):Table(other.Table){
}
Branch_Predictor::Branch_Predictor(Branch_Predictor&& other):Table(other.Table){
}

Branch_Predictor::~Branch_Predictor(){
}

void Branch_Predictor::addIfNotPresent(uint32_t pc, uint32_t target_pc){
    if(!exists(pc)){
        line l;
        l.from_address = pc; l.target_address = target_pc; l.decision = Decision::Definitely_NO;
        Table.push_back(l);
    }
}
bool Branch_Predictor::shouldBranch(uint32_t pc) const{
    for(int i = 0; i < Table.size(); ++i)
        if(pc == Table[i].from_address)
            return (Table[i].decision >> 1);
    return false;
}
Branch_Predictor::Decision Branch_Predictor::getDecision(uint32_t pc) const{
    for(int i = 0; i < Table.size(); ++i)
        if(pc == Table[i].from_address)
            return Table[i].decision;
    return Decision::Definitely_NO;
}
bool Branch_Predictor::updateDecision(uint32_t pc, bool taken){
    for(int i = 0; i < Table.size(); i++)
        if(pc == Table[i].from_address){
            if(taken && Table[i].decision != Decision::Definitely_YES){
                Table[i].decision = Decision(int(Table[i].decision) + 1);
                return true;
            }
            if(!taken && Table[i].decision != Decision::Definitely_NO){
                Table[i].decision = Decision(int(Table[i].decision) - 1);
                return true;
            }
        }
    return false;
}

bool Branch_Predictor::exists(uint32_t pc) const{
    for(int i = 0; i < Table.size(); ++i)
        if(pc == Table[i].from_address)
            return true;
    return false;
}

uint32_t Branch_Predictor::getSpeculatedAddress(uint32_t pc) const{
    for(int i = 0; i < Table.size(); i++){
        if(Table[i].from_address == pc){
            if(int(Table[i].decision) >> 1)
                return Table[i].target_address;
            else
                return pc;
        }
    }
    return pc;
}

void Branch_Predictor::clear(){
    Table.resize(0);
}

