# Compilador y flags
CC = gcc
CFLAGS = -Wall -g -Iinclude
LDFLAGS = -lm

# Directorios
SRC_DIR = src
BIN_DIR = bin
OBJ_DIR = $(BIN_DIR)/obj

# Buscar todos los .c recursivamente en src y subdirectorios
SRC = $(shell find $(SRC_DIR) -name "*.c")
OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))

# Nombre del ejecutable
TARGET = $(BIN_DIR)/vmx

# Regla por defecto
all: $(TARGET)

# Cómo generar el ejecutable
$(TARGET): $(OBJ) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(OBJ) -o $@ $(LDFLAGS)

# Cómo generar cada .o a partir de un .c
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Crear directorios si no existen
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Limpiar compilación
clean:
	rm -rf $(OBJ_DIR) $(TARGET)
