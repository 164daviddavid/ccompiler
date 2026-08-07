#include <stdio.h>

#include <llvm-c/Core.h>
#include <llvm-c/IRReader.h>


/**
 * Calls the relevant functions to dispose LLVM structures that have been allocated.
 * 
 * If error_msg has not been allocated, pass NULL. 
 */
void cleanup_LLVM(LLVMContextRef context_ref, LLVMModuleRef module, char **error_msg) {
	// LLVMDisposeMessage() only needs to be called if there was an error when
	// creating the memory buffer or parsing the IR. If there was no error, <error_msg>
	// would not have been allocated and the dispose function should not be called.
	if (error_msg != NULL) {
		LLVMDisposeMessage(*error_msg);
	}
	LLVMDisposeModule(module);
	LLVMContextDispose(context_ref);
}

int main() {
	LLVMContextRef context_ref = LLVMContextCreate();


	LLVMModuleRef module = LLVMModuleCreateWithNameInContext("module_1", context_ref);


	// For now, the file path of the LLVM IR file will be hardcoded with a relative path from the root of the repository
	LLVMMemoryBufferRef buffer;
	char *error_msg = "Default error message";

	LLVMBool buffer_success = LLVMCreateMemoryBufferWithContentsOfFile("./test_file_add_int.ll", &buffer, &error_msg);	if (buffer_success != 0) {
		fprintf(stderr, "%s\n", error_msg);
		cleanup_LLVM(context_ref, module, &error_msg);
		return 0;
	}

	LLVMBool parse_success = LLVMParseIRInContext(context_ref, buffer, &module, &error_msg);
	if (parse_success != 0) {
		fprintf(stderr, "%s\n", error_msg);
		cleanup_LLVM(context_ref, module, &error_msg);
		return 0;
	}


	// Temporary call to dump module for experimentation purposes
	LLVMDumpModule(module);

	
	cleanup_LLVM(context_ref, module, NULL);
	
	
	return 0;
}
