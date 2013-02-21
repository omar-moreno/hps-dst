
# Variables
CC		:= g++
CFLAGS	:= -Wall `root-config --cflags` -I$(LCIO)/include 
LFLAGS  := -L$(LCIO)/lib -llcioDict -llcio `root-config --libs`  
BIN		:= $(PWD)/bin
OBJ		:= $(PWD)/.obj


# Root Sources
ROOT_DIR	:= $(PWD)
ROOT_SRC	:= $(wildcard $(ROOT_DIR)/*.cxx)
ROOT_BIN	:= $(patsubst $(ROOT_DIR)/%.cxx, $(BIN)/%, $(ROOT_SRC))

# Default
all: dirs $(ROOT_BIN)

# Create obj and bin directories
dirs:
	test -d $(OBJ) || mkdir $(OBJ)
	test -d $(BIN) || mkdir $(BIN)

# Compile ROOT sources
$(BIN)/%: $(ROOT_DIR)/%.cxx
	$(CC) $(CFLAGS) $(LFLAGS) -o $@ $<

# Clean
clean:
	rm -rf $(OBJ)
