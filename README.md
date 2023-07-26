# CppGo

A Go (game) engine written in C++.

## Building
```
git clone https://github.com/TexanElite/cppgo
cd cppgo
cmake --no-warn-unused-cli -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE:STRING=Debug -S . -B build -G Ninja
cmake --build build --config Debug --target all --
```

## Notes

Written in C++20