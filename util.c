#include "util.h"

char *LLVMValueKind_as_string(LLVMValueKind value_kind) {
	switch (value_kind) {
		case LLVMArgumentValueKind:
			return "Argument Value";
		case LLVMBasicBlockValueKind:
			return "Basic Block Value";
		case LLVMMemoryUseValueKind:
			return "Memory Use Value";
		case LLVMMemoryDefValueKind:
			return "Memory Def Value";
		case LLVMMemoryPhiValueKind:
			return "Memory Phi Value";
		case LLVMFunctionValueKind:
			return "Function Value";
		case LLVMGlobalAliasValueKind:
			return "Global Alias Value";
		case LLVMGlobalIFuncValueKind:
			return "Global IFunc Value";
		case LLVMGlobalVariableValueKind:
			return "Global Variable Value";
		case LLVMBlockAddressValueKind:
			return "Block Address Value";
		case LLVMConstantExprValueKind:
			return "Constant Expr Value";
		case LLVMConstantArrayValueKind:
			return "Constant Array Value";
		case LLVMConstantStructValueKind:
			return "Constant Struct Value";
		case LLVMConstantVectorValueKind:
			return "Constant Vector Value";
		case LLVMUndefValueValueKind:
			return "Under Value Value";
		case LLVMConstantAggregateZeroValueKind:
			return "Constant Aggregate Zero Value";
		case LLVMConstantDataArrayValueKind:
			return "Constant Data Array Value";
		case LLVMConstantDataVectorValueKind:
			return "Constant Data Vector Value";
		case LLVMConstantIntValueKind:
			return "Constant Int Value";
		case LLVMConstantFPValueKind:
			return "Constant FP Value";
		case LLVMConstantPointerNullValueKind:
			return "Constant Pointer Null Value";
		case LLVMConstantTokenNoneValueKind:
			return "Constant Token None Value";
		case LLVMMetadataAsValueValueKind:
			return "Metadata as Value Value";
		case LLVMInlineAsmValueKind:
			return "Inline ASM Value";
		case LLVMInstructionValueKind:
			return "Instruction Value";
		case LLVMPoisonValueValueKind:
			return "Poison Value Value";
		case LLVMConstantTargetNoneValueKind:
			return "Constant Target None Value Value";
		default:
			return "";
	}
}

void print_LLVMValueKind(LLVMValueKind value_kind) {
	printf("%s\n", LLVMValueKind_as_string(value_kind));
}