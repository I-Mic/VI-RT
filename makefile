# -----------//-----------
# This is a C++ makefile.|
# -----------//-----------



# whether or not to use precompiled headers (set to 'true' to use)
USE_PCH			:= false



# directories
SRC_DIR 		:= src
OBJ_DIR 		:= obj
BIN_DIR 		:= bin


INC_PATHS 		:= $(SRC_DIR) lib/tiny_obj_loader/include lib/toml11/include


# files
SRC_FILES 		:= $(shell find $(SRC_DIR) -name "*.cpp" -o \
                                           -name "*.cxx" -o \
										   -name "*.c++" -o \
										   -name "*.cc")
OBJ_FILES		:= $(patsubst $(SRC_DIR)/%,$(OBJ_DIR)/%.o,$(SRC_FILES))
INC_FILES 		:= $(shell find $(INC_PATHS) -name "*.hpp" -o \
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
CXXFLAGS 		:= -Wall -Wextra -Wsign-conversion -pedantic-errors -std=$(STD) -g3
CXXFLAGS        += $(foreach path,$(INC_PATHS),-I$(path))

# linker flags (e.g. -L/path/to/lib)
LDFLAGS			:=

# linker libraries (e.g. -lm)
LDLIBS			:=



ifeq (true, $(USE_PCH))

	ifeq (gcc, $(CXX))
		PCH_EXT 	:= gch
	else
		PCH_EXT 	:= pch
	endif

	PCH_INC_FILES 	+= $(patsubst %,%.$(PCH_EXT),$(INC_FILES))

endif



# profiles
release: CXXFLAGS += -O2 -DNDEBUG
release: all

debug: CXXFLAGS += -O0 -fsanitize=address
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

$(PCH_INC_FILES): %.$(PCH_EXT) : %
	$(CXX) $(CXXFLAGS) -x c++-header $< -o $@


# 'clean' doesn't represent an actual file generating recipe
.PHONY: clean

clean:
	-rm -rf $(OBJ_DIR) $(BIN_DIR) $(PCH_INC_FILES)
	-rm -rf *.out *.ppm
