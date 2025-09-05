# Compilador y flags
CC = gcc
CFLAGS = -Wall -g -Iinclude

# Directorios
SRC_DIR = src
BIN_DIR = bin

# Archivos fuente y objetos
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:.c=.o)

# Nombre del ejecutable
TARGET = $(BIN_DIR)/vmx

# Regla por defecto
all: $(TARGET)

# Cómo generar el ejecutable
$(TARGET): $(OBJ) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(OBJ) -o $@

# Cómo generar cada .o a partir de un .c
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Crear directorio bin si no existe
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Limpiar compilación
clean:
	rm -f $(OBJ) $(TARGET)
