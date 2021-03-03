##########################################################
# Global compiler settings:
include ../compiler_settings.mk

##########################################################
# Settings:

# Executable name:
EXE = animation

# Used static library name:
NEURAL_LIB = ../NeuralLib

# Source and objects files location:
SRC_DIR = src
OBJ_DIR = obj

# Creates the OBJ_DIR folder, if necessary:
$(shell mkdir -p $(OBJ_DIR))

##########################################################
# Libraries:

# Used high performance library:
ifeq ($(HIGH_PERF_LIB), OPENBLAS)

	HIGH_PERF_HEAD_DIR = -I$(HIGH_PERF_PATH)/include
	HIGH_PERF_LIB_DIR = -L$(HIGH_PERF_PATH)/lib
	HIGH_PERF_LINKING = -lopenblas
endif

# Graphical library, SDL2:
GRAPHIC_LIB = SDL2

ifeq ($(GRAPHIC_LIB), SDL2)

	GRAPHIC_FLAGS = `sdl2-config --cflags`
	GRAPHIC_LINKS = `sdl2-config --libs` -lSDL2_image -lSDL2_ttf
endif

##########################################################
# Compiler options:

# Necessary for using get_time():
POSIX_OPT = _POSIX_C_SOURCE=199309L

# For better performance:
PROCESSOR_ARCH = -march=native

# N.B: gcc for C, g++ for C++, alternative: clang.
CC = gcc
CPPFLAGS =
CFLAGS = -std=c99 -Wall -O2 $(PROCESSOR_ARCH) -D$(POSIX_OPT) -D_$(NUMBER_TYPE) -D_$(HIGH_PERF_LIB) $(HIGH_PERF_HEAD_DIR) $(GRAPHIC_FLAGS)
LDFLAGS =
LDLIBS = $(NEURAL_LIB).a $(HIGH_PERF_LIB_DIR) $(HIGH_PERF_LINKING) $(GRAPHIC_LINKS) -lm

##########################################################
# Compiling rules:

# The following names are not associated with files:
.PHONY: all clean

# All executables to be created:
all: $(EXE)

# Sources and objects files:
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Linking the program:
$(EXE): $(OBJ)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

# Compiling the source files:
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

##########################################################
# Cleaning with 'make clean' the object files:
clean:
	rm -fv $(EXE) $(OBJ_DIR)/*
