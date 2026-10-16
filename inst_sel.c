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
// Functions for ARM64 structures
//
//
int ARM64_program_init(struct ARM64_program *program, char *name, int initial_size) {
    if (program == NULL) {
        return 1;
    }
    if (name == NULL) {
        return 2;
    }
    if (initial_size < 1) {
        return 3;
    }

    program->name = name;
    program->current_size = 0;
    program->capacity = initial_size;
    program->basic_blocks = malloc(initial_size * sizeof(struct ARM64_basic_block));

    return 0;
}

int ARM64_program_next_empty_basic_block(struct ARM64_program *program,
                                         struct ARM64_basic_block **basic_block) {
    int EXPANSION_FACTOR = 2;

    if (program == NULL) {
        return 1;
    }
    if (basic_block == NULL) {
        return 2;
    }

    if (program->current_size == program->capacity) {
        int new_capacity = EXPANSION_FACTOR * program->capacity
                           * sizeof(ARM64_basic_block);
        program->basic_blocks = realloc(program->basic_blocks, new_capacity);
        program->capacity = new_capacity;
    }

    *basic_block = &program->basic_blocks[current_size];
    program->current_size += 1;

    return 0;
}

int ARM64_basic_block_init(struct ARM64_basic_block *basic_block, char *name,
                           int initial_size) {
    if (basic_block == NULL) {
        return 1;
    }
    if (name == NULL) {
        return 2;
    }
    if (initial_size < 1) {
        return 3;
    }

    basic_block->name = name;
    basic_block->current_size = 0;
    basic_block->capacity = initial_size;
    basic_block->instructions = malloc(initial_size * sizeof(struct ARM64_instruction));

    return 0;
}

int ARM64_basic_block_next_empty_instruction(struct ARM64_basic_block *basic_block,
                                             struct ARM64_instruction **instruction) {
    int EXPANSION_FACTOR = 2;

    if (basic_block == NULL) {
        return 1;
    }
    if (instruction == NULL) {
        return 2;
    }

    if (basic_block->current_size == basic_block->capacity) {
        int new_capacity = EXPANSION_FACTOR * basic_block->capacity
                           * sizeof(ARM64_instruction);
        basic_block->basic_blocks = realloc(basic_block->basic_blocks, new_capacity);
        basic_block->capacity = new_capacity;
    }

    *instruction = &basic_block->instructions[current_size];
    basic_block->current_size += 1;

    return 0;
}



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
