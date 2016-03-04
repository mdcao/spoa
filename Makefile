CP = g++
LD = g++
DX = doxygen

NAME = spoa

OBJ_DIR = obj
SRC_DIR = src
DOC_DIR = doc
EXC_DIR = bin

I_CMD = $(addprefix -I, $(SRC_DIR))
L_CMD = $(addprefix -L, )

CP_FLAGS = $(I_CMD) -O3 -Wall -std=c++11 -march=native
LD_FLAGS = $(I_CMD) $(L_CMD)

SRC = $(shell find $(SRC_DIR) -type f -regex ".*\.cpp")
OBJ = $(subst $(SRC_DIR), $(OBJ_DIR), $(addsuffix .o, $(basename $(SRC))))
DEP = $(OBJ:.o=.d)
EXC = $(NAME)
BIN = $(EXC_DIR)/$(EXC)
DOC = $(DOC_DIR)/DoxyFile

all: $(EXC)

install: bin

bin: $(BIN)

debug:

$(EXC): $(OBJ)
	@echo [LD] $@
	@mkdir -p $(dir $@)
	@$(LD) -o $@ $^ $(LD_FLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo [CP] $<
	@mkdir -p $(dir $@)
	@$(CP) $< -c -o $@ -MMD $(CP_FLAGS)

$(BIN): $(EXC)
	@echo [CP] $@
	@mkdir -p $(dir $@)
	@cp $< $@

docs:
	@echo [DX] generating documentation
	@$(DX) $(DOC)

clean:
	@echo [RM] cleaning
	@rm $(OBJ_DIR) $(EXC) -rf

remove:
	@echo [RM] removing
	@rm $(EXC_DIR) -rf

-include $(DEP)