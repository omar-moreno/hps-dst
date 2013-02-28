
# Variables
CC		:= g++
CFLAGS	:= -Wall `root-config --cflags` -I$(LCIO)/include -I$(PWD)/include -fPIC -pthread 
LFLAGS  := -L$(LCIO)/lib -llcioDict -llcio `root-config --libs`  
BIN		:= $(PWD)/bin
OBJ		:= $(PWD)/.obj
LIB		:= $(PWD)/lib
SOFLAGS := -shared

# HPS Event
HPS_EVENT_SRC_DIR := $(PWD)/src
HPS_EVENT_HDR_DIR := $(PWD)/include
HPS_EVENT_SRC := $(wildcard $(HPS_EVENT_SRC_DIR)/*.cxx)
HPS_EVENT_HDR := $(wildcard $(HPS_EVENT_HDR_DIR)/*.h)
HPS_EVENT_OBJ := $(patsubst $(HPS_EVENT_SRC_DIR)/%.cxx,$(OBJ)/%.o,$(HPS_EVENT_SRC))
HPS_EVENT_SO  := $(LIB)/libHpsEvent.so

# HPS Event Dictionary
HPS_EVENT_DIC_DIR := $(PWD)/root_dic
HPS_EVENT_DIC_SRC := $(HPS_EVENT_DIC_DIR)/HpsEventDic.cxx
HPS_EVENT_DIC_OBJ := $(OBJ)/HpsEventDic.o
ROOTCINT := rootcint

# Root Sources
ROOT_DIR	:= $(PWD)
ROOT_SRC	:= $(wildcard $(ROOT_DIR)/*.cxx)
ROOT_BIN	:= $(patsubst $(ROOT_DIR)/%.cxx, $(BIN)/%, $(ROOT_SRC))

# Default
all: dirs print $(HPS_EVENT_OBJ) $(HPS_EVENT_DIC_OBJ) $(HPS_EVENT_SO) $(ROOT_BIN) 

print: 
	echo $(HPS_EVENT_SRC)

# Clean
clean:
	rm -rf $(OBJ)
	rm -rf $(BIN)
	rm -rf $(HPS_EVENT_DIC_DIR)
	rm -rf $(LIB)

# Create obj and bin directories
dirs:
	test -d $(OBJ) || mkdir $(OBJ)
	test -d $(BIN) || mkdir $(BIN)
	test -d $(HPS_EVENT_DIC_DIR) || mkdir $(HPS_EVENT_DIC_DIR)
	test -d $(LIB) || mkdir $(LIB)

# Compile HPS Event sources
$(OBJ)/%.o: $(HPS_EVENT_SRC_DIR)/%.cxx $(HPS_EVENT_HDR_DIR)/%.h
	$(CC) $(CFLAGS) -c $< -o $@ 

# Compile ROOT sources
$(BIN)/%: $(ROOT_DIR)/%.cxx  
	$(CC) $(CFLAGS) $(LFLAGS) $(OBJ)/* -o $@ $<

# Create the HPS Event ROOT dictionary
$(HPS_EVENT_DIC_SRC): $(HPS_EVENT_HDR)
	$(ROOTCINT) -f $@ -c $^

# Compile HPS Event ROOT dictionary
$(HPS_EVENT_DIC_OBJ): $(HPS_EVENT_DIC_SRC) $(HPS_EVENT_DIC_DIR)/HpsEventDic.h
	$(CC) $(CFLAGS) -c $< -o $@ 

# Create HPS Event ROOT library
$(HPS_EVENT_SO): $(HPS_EVENT_DIC_OBJ)
	$(CC) $(SOFLAGS) $^ -o $@

