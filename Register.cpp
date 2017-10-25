//
//  Register.cpp
//  MIPS_Pipelined_Simulator
//
//  Created by Mohamed A Tawfik on May/4/17.
//  Copyright © 2017 Mohamed A Tawfik. All rights reserved.
//

#include "Register.hpp"

Register::Register(){
    Data.resize(0);
    Next_Data.resize(0);
    R = E = false;
}
Register::Register(const Register& other):Data(other.Data), Next_Data(other.Next_Data), R(other.R), E(other.E){
}
Register::Register(Register&& other):Data(other.Data), Next_Data(other.Next_Data), R(other.R), E(other.E){
}

Register::~Register(){
}

void Register::setD(const std::vector<uint32_t>& d){
    Next_Data = d;
}
void Register::getQ(std::vector<uint32_t>& Q){
    Q = Data;
}

void Register::setReset(bool reset){
    R = reset;
}

void Register::setEnable(bool enable){
    E = enable;
}

void Register::update(){
    if(R)
        for(int i = 0; i < Data.size(); i++)
            Data[i] = 0;
    else
        if(E)
            Data = Next_Data;
}
