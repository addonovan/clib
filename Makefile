BIN 	  := bin
INCLUDE	  := include
SRC 	  := src
TEST_SRC  := test
OBJ       := obj

CC 	  := gcc
LINKER 	  := gcc
CFLAGS 	  := -Wall -Wextra -Werror -g3

TEST_SRCS := $(wildcard $(TEST_SRC)/*.c)
SRCS      := $(wildcard $(SRC)/*.c)

TEST_OBJS := $(patsubst $(TEST_SRC)/%.c, $(OBJ)/%.o, $(TEST_SRCS))
OBJS      := $(patsubst      $(SRC)/%.c, $(OBJ)/%.o,      $(SRCS))

TEST_OUTS := $(patsubst $(TEST_SRC)/%.c, $(BIN)/test_%.out, $(TEST_SRCS))

tests: makedirs $(TEST_OUTS)
.PHONY: tests

$(TEST_OUTS): %.out: $(TEST_OBJS) $(OBJS)
	$(LINKER) $(CFLAGS) $^ -o $@

$(TEST_OBJS): %.o: $(TEST_SRCS) $(SRCS)
	$(CC) $(CFLAGS) -I$(INCLUDE) -c $< -o $@

$(OBJS): %.o: $(SRCS)
	$(CC) $(CFLAGS) -I$(INCLUDE) -c $< -o $@

makedirs:
	@mkdir -p $(BIN)
	@mkdir -p $(OBJ)
.PHONY: makedirs

clean:
	@rm -rf $(BIN)
	@rm -rf $(OBJ)
.PHONY: clean

