//
//  Register.hpp
//  MIPS_Pipelined_Simulator
//
//  Created by Mohamed A Tawfik on May/4/17.
//  Copyright © 2017 Mohamed A Tawfik. All rights reserved.
//

#ifndef Register_hpp
#define Register_hpp

#include <vector>
#include <stdint.h>

class Register{
private:
    std::vector<uint32_t> Data, Next_Data;
    bool R, E;
    
    
public:
    Register();
    Register(int);
    Register(const Register&);
    Register(Register&&);
    
    ~Register();

    void resize(int);
    
    void setD(const std::vector<uint32_t>&);
    void getQ(std::vector<uint32_t>&);
    
    void setReset(bool reset);
    void setEnable(bool enable);
    
    void update();
};

#endif /* Register_hpp */
