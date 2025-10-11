# Compilador y flags
CC = gcc
CFLAGS = -Wall -g -Iinclude
LDFLAGS = -lm

# Directorios
SRC_DIR = src
BIN_DIR = bin
OBJ_DIR = $(BIN_DIR)/obj

# Buscar todos los .c recursivamente - versión simple
rwildcard = $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))
SRC = $(call rwildcard,$(SRC_DIR)/,*.c)

OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))

# Nombre del ejecutable
TARGET = $(BIN_DIR)/vmx.exe

# Regla por defecto
all: $(TARGET)

# Cómo generar el ejecutable
$(TARGET): $(OBJ) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(OBJ) -o $@ $(LDFLAGS)

# Cómo generar cada .o a partir de un .c
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Crear directorios si no existen
$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# Limpiar compilación
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all clean