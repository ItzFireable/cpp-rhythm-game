Builds:
[![C/C++ CI](https://github.com/ItzFireable/cpp-rhythm-game/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/ItzFireable/cpp-rhythm-game/actions/workflows/c-cpp.yml)

# Compiling (Windows)

- Install MSYS2 (https://github.com/msys2/msys2-installer/releases)
- Open the ``mingw64.exe`` from the msys2 folder (by default, C:\msys2), it will fail to compile with the other executables.
- Run this command to install required packages:
```
pacman -S --needed base-devel mingw-w64-x86_64-toolchain mingw-w64-x86_64-lua mingw-w64-x86_64-glew
```
- Run these commands to compile:
```
git clone https://github.com/tredu/projektityo2022-KimiJok1.git
cd projektityo2022-KimiJok1
make
```

# Credits

- Code by ItzFireable
- Important things by Velcrow the almighty the creator of penis shader
