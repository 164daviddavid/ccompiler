#ifndef INSTRUCTION_SELECTION_H
#define INSTRUCTION_SELECTION_H

#include <stdio.h>

#include <llvm-c/Core.h>


struct ARM64_program;
struct ARM64_basic_block;
struct ARM64_instruction;
union ARM64_concrete_instruction;
enum ARM64_instruction_type;

struct ARM64_ADD_IMM;
struct ARM64_ADD_REG;

struct LLVMIR_pattern;
enum LLVMIR_pattern_kind;





/*
 * <LLVMIR_pattern_identify_func>: function pointer to functions that handle
 * specific combinations of LLVM IR instructions and determine an appropriate
 * pattern. See section 'Functions for LLVM IR structures' for specific
 * pattern identification functions.
 *
 * <LLVMIR_pattern_identify_table>: table of aforementioned function pointers.
 * Is indexed by the LLVM IR instruction opcodes.
 */
typedef struct LLVMIR_pattern (*LLVMIR_pattern_identify_func)(LLVMValueRef instructions);
LLVMIR_pattern_identify_func LLVMIR_pattern_identify_table[];


/*
 * <ARM64_inst_sel_func>: function pointer to functions that handle specific
 * LLVM IR patterns and produce ARM64 instruction structs that represent
 * the pattern. The instruction structs are added to the <instructions> array
 * of a basic block struct.
 */
typedef int (*ARM64_inst_sel_func)(struct LLVMIR_pattern pattern,
	                           struct *ARM64_basic_block basic_block);
ARM64_inst_sel_func ARM64_inst_sel_table[];



//
//
//
// High level ARM64 structures
// 
// - struct ARM64_program
// - struct ARM64_basic_block
// - enum ARM64_instruction_type
// - union ARM64_concrete_instruction
// - struct ARM64_instruction
//
//
//

/*
 * The struct representing an ARM64 program. It contains a dynamic array
 * of basic block structs, <basic_blocks>. The capacity of this array is
 * represented by <capacity> and <current_size> indicates the number of
 * basic blocks currently in the array.
 *
 * The array is heap allocated with an initial size that can be passed
 * when calling ARM64_program_init().
 *
 * Use ARM64_program_next_empty_basic_block() to obtain a pointer to the
 * first empty element of the array. It will automatically expand the
 * array if it is full.
 */
struct ARM64_program {
	char *name;
	int current_size;
	int capacity;
	struct ARM64_basic_block *basic_blocks;
};

/*
 * The struct representing a basic block. It contains a dynamic array of
 * instruction structs, <instructions>. The capacity of this array is
 * represented by <capacity> and <current_size> indicates the number of
 * instructions currently in the array.
 *
 * The array is heap allocated with an initial size that can be passed
 * when calling ARM64_basic_block_init().
 *
 * Use ARM64_basic_block_next_empty_instruction() to obtain a pointer to
 * the first empty element of the array. It will automatically expand the
 * array if it is full.
 */
struct ARM64_basic_block {
	char *name;
	int current_size;
	int capacity;
	struct ARM64_instruction *instructions;
};

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
// Functions for ARM64 structures
//
//
//
/*
 * Initialise an ARM64_program struct with the given name and dynamic
 * array with the given initial size. <initial_size> must be at least
 * 1.
 *
 * Returns 1 if the pointer is NULL. Returns 2 if the given name is NULL.
 * Returns 3 if the given initial size is invalid.
 */
int ARM64_program_init(struct ARM64_program *program, char *name, int initial_size);

int ARM64_program_next_empty_basic_block(struct ARM64_program *program);

int ARM64_basic_block_init(struct ARM64_basic_block *basic_block, char *name, int initial_size);

int ARM64_basic_block_next_empty_instruction(struct ARM64_basic_block *basic_block);




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
 * pattern. <instructions> is an LLVMValueRef iterator, where each
 * LLVMValueRef is specifically an Instruction value. Get the next
 * instruction by calling LLVMGetNextInstruction() with the iterator
 * as the argument. Use <num_of_instructions> to control the iteration.
 */
struct LLVMIR_pattern {
	enum LLVMIR_pattern_kind kind;
	int num_of_instructions;
	LLVMValueRef instructions;
};

enum LLVMIR_pattern_kind {
	UNKNOWN, // used when no other pattern can be identified
	ADD_REG_IMM,
	ADD_REG_REG,
}





//
//
//
// Functions for LLVM IR structures
//
//
//

/*
 * NOTE: <window_size> IS CURRENTLY FIXED TO 1.
 * 
 * Reads <window_size> LLVM IR instructions starting from <instructions>,
 * which is an iterator of LLVMValueRef (all of which are specifically
 * an Instruction value), and identifies a suitable pattern. Returns an
 * LLVMIR_pattern struct containing the identified pattern and instructions
 * used.
 * 
 * <consumed> is the number of instructions that are used for the
 * identified pattern (patterns will always consist of contiguous
 * LLVM IR instructions) and is set by the function.
 */
struct LLVMIR_pattern identify_LLVMIR_pattern(LLVMValueRef instructions,
											  size_t window_size,
											  size_t *consumed);

//
// Below is the start of specific pattern identification functions.
//
// All will return a struct LLVMIR_pattern with <num_of_instructions> set
// to 1 if they cannot handle the LLVM IR instructions passed to them.
//
struct LLVMIR_pattern identify_LLVMIR_pattern_ADD(LLVMValueRef instructions);

#endif
