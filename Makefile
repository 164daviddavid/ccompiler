BUILD ?= debug
CFLAGS = -Wall -Wextra

ifeq ($(BUILD),debug)
    CFLAGS += -g -DDEBUG
else ifeq ($(BUILD),asan)
    CFLAGS += -g -fsanitize=address
else ifeq ($(BUILD),release)
    CFLAGS += -DNDEBUG
else
    $(error Unknown build type: $(BUILD))
endif

ccompiler: ccompiler.c
	gcc ccompiler.c -o ccompiler $(CFLAGS) -I/usr/lib/llvm-18/include/ -L/usr/lib/llvm-18/include/ -lLLVM-18
