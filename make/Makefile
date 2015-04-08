#***********************************************************************
#* Description:                                                        *
#* ------------                                                        *
#* $Rev: 1370 $     Revision of last commit                             *
#* $Author: ettl $  Author of last commit                              *
#* $Date: 2015-02-18 10:04:42 +0100 (Wed, 18 Feb 2015) $               *
#*                                                                     *
#* Tested on Ubuntu Linux and Raspberry Pi (Raspbian)       		   *
#* Makefile to compile and build programs.               			   *
#*                                                                     *
#* Usage and basic features:                                           *
#*                                                                     *
#*  - make build  [creates the whole project]                          *
#*  - make clean  [removes all object and binary files]                *
#*  - make depend [detects header dependencies]                        *
#*  - make style  [formates the source, based on artistic style]       *
#*                                                                     *
#***********************************************************************

# Folder definitions
BIN_DIR 	= ../bin
OBJ_DIR 	= ../obj
SRC_DIR 	= ../src
SRC_EXT_DIR = ../srcext
SRC_TEST    = ../src-test
MAKE_DIR    = ../make
APP_NAME    = sample
TESTSUITE_DIR = $(SRC_EXT_DIR)/simple_testsuite
SHELL_MACROS_DIR = $(SRC_EXT_DIR)/simple_shell_macros
TOKENIZE_DIR = $(SRC_EXT_DIR)/simple_tokenize

# Activate all sanitizers at once, use SAN=yes
ifdef SAN
	ASAN=yes
	USAN=yes
	ifeq (,$(findstring $(CXX),clang++)) # clang specific checks
       ISAN=yes
    endif 
endif 
# To compile with the address sanitizer, use the option ASAN=yes
ifdef ASAN
	SANITIZE=-fsanitize=address 
endif 
# To compile with the undefined sanitizer, use the option  USAN=yes
ifdef USAN
	SANITIZE+=-fsanitize=undefined 
endif
# To compile with the integer sanitizer, use the option ISAN=yes
ifdef ISAN
	SANITIZE+=-fsanitize=integer
endif
# To compile with the memory sanitizer, use the option MSAN=yes
# In order to get detailed stack traces with line numbers, please make sure the llvm-symbolizer is set properly.
# For example: 
#  $ export MSAN_SYMBOLIZER_PATH=/home/martin/compiler/clang+llvm-3.6.0-rc2-x86_64-linux-gnu/bin/llvm-symbolizer 
ifdef MSAN
	SANITIZE+=-fsanitize=memory -fsanitize-memory-track-origins=2 -fno-omit-frame-pointer -fno-optimize-sibling-calls
endif

# C++ compiler 
CXX 		= g++
CXX_INCLUDE	= -I$(SRC_DIR) -I$(SRC_EXT_DIR) -I$(SRC_TEST) -I$(SHELL_MACROS_DIR) -I$(TESTSUITE_DIR) -I$(TOKENIZE_DIR)
CXX_OPT		= -O3
CXX_DEBUG	= $(SANITIZE)
CXX_WFLAGS	= -W -Wall -Wunused -Wshadow -Wextra -pedantic -Wno-write-strings -Wno-long-long -fno-strict-aliasing
CXXFLAGS 	= $(CXX_OPT) $(CXX_DEBUG) $(CXX_WFLAGS) $(CXX_INCLUDE)
CXX_CALL    = $(CXX) -c $(CXXFLAGS) -o $@ $< 

# C compiler
CC	        = gcc
CC_INCLUDE  = 
CC_OPT		= -O3
CC_DEBUG    = $(SANITIZE)
CC_WFLAGS	= -W -Wall -Wunused -Wextra -Wconversion -pedantic
CFLAGS 	    = $(CC_OPT) $(CC_DEBUG) $(CC_WFLAGS) -I$(CC_INCLUDE) 
CC_CALL     = $(CC) -c $(CFLAGS) -o $@ $< 

# Linker 
LINKER      = $(CXX) 
LDFLAGS     = $(SANITIZE)
LINKER_CALL = $(LINKER) -o $@ $^ $(LDFLAGS)

# Determine the number cores of the machine, where the makefile is executed.
# This helps to set the -j option (from make), to speedup the build
# process of 'large' projects.  
NPROCS=1
OS_TYPE:=$(shell uname -s)
OS_NAME:=$(shell uname -n)
ifeq ($(OS_TYPE),Linux)
	ifneq ($(OS_NAME),raspberrypi)
		NPROCS_TMP:=$(shell grep -c ^processor /proc/cpuinfo) 
		ifneq ($(NPROCS_TMP),0)
			NPROCS:=$(NPROCS_TMP)
		endif
	endif
endif

all: $(APP_NAME)_demo testrunner

$(APP_NAME)_demo: $(BIN_DIR)/$(APP_NAME)_demo

testrunner: $(BIN_DIR)/testrunner

# ============================================================
# LINKER-RULES
# ===========================================================
$(BIN_DIR)/$(APP_NAME)_demo: $(OBJ_DIR)/$(APP_NAME)_demo.o
	$(LINKER_CALL)
	
$(BIN_DIR)/testrunner: $(OBJ_DIR)/simple_testsuite.o\
                       $(OBJ_DIR)/testrunner.o\
                       $(OBJ_DIR)/test_$(APP_NAME).o
	$(LINKER_CALL)
# ===========================================================
# c++ - SOURCES
# ===========================================================
SRCS = $(SRC_DIR)/$(APP_NAME)_demo.cpp\
       $(TESTSUITE_DIR)/simple_testsuite.cpp\
       $(TESTSUITE_DIR)/testrunner.cpp\
       $(SRC_TEST)/test_$(APP_NAME).cpp

# ===========================================================
# c - SOURCES
# =========================================================== 
CSRCS = 

# ===========================================================
# SUPPORTING-RULES
# ===========================================================
build: mrproper depend 
	# after generating the dependencies it is necessary to call make again
	make -j$(NPROCS) all

clean:
	rm -f $(BIN_DIR)/*
	rm -f $(OBJ_DIR)/*

mrproper:
	rm -f $(BIN_DIR)/*
	rm -f $(OBJ_DIR)/*
	rm -f $(SRC_DIR)/*.bck
	rm -f $(SRC_DIR)/*.orig
	rm -f $(SRC_TEST)/*.bck
	rm -f $(SRC_TEST)/*.orig
	rm -f $(MAKE_DIR)/.Makefile

touch:
	@find $(SRC_DIR) -name "*.c*" -type f -exec touch {} \;
	@find $(SRC_DIR) -name "*.h*" -type f -exec touch {} \;
	@find $(SRC_EXT_DIR) -name "*.c*" -type f -exec touch {} \;
	@find $(SRC_EXT_DIR) -name "*.h*" -type f -exec touch {} \;
	@find $(SRC_TEST) -name "*.c*" -type f -exec touch {} \;
	@find $(SRC_TEST) -name "*.h*" -type f -exec touch {} \;
	
style:
	find $(SRC_TEST) -name "*.c*" -type f -exec astyle --style=ansi -S -N -w -Y -p -C -c {} \;
	find $(SRC_TEST) -name "*.h*" -type f -exec astyle --style=ansi -S -N -w -Y -p -C -c {} \;
	find $(SRC_DIR)  -name "*.c*" -type f -exec astyle --style=ansi -S -N -w -Y -p -C -c {} \;
	find $(SRC_DIR)  -name "*.h*" -type f -exec astyle --style=ansi -S -N -w -Y -p -C -c {} \;
	
lib:
	# not implemented
install:
	# not implemented
prepare:
	# not implemented

# compile and run all unit tests
check: build
	../bin/testrunner

# compile and run all unit tests using valgrind
memcheck: mrproper depend 
	# after generating the dependencies it is necessary to call make again
	make -j$(NPROCS) all CXX_OPT=-g CC_OPT=-g
	valgrind ../bin/testrunner

depend:
	@cat < /dev/null > makedep
	@cat < /dev/null > cmakedep
# First C++
	@for i in ${SRCS}; do \
		 (echo $$i; $(CXX) $(CXXFLAGS) -MM $$i >> makedep); done
#       Write full path information before each *.o-dependency
	@echo 's/.*\.o:/..\/obj\/&/g' > eddep
	@sed -f eddep makedep > makedep1
	@mv makedep1 makedep
	@rm eddep
#       Add the rule to each dependency
	@echo 'a' > eddep
	@echo '	$$(CXX_CALL)' >> eddep
	@echo '.' >> eddep
	@echo 'g/.*\.o:/i\' >> eddep
	@echo '	$$(CXX_CALL)\' >> eddep
	@echo '.' >> eddep
	@echo '1,1d' >> eddep
	@echo 'w' >> eddep
	@ed - makedep < eddep
	@rm eddep
# Then C
	@for i in ${CSRCS}; do \
		 (echo $$i; $(CC) $(CFLAGS) -MM $$i >> cmakedep); done
#       Write full path information before each *.o-dependency
	@echo 's/.*\.o:/..\/obj\/&/g' > ceddep
	@sed -f ceddep cmakedep > cmakedep1
	@mv cmakedep1 cmakedep
	@rm ceddep
##       Add the rule to each dependency
	@echo 'a' > ceddep
	@echo '	$$(CC_CALL)' >> ceddep
	@echo '.' >> ceddep
	@echo 'g/.*\.o:/i\' >> ceddep
	@echo '	$$(CC_CALL)\' >> ceddep
	@echo '.' >> ceddep
	@echo '1,1d' >> ceddep
	@echo 'w' >> ceddep
	@ed - cmakedep < ceddep
	@rm ceddep
	@cat < cmakedep >> makedep
	@rm cmakedep
	@echo '# DO NOT EDIT THIS FILE HERE.' > .Makefile.dep
	@cat < makedep >> .Makefile.dep
	@echo '# DO NOT EDIT THIS FILE HERE.' >> .Makefile.dep
	@rm makedep

.Makefile.dep: 
	@cat < /dev/null > .Makefile.dep

include .Makefile.dep
