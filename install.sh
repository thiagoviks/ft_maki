#!/bin/bash

# Destination paths
INCLUDE_DIR="/usr/local/include"
LIB_DIR="/usr/local/lib"

# Source files
HEADER="include/ft_maki.h"
LIBRARY="libft_maki.a"

# Check if running as root
if [[ "$EUID" -ne 0 ]]; then
    echo "❌ This script must be run as root. Use: sudo ./install.sh"
    exit 1
fi

echo "📦 Installing ft_maki..."

# Check if files exist
if [[ ! -f "$HEADER" ]]; then
    echo "❌ Header $HEADER not found."
    exit 1
fi

if [[ ! -f "$LIBRARY" ]]; then
    echo "❌ Library $LIBRARY not found."
    exit 1
fi

# Copy the files
cp "$HEADER" "$INCLUDE_DIR/"
cp "$LIBRARY" "$LIB_DIR/"

# Update library cache, if necessary
ldconfig

echo "✅ ft_maki installed successfully!"
echo "You can now use:"
echo "#include <ft_maki.h>"
echo "and compile with: gcc main.c -lft_maki"
