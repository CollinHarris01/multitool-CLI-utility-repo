toolkit

A minimal C++ command-line toolkit application built with CMake. This project is 
designed as the foundation for a growing collection of small, modular utilities 
accessible through a single command-line interface.

Features:
...


From project root directory: 
1.	Configure -> cmake --preset default
2.	Build -> cmake --build build/default
3.	Run -> ./build/default/toolkit
4.	Build Tests -> cmake --build build/default --target unit_tests
5.	Run TestS -> ctest --test-dir build/default