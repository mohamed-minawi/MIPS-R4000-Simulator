MIPS_Assembler
v 1.0.0

This code is part of the R4000 MIPS pipleined processor simulator project for the CSCE 3301 Computer Architecture course at AUC Spring 2017.
Our group consists of Ahmed Refaey, Mohamed Abdelrahman, Mohamed Ali and Mohamed El-Minawi.
This MIPS_Assembler class was written by Mohamed Abdelrahman.

The MIPS_Assembler class includes only one static method in its API. The class is not meant to be instantiated.

The exprToMachine method accepts a C++ string as a parameter, interprets it as a True MIPS instruction, and returns the corresponding machine code bits as a uint32_t. It throws an std::invalid_argument exception if the sent instruction is unsupported, an invalid register or immediate value is used, or invalid syntax is detected.

The following instructions are supported:
    R-Type:
        "add", "addu", "and", "jr", "nor", "or", "sll", "sllv", "slt", "sltu", "sra", "srav", "srl", "srlv", "sub", "subu", "xor"
    I-Type:
        "addi", "addiu", "andi", "beq", "bne", "lw", "lui", "ori", "slti", "sw", "xori"
    J-Type:
        "j", "jal"

The assembler supports a slightly different syntax. The characters '$', ',', '(' and ')' are to be omitted. A register is directly referenced to by its number [0-31], and immediates shall be ONLY in decimal. No labels are allowed, and addresses are also to be in decimal. Tokens are to be seperated by 1 or multiple spaces.

An accompanying test code is provided that handles sequential expressions.
