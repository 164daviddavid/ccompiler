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
	
	LLVMValueRef function_iterator = LLVMGetFirstFunction(module);
	size_t function_num = 0;
	while (function_iterator != NULL) {
		// Temporary call to print function name
		size_t function_name_length;
		const char *function_name = LLVMGetValueName2(function_iterator, &function_name_length);

		printf("Function %ld - Name - %.*s:\n", function_num, (int) function_name_length, function_name);
		unsigned int num_of_basic_blocks = LLVMCountBasicBlocks(function_iterator);
		LLVMBasicBlockRef block_iterator = LLVMGetFirstBasicBlock(function_iterator);
		for (size_t block = 0; block < num_of_basic_blocks; block++) {
			// Temporary call to dump basic block for experimentation purposes
			printf("Basic Block %ld - Name - %s:\n", block, LLVMGetBasicBlockName(block_iterator));
			LLVMDumpValue(LLVMBasicBlockAsValue(block_iterator));
			
			block_iterator = LLVMGetNextBasicBlock(block_iterator);		
		}

		function_num++;
		function_iterator = LLVMGetNextFunction(function_iterator);
	}


	cleanup_LLVM(context_ref, module, NULL);
	
	
	return 0;
}
