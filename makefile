# ======================
#  compiler
# ======================

CC      ?= gcc
OUT_DIR := run_tests
BUILD   := build
MODE    ?= debug

# ======================
#  sanitizers
# ======================

SANITIZERS := -fsanitize=address,undefined,leak,pointer-compare,pointer-subtract,alignment,bounds,float-cast-overflow,float-divide-by-zero,signed-integer-overflow

# ======================
#  common flags
# ======================

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

# ======================
#  sources
# ======================

TD_SRC := src/td/td.c

SLIST_SRC := src/structs/list/slist/slist.c \
	     src/structs/list/slist/snode/snode.c

DLIST_SRC := src/structs/list/dlist/dlist.c \
	     src/structs/list/dlist/dnode/dnode.c

CLIST_SRC := src/structs/list/clist/clist.c \
	     src/structs/list/clist/cnode/cnode.c

STACK_SRC := src/structs/stack/stack.c \
	     src/structs/stack/stknode/stknode.c

QUEUE_SRC := src/structs/queue/queue.c \
	     src/structs/queue/qnode/qnode.c

DICT_SRC := src/structs/dict/dict.c \
	    src/structs/dict/entry/entry.c

# ======================
#  tests
# ======================

SLIST_TEST := tests/sltest.c
DLIST_TEST := tests/dltest.c
CLIST_TEST := tests/cltest.c
STACK_TEST := tests/stktest.c
QUEUE_TEST := tests/qtest.c
DICT_TEST  := tests/dict_test.c

# ======================
#  object conversion
# ======================

SLIST_OBJ := $(patsubst %.c,$(BUILD)/%.o,$(SLIST_SRC) $(TD_SRC) $(SLIST_TEST))
DLIST_OBJ := $(patsubst %.c,$(BUILD)/%.o,$(DLIST_SRC) $(TD_SRC) $(DLIST_TEST))
CLIST_OBJ := $(patsubst %.c,$(BUILD)/%.o,$(CLIST_SRC) $(TD_SRC) $(CLIST_TEST))
STACK_OBJ := $(patsubst %.c,$(BUILD)/%.o,$(STACK_SRC) $(TD_SRC) $(STACK_TEST))
QUEUE_OBJ := $(patsubst %.c,$(BUILD)/%.o,$(QUEUE_SRC) $(TD_SRC) $(QUEUE_TEST))
DICT_OBJ  := $(patsubst %.c,$(BUILD)/%.o,$(DICT_SRC) $(TD_SRC) $(DICT_TEST))

# ======================
#  targets
# ======================

TARGETS := $(OUT_DIR)/sltest \
	   $(OUT_DIR)/dltest \
	   $(OUT_DIR)/cltest \
	   $(OUT_DIR)/stktest \
	   $(OUT_DIR)/qtest \
	   $(OUT_DIR)/dict_test

all: directories $(TARGETS)

directories:
	mkdir -p $(OUT_DIR) $(BUILD)

# ======================
#  compile rule
# ======================

$(BUILD)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# ======================
#  link rules
# ======================

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

# ======================
#  dependencies
# ======================

-include $(shell find $(BUILD) -name '*.d' 2>/dev/null)

# ======================
#  cleanup
# ======================

clean:
	rm -rf $(OUT_DIR) $(BUILD)

.PHONY: all clean directories
