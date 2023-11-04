#!/bin/bash

# Configuration
PYTHON_VERSION=3.10  # Replace with your Python version
MODULE_NAME=tdd
LIB_FOLDER=lib

# List of source files
SOURCE_FILES=(
    # $MODULE_NAME.c
    python-c.cpp
    # Add new source files here
)

# Compile source files
for source_file in "${SOURCE_FILES[@]}"; do
    gcc -I/usr/include/python$PYTHON_VERSION -o $LIB_FOLDER/${source_file%}.o -c $source_file -fPIC
done

# Link object files into a shared library
gcc -o $LIB_FOLDER/$MODULE_NAME.so -shared $(printf "$LIB_FOLDER/%s " "${SOURCE_FILES[@]%.c}.o") -lpython$PYTHON_VERSION

echo "Compilation completed."