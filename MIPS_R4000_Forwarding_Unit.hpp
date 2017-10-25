//
//  MIPS_R4000_Forwarding_Unit.hpp
//  MIPS_Pipelined_Simulator
//
//  Created by Mohamed A Tawfik on Apr/21/17.
//  Copyright © 2017 Mohamed A Tawfik. All rights reserved.
//

#ifndef MIPS_R4000_Forwarding_Unit_hpp
#define MIPS_R4000_Forwarding_Unit_hpp

#include <stdint.h>

class MIPS_R4000_Forwarding_Unit{
public:
    enum Final_Source{EX, DF, DS, TC_mem, TC_alu};
    
    static Final_Source getDecision(uint32_t rs_EX, uint32_t rd_DF, bool reg_we_DF, uint32_t rd_DS, bool reg_we_DS, uint32_t rd_TC, bool reg_we_TC, uint32_t rt_TC, bool mem_to_reg_TC);
};

#endif /* MIPS_R4000_Forwarding_Unit_hpp */
