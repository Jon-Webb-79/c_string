#!/usr/bin/zsh
# ================================================================================
# ================================================================================
# - File:    install.zsh
# - Purpose: Install/Update c_string library files to system directories
#
# Source Metadata
# - Author:  Jonathan A. Webb
# - Date:    January 11, 2025
# - Version: 1.1
# - Copyright: Copyright 2025, Jon Webb Inc.
# ================================================================================
# ================================================================================

# Check if running with sudo/root permissions
if [ "$EUID" -ne 0 ]; then
    echo "Please run as root (use sudo)"
    exit 1
fi

# Detect OS
if [[ "$OSTYPE" == "darwin"* ]]; then
    # macOS paths
    INCLUDE_DIR="/usr/local/include"
    LIB_DIR="/usr/local/lib"
elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
    # Linux paths
    INCLUDE_DIR="/usr/include"
    LIB_DIR="/usr/lib"
else
    echo "Unsupported operating system"
    exit 1
fi

# Create directories if they don't exist
mkdir -p "$INCLUDE_DIR"
mkdir -p "$LIB_DIR"

# Create backup directory if needed
BACKUP_DIR="/tmp/c_string_backup"
mkdir -p "$BACKUP_DIR"

# Function to backup existing file
backup_file() {
    local file=$1
    local backup_path="$BACKUP_DIR/$(basename $file).$(date +%Y%m%d_%H%M%S)"
    if [ -f "$file" ]; then
        cp "$file" "$backup_path"
        echo "Backed up existing file to $backup_path"
    fi
}

# Install/Update header file
if [ -f "$INCLUDE_DIR/c_string.h" ]; then
    echo "Updating existing header file..."
    backup_file "$INCLUDE_DIR/c_string.h"
fi

cp ../../c_string/c_string.h "$INCLUDE_DIR"
if [ $? -eq 0 ]; then
    echo "Header file installed/updated successfully"
else
    echo "Error installing header file"
    exit 1
fi

# Install/Update source file
if [ -f "$LIB_DIR/c_string.c" ]; then
    echo "Updating existing source file..."
    backup_file "$LIB_DIR/c_string.c"
fi

cp ../../c_string/c_string.c "$LIB_DIR"
if [ $? -eq 0 ]; then
    echo "Source file installed/updated successfully"
else
    echo "Error installing source file"
    exit 1
fi

# Set permissions
chmod 644 "$INCLUDE_DIR/c_string.h"
chmod 644 "$LIB_DIR/c_string.c"

echo "Installation/Update completed successfully"
echo "Backups (if any) are stored in $BACKUP_DIR"
# ================================================================================
# ================================================================================
# eof
