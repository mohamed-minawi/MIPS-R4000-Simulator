//
//  Jump_Table.cpp
//  MIPS_Pipelined_Simulator
//
//  Created by Mohamed A Tawfik on May/15/17.
//  Copyright © 2017 Mohamed A Tawfik. All rights reserved.
//

#include "Jump_Table.hpp"

Jump_Table::Jump_Table(){
    Table.resize(0);
}
Jump_Table::Jump_Table(const Jump_Table& other):Table(other.Table){
}
Jump_Table::Jump_Table(Jump_Table&& other):Table(other.Table){
}

Jump_Table::~Jump_Table(){
}

void Jump_Table::addIfNotPresent(uint32_t pc, uint32_t target_pc){
    if(!exists(pc)){
        line l;
        l.from_address = pc; l.target_address = target_pc;
        Table.push_back(l);
    }
}
bool Jump_Table::exists(uint32_t pc) const{
    for(int i = 0; i < Table.size(); ++i)
        if(pc == Table[i].from_address)
            return true;
    return false;
}

uint32_t Jump_Table::getSpeculatedAddress(uint32_t pc) const{
    for(int i = 0; i < Table.size(); i++)
        if(Table[i].from_address == pc)
            return Table[i].target_address;
    return pc;
}

void Jump_Table::clear(){
    Table.resize(0);
}

