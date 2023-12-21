## Installation Guide

This project has several dependencies that need to be installed manually. Below are the instructions for both Linux and Windows systems.

### Dependencies
- CMake >= 3.20.0
- XTL >= 0.7.5
- XTENSOR >= 0.24.0
- Graphviz >= 2.43.0
- XTENSOR-Python >= 0.26.0
- Pybind11 >= 2.12.0
- Numpy >= 1.20.0

### Linux

1. **CMake**: 
sudo apt-get install cmake


2. **XTL**:
- Download the latest version from [XTL Releases](https://github.com/xtensor-stack/xtl/releases).
- Unpack and install:
  ```
  tar -xzvf xtl-0.7.5.tar.gz
  cd xtl-0.7.5
  sudo make install
  ```

3. **XTENSOR**:
- Download from [XTENSOR Releases](https://github.com/xtensor-stack/xtensor/releases).
- Install similarly to XTL.

4. **Graphviz**:
sudo apt-get install graphviz


5. **XTENSOR-Python**:
- Requires `pip` for installation:
  ```
  pip install xtensor-python==0.26.0
  ```

6. **Pybind11**:
sudo apt-get install pybind11-dev


7. **Numpy**:
pip install numpy==1.20.0


### Windows

1. **CMake**:
- Download and install from [CMake Downloads](https://cmake.org/download/).

2. **XTL, XTENSOR, XTENSOR-Python, Pybind11**:
- It's recommended to use vcpkg or conan for these libraries. Here's an example with vcpkg:
  ```
  .\vcpkg install xtl:x64-windows
  .\vcpkg install xtensor:x64-windows
  .\vcpkg install pybind11:x64-windows
  ```
- For XTENSOR-Python, follow the Linux pip installation steps.

3. **Graphviz**:
- Download and install from [Graphviz Downloads](https://graphviz.org/download/).

4. **Numpy**:
- Install using pip in Command Prompt:
  ```
  pip install numpy==1.20.0
  ```

Please ensure that all dependencies are installed correctly to avoid any issues during the build process.
