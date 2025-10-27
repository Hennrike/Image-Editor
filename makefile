# Compiler & Flags
CC      := gcc
CFLAGS  := -Wall -Wextra -O2 -std=c17
LDFLAGS :=
BUILD   := build
TARGET  := app

# Source Files
SRC     := $(wildcard src/*.c)
OBJ     := $(SRC:src/%.c=$(BUILD)/%.o)
DEP     := $(OBJ:.o=.d)

# Default Target
all: $(TARGET)

# Link final binary
$(TARGET): $(OBJ)
	@echo "Linking $@"
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

# Compile source files to object files
$(BUILD)/%.o: src/%.c | $(BUILD)
	@echo "Compiling $<"
	$(CC) $(CFLAGS) -MMD -c $< -o $@

# Ensure build directory exists
$(BUILD):
	@mkdir -p $(BUILD)

# If dependency files exist, include them
-include $(DEP)

# Run the app
run: all
	./$(TARGET)

# Clean build artifacts
clean:
	@echo "Cleaning project..."
	@if exist $(BUILD) rmdir /S /Q $(BUILD)
	@if exist $(TARGET).exe del /Q $(TARGET).exe

# Helpful debug info
print:
	@echo "SRC: $(SRC)"
	@echo "OBJ: $(OBJ)"
	@echo "DEP: $(DEP)"

.PHONY: all clean run print
