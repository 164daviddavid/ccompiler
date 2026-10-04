#ifndef INSTRUCTION_SELECTION_H
#define INSTRUCTION_SELECTION_H

#include <stdio.h>

#include <llvm-c/Core.h>


struct ARM64_instruction;
union ARM64_concrete_instruction;
enum ARM64_instruction_type;

struct ARM64_ADD_IMM;
struct ARM64_ADD_REG;




//
//
//
// High level ARM64 structures
// 
// - enum ARM64_instruction_type
// - union ARM64_concrete_instruction
// - struct ARM64_instruction
//
//
//

/*
 * The overall struct representing an ARM64 instruction. <type> indicates the
 * specific ARM instruction/mnemonic (e.g. ADD, ADDS, CMP, etc.) and
 * <instruction> contains the operands of a specific, concrete ARM64
 * instruction.
 */
struct ARM64_instruction {
	enum ARM64_instruction_type type;
	union ARM64_concrete_instruction instruction;
};

enum ARM64_instruction_type {
	ADD_IMM,
	ADD_REG
}

union ARM64_concrete_instruction {
	struct ARM64_ADD_IMM ADD_IMM;
	struct ARM64_ADD_REG ADD_REG;
};



//
//
//
// Concrete ARM64 instruction structures
//
//
//

/*
 * The struct representing an ADD using an immediate value. <shift> is either
 * 0 or 1, with the former indicating no shift and the latter indicating
 * a logical shift left of 12 bits; the immediate value itself is a value from
 * 0 to 4096 (2^11 - 1).
 */
struct ARM64_ADD_IMM {
	unsigned int destination_reg;
	unsigned int operand_1;
	unsigned int operand_immediate;
	unsigned int shift;
};

/*
 * The struct representing an ADD using two registers as operands. For the
 * ADD instruction using an immediate value, see ARM64_ADD_IMM. <shift> is
 * 0, 1, or 2, indicating a logical shift left, logical shift right, and
 * arithmetic shift right respectively.
 */
struct ARM64_ADD_REG {
	unsigned int destination_reg;
	unsigned int operand_1;
	unsigned int operand_2;
	unsigned int shift;
	unsigned int shift_ammount;
};


//
//
//
// High level LLVM IR structures
// 
// - enum LLVMIR_pattern_kind
// - struct LLVMIR_pattern
//
//
//

/*
 * The struct containing one or more LLVM IR instructions that make up a
 * pattern. <instructions> is a pointer to one or more LLVMValueRef
 * structures that are specifically an Instruction value.
 */
struct LLVMIR_pattern {
	enum LLVMIR_pattern_kind kind;
	int num_of_instructions;
	LLVMValueRef *instructions;
};

enum LLVMIR_pattern_kind {
	ADD_REG_IMM,
	ADD_REG_REG,
}

#endif