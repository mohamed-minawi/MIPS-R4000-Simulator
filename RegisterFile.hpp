//
//  RegisterFile.hpp
//  MIPS_Pipelined_Simulator
//
//  Created by Mohamed A Tawfik on May/8/17.
//  Copyright © 2017 Mohamed A Tawfik. All rights reserved.
//

#ifndef RegisterFile_hpp
#define RegisterFile_hpp

#include <stdint.h>
#include <vector>

class RegisterFile{
public:
    RegisterFile();
    RegisterFile(uint32_t size);
    RegisterFile(const RegisterFile& other);
    
    ~RegisterFile();
    
    void resizeAndReset(uint32_t size);
    void write(uint32_t index, uint32_t value, bool RegWrite);
    uint32_t read(uint32_t);
    
private:
    std::vector<uint32_t> array;
};
#endif /* RegisterFile_hpp */
