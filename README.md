# Bitc-Compiler
### Simple Compiler for BITC

## Project Details
That's a project I've created as a university homework for the course, CMPE 230 - Systems Programming on Nov 4th, 2017.

What is BITC and what the project was about can be found via [Project.pdf](https://github.com/corupta/Bitc-Compiler/blob/master/Project.pdf)

Basically, that's a cpp project that reads a .bc file (Code written in a simple programming language, BITC) and generates an a86 assembly .asm file for that code. The .asm file can be compiled with the [A86 Compiler](http://www.eji.com/a86/) on Windows XP or on DOSBox.

## Compile
The "bitc.cpp" file in either for [Windows](https://github.com/corupta/Bitc-Compiler/blob/master/Windows/bitc.cpp) or [Linux](https://github.com/corupta/Bitc-Compiler/tree/master/Ubuntu) (Tested only with Ubuntu, but it should work the same on any Linux) can be compiled with the terminal command "g++ bit.cpp -o bitc"

### On Windows:
* Download [bitc.cpp](https://github.com/corupta/Bitc-Compiler/blob/master/Windows/bitc.cpp) or [Linux](https://github.com/corupta/Bitc-Compiler/blob/master/Windows/bitc.cpp)
* Open a Windows Command Prompt (type cmd on Start Menu or Press Windows + R and type cmd)
* Change directory in the command prompt to the place where the bit.cpp is downloaded.
* Type _g++ bit.cpp -o bitc_ on the command prompt to compile the project.
* Now, you can use the created "bitc" executable to compile .bc files. Check out below "Run" section.

### On Ubuntu:
* Download [bitc.cpp](https://github.com/corupta/Bitc-Compiler/blob/master/Windows/bitc.cpp) or [Linux](https://github.com/corupta/Bitc-Compiler/blob/master/Ubuntu/bitc.cpp)
* Open a Terminal Window (Press Ctrl + Alt + T or Press super button and type Terminal)
* Change directory in the terminal to the place where the bit.cpp is downloaded.
* Type _g++ bit.cpp -o bitc_ on the command prompt to compile the project.
* Now, you can use the created "bitc" executable to compile .bc files. Check out below "Run" section.

## Run
You can compile .bc (BITC files) by using the command “./bitc [input_file].bc” and it should create a file “[input_file].asm”
The outputted .asm file can be used with [A86 Compiler](http://www.eji.com/a86/) or another A86 compiler to run.

For more details about how to use the project, check out [Report-Documentation.pdf](https://github.com/corupta/Bitc-Compiler/blob/master/Report-Documentation.pdf)
