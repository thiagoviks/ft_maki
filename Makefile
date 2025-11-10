NAME = ft_maki
SRC_DIR = src
OBJ_DIR = build
BC_DIR = bitcode
INCLUDE_DIR = include
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))
BC = $(patsubst $(SRC_DIR)/%.c, $(BC_DIR)/%.bc, $(SRC))
CC = clang-14
# Use LLVM 14 tools (matching IKOS installation)
CLANG_BC = clang-14
LLVM_LINK = llvm-link-14
CFLAGS = -Wall -Wextra -Werror -fPIC -I$(INCLUDE_DIR)
# Use -O0 and disable fast-math to avoid unsupported instructions
BCFLAGS = -c -emit-llvm -g -O0 -fno-fast-math -I$(INCLUDE_DIR)

.PHONY: all clean fclean re test install ikos ikos-analyze clean-bc

all: $(OBJ_DIR) lib$(NAME).a lib$(NAME).so

$(OBJ_DIR):
	mkdir -p $@

$(BC_DIR):
	mkdir -p $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BC_DIR)/%.bc: $(SRC_DIR)/%.c | $(BC_DIR)
	$(CLANG_BC) $(BCFLAGS) $< -o $@

lib$(NAME).a: $(OBJ)
	ar rcs $@ $^

lib$(NAME).so: $(OBJ)
	$(CC) -shared -Wl,-z,defs -o $@ $^

# Generate LLVM bitcode files
ikos: $(BC_DIR) clean-bc $(BC)
	@echo "✅ LLVM bitcode files generated in $(BC_DIR)/"
	@echo "Generated files:"
	@ls -1 $(BC_DIR)/
	@echo ""
	@echo "To analyze with IKOS, run:"
	@echo "  make ikos-analyze"

# Clean only bitcode files (to force regeneration)
clean-bc:
	rm -rf $(BC_DIR)/*.bc

# Analyze with IKOS (requires llvm-link)
ikos-analyze: ikos
	@echo "Running IKOS analysis..."
	@if [ -n "$(LLVM_LINK)" ]; then \
		echo "Using: $(LLVM_LINK)"; \
		$(LLVM_LINK) $(BC) -o $(BC_DIR)/combined.bc; \
		ikos $(BC_DIR)/combined.bc -o ikos_output.db; \
		echo ""; \
		echo "✅ Analysis complete! Results saved to ikos_output.db"; \
		echo "To view results, run:"; \
		echo "  ikos-view ikos_output.db"; \
	else \
		echo "❌ llvm-link not found in PATH."; \
		echo "Try: make ikos-simple (doesn't require llvm-link)"; \
	fi

# Analyze without llvm-link (simpler, but less comprehensive)
ikos-simple: ikos
	@echo "Running IKOS analysis on all files together..."
	ikos $(BC) -o ikos_output.db
	@echo ""
	@echo "✅ Analysis complete! Results saved to ikos_output.db"
	@echo "To view results, run:"
	@echo "  ikos-view ikos_output.db"

# Alternative: analyze each file separately
ikos-individual: ikos
	@echo "Analyzing each file individually..."
	@for bc in $(BC); do \
		echo "Analyzing $$bc..."; \
		ikos $$bc; \
	done
	@echo "✅ Individual analysis complete!"

clean:
	rm -rf $(OBJ_DIR) $(BC_DIR)

fclean: clean
	rm -f lib$(NAME).a lib$(NAME).so main ikos_output.db

re: fclean all

test: all
	$(CC) main.c -I$(INCLUDE_DIR) -L. -l$(NAME) -o main -no-pie

install: all
	sudo cp lib$(NAME).a /usr/local/lib/
	sudo cp lib$(NAME).so /usr/local/lib/
	sudo cp $(INCLUDE_DIR)/ft_maki.h /usr/local/include/
	sudo ldconfig
	@echo "✅ ft_maki installed successfully!"
	@echo "You can now use:"
	@echo "#include <ft_maki.h>"
	@echo "and compile with: gcc main.c -lft_maki"


#grep -n "float\|double" src/*.c include/*.h
#to verify float and double in my whole codebase