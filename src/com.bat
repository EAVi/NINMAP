rem This is a clunky way to compile using the Visual Studio cl command
rem It will compile the ninmap executable
cl /EHsc -o ninmap.exe algorithm.cpp main.cpp flag.cpp compiler.cpp token.cpp expression.cpp