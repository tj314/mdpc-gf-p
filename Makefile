SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := build
OUT_EXE := $(BIN_DIR)/mdpc_gf_p

SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS := $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

CXX := g++
CXXFLAGS := -std=c++17 -Wall -Werror -Wextra

CPPFLAGS := -I/usr/include/flint -Iinclude -MMD -MP # -MDD -MP generate header deps automatically
LDFLAGS := -lflint -lgmp

.PHONY: all clean

all: $(OUT_EXE)

$(OUT_EXE): $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	@$(RM) -rv $(BIN_DIR) $(OBJ_DIR)

-include $(OBJ:.o=.d)

# this Makefile was largely taken from here https://stackoverflow.com/a/30602701
