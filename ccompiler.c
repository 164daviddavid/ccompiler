#include <stdio.h>

#include <llvm-c/Core.h>
#include <llvm-c/IRReader.h>

int main() {
	LLVMContextRef context_ref = LLVMContextCreate();


	LLVMModuleRef module = LLVMModuleCreateWithNameInContext("module_1", context_ref);


	// For now, the file path of the LLVM IR file will be hardcoded with a relative path from the root of the repository
	LLVMMemoryBufferRef buffer;
	char *error_msg;

	LLVMBool buffer_success = LLVMCreateMemoryBufferWithContentsOfFile("./test_file_empty_main.ll", &buffer, &error_msg);	if (buffer_success != 0) {
		fprintf(stderr, "%s\n", error_msg);
	}

	LLVMBool parse_success = LLVMParseIRInContext(context_ref, buffer, &module, &error_msg);
	if (parse_success != 0) {
		fprintf(stderr, "%s\n", error_msg);
	}

	
	// LLVMDisposeMessage() only needs to be called if there was an error when
	// creating the memory buffer or parsing the IR. If there was no error, <error_msg>
	// would not have been allocated.
	if (buffer_success != 0 || parse_success != 0) {
		LLVMDisposeMessage(error_msg);
	}
	LLVMDisposeModule(module);
	LLVMContextDispose(context_ref);

	return 0;
}
