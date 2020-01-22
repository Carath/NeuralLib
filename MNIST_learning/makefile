##########################################################
# Global compiler settings:
include ../compiler_settings.mk

##########################################################
# Settings:

# Executable name:
EXE = MNIST_learning

# Used static library name:
STATIC_LIB = ../NeuralLib

# Source and objects files location:
SRC_DIR = src
OBJ_DIR = obj

##########################################################

# Necessary for using clock_gettime():
POSIX_OPT = _POSIX_C_SOURCE=199309L

# For better performance:
PROCESSOR_ARCH = -march=native


# Used high performance library:
ifeq ($(HIGH_PERF_LIB), OPENBLAS)

	HIGH_PERF_HEAD_DIR = -I$(HIGH_PERF_PATH)/include
	HIGH_PERF_LIB_DIR = -L$(HIGH_PERF_PATH)/lib
	HIGH_PERF_LINKING = -lopenblas
endif


# N.B: clang can be used instead of gcc:
CC = gcc
CPPFLAGS =
CFLAGS = -std=c99 -Wall -O2 $(PROCESSOR_ARCH) -D$(POSIX_OPT) -D_$(NUMBER_TYPE) -D_$(HIGH_PERF_LIB) $(HIGH_PERF_HEAD_DIR)
LDFLAGS =
LDLIBS = $(STATIC_LIB).a $(HIGH_PERF_LIB_DIR) $(HIGH_PERF_LINKING) -lm


# Finding the source files and objects names:
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)


# The following names are not associated with files:
.PHONY: all clean

# All executables to be created:
all: $(EXE)


# Linking the program:
$(EXE): $(OBJ)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

# Compiling other source files:
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@


# Cleaning with 'make clean' the object files:
clean:
	rm -fv $(EXE) $(OBJ_DIR)/*