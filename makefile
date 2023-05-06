# -----------//-----------
# This is a C++ makefile.|
# -----------//-----------



# whether or not to use precompiled headers (comment to disable)
# USE_PCH			:= 1



# directories
SRC_DIR 		:= src
INC_DIR 		:= include
OBJ_DIR 		:= obj
LIB_DIR 		:= lib
BIN_DIR 		:= bin



# files
SRC_FILES 		:= $(shell find $(SRC_DIR) -name "*.cpp" -o \
                                           -name "*.cxx" -o \
										   -name "*.c++" -o \
										   -name "*.cc")
OBJ_FILES		:= $(patsubst $(SRC_DIR)/%,$(OBJ_DIR)/%.o,$(SRC_FILES))
INC_FILES 		:= $(shell find $(INC_DIR) -name "*.hpp" -o \
										   -name "*.hxx" -o \
										   -name "*.h++" -o \
										   -name "*.hh"  -o \
										   -name "*.h")
PCH_INC_FILES 	:=

BIN 			:= $(BIN_DIR)/renderer



# compiler
CXX 			:= clang++

# language standard to be used
STD				:= c++20

# compiler flags
CXXFLAGS 		:= -Wall -Wextra -Wsign-conversion -pedantic-errors -std=$(STD)

# include flags
CXXFLAGS 		+= -I$(INC_DIR) -I$(LIB_DIR)/tiny_obj_loader/include
CXXFLAGS        += -I$(LIB_DIR)/toml11/include

# linker flags (e.g. -L/path/to/lib)
LDFLAGS			:=

# linker libraries (e.g. -lm)
LDLIBS			:=



ifdef USE_PCH

	ifeq (gcc, $(CXX))
		PCH_EXT 	:= gch
	else
		PCH_EXT 	:= pch
	endif

	PCH_INC_FILES 	+= $(patsubst %,%.$(PCH_EXT),$(INC_FILES))

endif



# profiles
release: CXXFLAGS += -O2 -DNDEBUG -g3 -ggdb
release: all

debug: CXXFLAGS += -O0 -g3 -ggdb
debug: all



# make default goal (using make with no specified recipe)
#.DEFAULT_GOAL := debug
.DEFAULT_GOAL := release

all: $(BIN)

rbuild: clean release
dbuild: clean debug

$(BIN): $(PCH_INC_FILES) $(OBJ_FILES)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(filter %.o,$^) -o $@ $(LDFLAGS) $(LDLIBS)

# generate each object file according to the corresponding source file
# create directories as needed
# https://www.gnu.org/software/make/manual/make.html#Static-Pattern
$(OBJ_FILES): $(OBJ_DIR)/%.o : $(SRC_DIR)/%
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(PCH_INC_FILES): $(INC_DIR)/%.$(PCH_EXT) : $(INC_DIR)/%
	$(CXX) $(CXXFLAGS) -x c++-header $< -o $@


# 'clean' doesn't represent an actual file generating recipe
.PHONY: clean

clean:
	-rm -rf $(OBJ_DIR)
	-rm -rf $(BIN_DIR)
	-rm -rf *.out *.ppm
ifdef PCH_INC_FILES
	-rm -rf $(PCH_INC_FILES)
endif



# Valgrind flags
VALGFLAGS 		:= --leak-check=full --show-leak-kinds=all --track-origins=no --verbose

# Valgrind log file
VALG_OUT 		:= valgrind_log.out

valg_check: $(VALG_OUT)

$(VALG_OUT): $(BIN)
	valgrind $(VALGFLAGS) --log-file=$@ $^ $(ARGS)
