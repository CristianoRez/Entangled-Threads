# Diretórios
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin
INC_DIR := include

# Arquivos-fonte
SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

# Compilador e flags
CXX := g++
CXXFLAGS := -Wall -Wextra -g3 -I$(INC_DIR)

# Nome do executável
EXEC := $(BIN_DIR)/main

# Regra padrão
all: $(EXEC)

# Linkagem final
$(EXEC): $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(OBJECTS) -o $@

# Compilação dos .cpp em .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Criar diretórios se não existirem
$(OBJ_DIR) $(BIN_DIR):
	mkdir -p $@

# Limpeza
clean:
	rm -rf $(OBJ_DIR)/*.o $(EXEC)

.PHONY: all clean