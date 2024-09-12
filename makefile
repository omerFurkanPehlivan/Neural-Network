CC = gcc
CCFLAGS = -Wall -Wextra -Werror

LINKER = gcc

MAIN_EXE = ./main.exe
TEST_EXE = ./test.exe
DBG_EXE = ./dbg.exe
MAIN_C = ./main.c
TEST_C = ./test.c
MAIN_O = ./main.o
TEST_O = ./test.o

SRC_DIR = ./src
HEADER_DIR = ./include
LIB_DIR = ./lib
BIN_DIR = ./bin
TEST_DIR = ./test

SRC = ${wildcard $(SRC_DIR)/*.c}
HEADER = ${patsubst %.c, %.h, ${subst $(SRC_DIR), $(HEADER_DIR), $(SRC)}}
OBJ = ${patsubst %.c, %.o, ${subst $(SRC_DIR), $(BIN_DIR), $(SRC)}}

FILE = nofile
TEST_FILE = $(TEST_DIR)/test_$(FILE).c


all: $(MAIN_EXE)
	$(MAIN_EXE)

obj: $(BIN_DIR)/$(FILE).o

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c $(HEADER_DIR)/%.h $(LIB_DIR)/*.h
	$(CC) $(CCFLAGS) -c -o $@ $<

$(MAIN_EXE): $(MAIN_C) $(SRC) $(HEADER)
	$(CC) $(CCFLAGS) -o $(MAIN_EXE) $(MAIN_C) $(SRC)

test: $(TEST_EXE)
	$(TEST_EXE)

test_%: $(TEST_DIR)/test_%.c $(SRC_DIR)/%.c $(HEADER_DIR)/%.h $(LIB_DIR)/*.h
	$(CC) $(CCFLAGS) -o $(TEST_DIR)/$@.exe $(TEST_DIR)/$@.c $(SRC); \
	$(TEST_DIR)/$@.exe

$(TEST_EXE): $(TEST_C) $(TEST_SRC)
	$(CC) $(CCFLAGS) -o $(TEST_EXE) $(TEST_SRC)

dbg: $(DBG_EXE)
	gdb $(DBG_EXE)

$(DBG_EXE): $(MAIN_C) $(SRC) $(HEADER)
	$(CC) $(CCFLAGS) -g -o $(DBG_EXE) $(MAIN_C) $(SRC)


