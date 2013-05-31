#
#
# author:  Omar Moreno <omoreno1@ucsc.edu>
#		   Santa Cruz Institute for Particle Physics
#		   University of California, Santa Cruz
# date:    May 31, 2013 
# version: 1.0
#
# 	Simple Makefile which can be used to build analysis code
#

# Variables
CC		:= g++
CFLAGS	:= -O2 -Wall -fPIC -pthread -m64 `root-config --cflags` -I$(LCIO)/include -I$(HPS_DST_HOME)/utils -I$(HPS_DST_HOME)/include   
LFLAGS  := -L$(LCIO)/lib -llcioDict -llcio `root-config --libs`  
BIN		:= $(PWD)/bin
OBJ		:= $(HPS_DST_HOME)/.obj
SOFLAGS := -shared -O2 -m64

# File extension of c++ files
EXT := cxx

# Analysis Sources
ANALYSIS_DIR := $(PWD)
ANALYSIS_SRC := $(wildcard $(ANALYSIS_DIR)/*.$(EXT))
ANALYSIS_BIN := $(patsubst $(ANALYSIS_DIR)/%.$(EXT), $(BIN)/%, $(ANALYSIS_SRC))


# Default
all: dirs print $(ANALYSIS_BIN)
	@echo "Done compiling analysis!"
	
print: 
	@echo $(ANALYSIS_BIN)

# Clean
clean:
	@echo "Cleaning all directories ..."
	rm -rf $(OBJ)
	rm -rf $(BIN)
	@echo "Done!"
	

# Create obj and bin directories
dirs:
	test -d $(OBJ) || mkdir $(OBJ)
	test -d $(BIN) || mkdir $(BIN)

# Compile Ecal Analysis sources
$(BIN)/%: $(ANALYSIS_DIR)/%.cxx
	$(CC) $(CFLAGS) $(LFLAGS) $(OBJ)/* -o $@ $<
	@echo "Done compiling analysis/ sources" 

