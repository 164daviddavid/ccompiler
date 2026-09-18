#include <stdio.h>

#include <llvm-c/Core.h>

char *LLVMValueKind_as_string(LLVMValueKind value_kind);

void print_LLVMValueKind(LLVMValueKind value_kind);