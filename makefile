CC      ?= gcc
OUT_DIR := run_tests
BUILD   := build
MODE    ?= debug

SANITIZERS := -fsanitize=address,undefined,leak,pointer-compare,pointer-subtract,alignment,bounds,float-cast-overflow,float-divide-by-zero,signed-integer-overflow
COMMON_WARNINGS := -Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -Wconversion \
		   -Wnull-dereference -Wdouble-promotion -Wimplicit-fallthrough \
		   -Wcast-align -Wstrict-aliasing=3 -Wstrict-prototypes -Wmissing-prototypes \
		   -Wmissing-declarations -Wunused-parameter -Wfloat-equal \
		   -Winit-self -Wuninitialized -Wswitch-enum -Wredundant-decls \
		   -Wpointer-arith -Wvla -Werror

COMMON_FLAGS := -std=c11 -fno-omit-frame-pointer \
		-fno-optimize-sibling-calls -fstack-protector-all \
		-fPIC \
		-Iinclude -Isrc/structs -MMD -MP

ifeq ($(MODE),debug)
	CFLAGS := -g3 -O0 $(COMMON_WARNINGS) $(COMMON_FLAGS) $(SANITIZERS)
	LDFLAGS := $(SANITIZERS) -lm
else
	CFLAGS := -O2 -DNDEBUG -D_FORTIFY_SOURCE=2 $(COMMON_WARNINGS) $(COMMON_FLAGS)
	LDFLAGS := -lm
endif
TD_SRC := src/td/td.c

SLIST_SRC := src/structs/slist/slist.c \
	     src/structs/slist/slist_node/slist_node.c

DLIST_SRC := src/structs/dlist/dlist.c \
	     src/structs/dlist/dlist_node/dlist_node.c

CLIST_SRC := src/structs/clist/clist.c \
	     src/structs/clist/clist_node/clist_node.c

STACK_SRC := src/structs/stack/stack.c \
	     src/structs/stack/stack_node/stack_node.c

QUEUE_SRC := src/structs/queue/queue.c \
	     src/structs/queue/queue_node/queue_node.c

DICT_SRC := src/structs/dict/dict.c \
	    src/structs/dict/dict_entry/dict_entry.c

DEQUE_SRC := src/structs/deque/deque.c \
	     src/structs/deque/deque_node/deque_node.c

SLIST_TEST := tests/sltest.c
DLIST_TEST := tests/dltest.c
CLIST_TEST := tests/cltest.c
STACK_TEST := tests/stktest.c
QUEUE_TEST := tests/qtest.c
DICT_TEST  := tests/dict_test.c
DEQUE_TEST := tests/dqtest.c

SLIST_OBJ := $(patsubst %.c,$(BUILD)/%.o,$(SLIST_SRC) $(TD_SRC) $(SLIST_TEST))
DLIST_OBJ := $(patsubst %.c,$(BUILD)/%.o,$(DLIST_SRC) $(TD_SRC) $(DLIST_TEST))
CLIST_OBJ := $(patsubst %.c,$(BUILD)/%.o,$(CLIST_SRC) $(TD_SRC) $(CLIST_TEST))
STACK_OBJ := $(patsubst %.c,$(BUILD)/%.o,$(STACK_SRC) $(TD_SRC) $(STACK_TEST))
QUEUE_OBJ := $(patsubst %.c,$(BUILD)/%.o,$(QUEUE_SRC) $(TD_SRC) $(QUEUE_TEST))
DICT_OBJ  := $(patsubst %.c,$(BUILD)/%.o,$(DICT_SRC) $(TD_SRC) $(DICT_TEST))
DEQUE_OBJ := $(patsubst %.c,$(BUILD)/%.o,$(DEQUE_SRC) $(TD_SRC) $(DEQUE_TEST))

TARGETS := $(OUT_DIR)/sltest \
	   $(OUT_DIR)/dltest \
	   $(OUT_DIR)/cltest \
	   $(OUT_DIR)/stktest \
	   $(OUT_DIR)/qtest \
	   $(OUT_DIR)/dict_test \
	   $(OUT_DIR)/dqtest

all: directories $(TARGETS)

directories:
	mkdir -p $(OUT_DIR) $(BUILD)


$(BUILD)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@


$(OUT_DIR)/sltest: $(SLIST_OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

$(OUT_DIR)/dltest: $(DLIST_OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

$(OUT_DIR)/cltest: $(CLIST_OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

$(OUT_DIR)/stktest: $(STACK_OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

$(OUT_DIR)/qtest: $(QUEUE_OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

$(OUT_DIR)/dict_test: $(DICT_OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

$(OUT_DIR)/dqtest: $(DEQUE_OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

-include $(shell find $(BUILD) -name '*.d' 2>/dev/null)

clean:
	rm -rf $(OUT_DIR) $(BUILD)

.PHONY: all clean directories
