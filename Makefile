BUILD ?= debug
CFLAGS = -Wall -Wextra

TEST_DIR = tests
FIXTURE_SRC_DIR = $(TEST_DIR)/fixture_src
FIXTURE_BIN_DIR = $(TEST_DIR)/fixtures
FIXTURE_SRCS = $(wildcard $(FIXTURE_SRC_DIR)/*.c)
FIXTURE_BINS = $(patsubst $(FIXTURE_SRC_DIR)/%.c, $(FIXTURE_BIN_DIR)/%.ll, $(FIXTURE_SRCS))

ifeq ($(BUILD),debug)
    CFLAGS += -g -DDEBUG
else ifeq ($(BUILD),asan)
    CFLAGS += -g -fsanitize=address
else ifeq ($(BUILD),release)
    CFLAGS += -DNDEBUG
else
    $(error Unknown build type: $(BUILD))
endif

ccompiler: ccompiler.c util.c
	gcc $^ -o ccompiler $(CFLAGS) -I/usr/lib/llvm-18/include/ -L/usr/lib/llvm-18/include/ -lLLVM-18

clean:
	rm -f ./ccompiler
	rm -rf $(FIXTURE_BIN_DIR)

fixtures: $(FIXTURE_BIN_DIR) $(FIXTURE_BINS)

$(FIXTURE_BIN_DIR)/%.ll: $(FIXTURE_SRC_DIR)/%.c
	clang -target arm64 -S -emit-llvm -O0 -o $@ $<

$(FIXTURE_BIN_DIR):
	mkdir -p $(FIXTURE_BIN_DIR)
