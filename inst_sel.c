#include "inst_sel.h"

/*
 * Pattern Identification Function Table
 *
 * This is a table of all pattern identification functions. Index the array
 * using the identified opcode of each instruction, obtained by using
 * LLVMGetInstructionOpcode() on an instruction.
 */
pattern_identify_func LLVMIR_pattern_identify_table[] = {
    [LLVMAdd] = identify_LLVMIR_pattern_ADD,
};

//
//
//
// Functions for LLVM IR structures
//
//
//
struct LLVMIR_pattern identify_LLVMIR_pattern(LLVMValueRef instructions,
											  size_t window_size,
											  size_t *consumed) {
    // CURRENTLY, THE ONLY ACCEPTED WINDOW SIZE IS 1
    window_size = 1;
    
    LLVMOpcode opcode = LLVMGetInstructionOpcode(instructions);
    pattern_identify_func handler = LLVMIR_pattern_identify_table[opcode];
    
    return handler(instructions);
}

struct LLVMIR_pattern identify_LLVMIR_pattern_ADD(LLVMValueRef instructions) {
    // CURRENTLY, ONLY CHECKS ONE INSTRUCTION I.E. WINDOW SIZE OF 1
    struct LLVMIR_pattern result = {
        .kind = UNKNOWN;
        .num_of_instructions = -1;
        .instructions = instructions;
    };
    
    if (LLVMGetInstructionOpcode(opcode) != LLVMAdd) {
        return result;
    }

    
    LLVMValueRef operand2 = LLVMGetOperand(instruction_iterator, 1);
    
    LLVMValueKind value_kind2 = LLVMGetValueKind(operand2);

    if (value_kind2 == LLVMInstructionValueKind) {
        result.kind = ADD_REG_REG;

    // there are other Constant Value Kinds that an ADD instruction can
    // accept, but Int is the only one supported for now
    } else if (value_kind2 == LLVMConstantIntValueKind) {
        result.kind = ADD_REG_IMM;
    }

    result.num_of_instructions = 1;
    return result;
}