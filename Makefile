#
# author:  Omar Moreno <omoreno1@ucsc.edu>
#		   Santa Cruz Institute for Particle Physics
#		   University of California, Santa Cruz
# version: 1.0
# date:    February 19, 2013
#

# Variables
CC		:= g++
CFLAGS	:= -O2 -Wall -fPIC -pthread -m64 `root-config --cflags` -I$(LCIO)/include -I$(PWD)/include   
LFLAGS  := -L$(LCIO)/lib -llcioDict -llcio `root-config --libs`  
BIN		:= $(PWD)/bin
OBJ		:= $(PWD)/.obj
LIB		:= $(PWD)/lib
SOFLAGS := -shared -O2 -m64

# HPS Event
HPS_EVENT_SRC_DIR := $(PWD)/src
HPS_EVENT_HDR_DIR := $(PWD)/include
HPS_EVENT_SRC := $(wildcard $(HPS_EVENT_SRC_DIR)/*.cxx)
HPS_EVENT_HDR := $(filter-out $(HPS_EVENT_HDR_DIR)/HpsEventLinkDef.h, $(wildcard $(HPS_EVENT_HDR_DIR)/*.h))
HPS_EVENT_OBJ := $(patsubst $(HPS_EVENT_SRC_DIR)/%.cxx,$(OBJ)/%.o,$(HPS_EVENT_SRC))
HPS_EVENT_SO  := $(LIB)/libHpsEvent.so
HPS_EVENT_LINK_DEF := $(HPS_EVENT_HDR_DIR)/HpsEventLinkDef.h 

# HPS Event Dictionary
HPS_EVENT_DIC_DIR  := $(PWD)/root_dic
HPS_EVENT_DIC_SRC  := $(HPS_EVENT_DIC_DIR)/HpsEventDic.cxx
HPS_EVENT_DIC_OBJ  := $(OBJ)/HpsEventDic.o
ROOTCINT := rootcint

# Root Sources
ROOT_DIR	:= $(PWD)
ROOT_SRC	:= $(wildcard $(ROOT_DIR)/*.cxx)
ROOT_BIN	:= $(patsubst $(ROOT_DIR)/%.cxx, $(BIN)/%, $(ROOT_SRC))

# Example Sources
EXAMPLES_DIR := $(PWD)/examples
EXAMPLES_SRC := $(wildcard $(EXAMPLES_DIR)/*.cxx)
EXAMPLES_BIN := $(patsubst $(EXAMPLES_DIR)/%.cxx, $(BIN)/%, $(EXAMPLES_SRC))

# Default
all: dirs $(HPS_EVENT_OBJ) $(HPS_EVENT_DIC_OBJ) $(HPS_EVENT_SO) $(ROOT_BIN) $(EXAMPLES_BIN)
	@echo "HPS Event is done!"
	
# Clean
clean:
	@echo "Cleaning all directories ..."
	rm -rf $(OBJ)
	rm -rf $(BIN)
	rm -rf $(HPS_EVENT_DIC_DIR)
	rm -rf $(LIB)
	@echo "Done!"
	

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
	@echo "Generating dictionary $@ ..." 
	$(ROOTCINT) -f $@ -c $^ $(HPS_EVENT_LINK_DEF)

# Compile HPS Event ROOT dictionary
$(HPS_EVENT_DIC_OBJ): $(HPS_EVENT_DIC_SRC) $(HPS_EVENT_DIC_DIR)/HpsEventDic.h
	@echo "Compiling ROOT dictionary $@ ..."
	$(CC) $(CFLAGS) -c $< -o $@ 

# Create HPS Event ROOT library
$(HPS_EVENT_SO): $(HPS_EVENT_DIC_OBJ) $(HPS_EVENT_OBJ)
	@echo "Creating shared library $@ ..."
	$(CC) $(SOFLAGS) $^ -o $@
	
# Compile ROOT sources
$(BIN)/%: $(EXAMPLES_DIR)/%.cxx  
	$(CC) $(CFLAGS) $(LFLAGS) $(OBJ)/* -o $@ $<

