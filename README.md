# CYK

![C/C++ CI (Ubuntu)](https://github.com/tobias-wilfert/CYK/workflows/C/C++%20CI%20(Ubuntu)/badge.svg)
![CMake (Windows)](https://github.com/tobias-wilfert/CYK/workflows/CMake%20(Windows)/badge.svg)

## Description:

A C++ implementation of the [CYK algorithm](https://en.wikipedia.org/wiki/CYK_algorithm) that will generate an HTML representation of the constrcuted table.

## How to use:

A script ```compile.sh``` is provided to build the CYK aswell as a script ```test.sh``` to test the CYK.

The first paramter of the program should be the path to the Grammar to use in form of a json file (for an example see ```Grammar.json```) followed by a variable number of strings to simulate.

For example outputs see ```./Examples``` these were generated using the ```test.sh``` script.
