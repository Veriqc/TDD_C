#!/bin/bash

# Remove the "build" directory and its contents
rm -rf build

# Create the "build" directory
mkdir build

# Change to the "build" directory
cd build

# Run CMake to generate Makefiles
cmake ..

# Build your project
cmake --build .

# Pause to keep the terminal open (optional)
read -p "Press Enter to exit..."
