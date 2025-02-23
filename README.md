# Hack Assembler

## About 

The Hack Assembler converts code written in the Hack assembly language to machine code that can be directly executed on the Hack computer. This project is one of the many enlightening projects from the book [The Elements of Computing Systems: Building a Modern Computer from First Principles](https://www.amazon.com/Elements-Computing-Systems-Building-Principles/dp/0262640686). The lessons in this book are also available as a two-part series of Massive Open Online Courses (M.O.O.C) on [Coursera](https://www.coursera.org/learn/build-a-computer). You can find out more about the Hack Computer and it's instruction set [here](https://en.wikipedia.org/wiki/Hack_computer).

## Usage

These instructions assume you haver the **GNU Make utility** and the **GNU Compiler Collection (GCC)** installed locally. If you don't, you can find download instructions [here](https://www.gnu.org/software/make/#download) and [here](https://gcc.gnu.org/install/download.html).

1. Clone this repo to your machine
    ```sh
      git clone git@github.com:MlkMahmud/hack-assembler.git && cd hack-assembler/
    ```
2. From the root directory, compile the source files (outputs an executable named **main** )
    ```sh
      make
    ```
3. Run the assembler
    ```sh
      ./main path_to_asm_file
    ```
    On success, the program generates a new file with the same name as your source file, but with a ".hack" extension instead of ".asm".