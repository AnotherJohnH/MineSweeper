# Mine Sweeper

A game from the past. This version is intended as an example of the use of the
[Platform](https://github.com/AnotherJohnH/Platform) GUI C++ API.

Try a browser hosted version of the application [here](https://anotherjohnh.github.io/MineSweeper/mines.html)

## Dependencies

+ https://github.com/AnotherJohnH/Platform  (pulled in at checkout)
+ SDL2
+ cmake and UNIX make or auto detection of ninja if installed

## Checkout

This repo uses git sub-modules, so checkout using --recurse to clone all the
dependent source...

    git clone --recurse https://github.com/AnotherJohnH/MineSweeper.git

or

    git clone --recurse ssh://git@github.com/AnotherJohnH/MineSweeper.git

## Build

Developed on MacOS but should build on Linux too. Indirect build with
`cmake` and `make` or `ninja`...

    make

Direct build using `cmake` and `make`...

    mkdir build
    cd build
    cmake ..
    make

## Run

After a build binary will be found here `buid/native/maze`. Command line interface...

    NAME
         MineSweeper - An old game

    SYNOPSIS
         mines [options] 

    OPTIONS
         -v,--version             Display version information
         -h,--help                Display this help
         -l,--level <unsigned>    Level of difficulty 1..3 [1]
