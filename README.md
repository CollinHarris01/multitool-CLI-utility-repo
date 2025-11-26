toolkit

A minimal C++ command-line toolkit application built with CMake. This project is 
designed as the foundation for a growing collection of small, modular utilities 
accessible through a single command-line interface.

Features:
1. "search" command searches all files in a directory for specified substring pattern.
    Structure -> toolkit search [path] [pattern] --case --recursive --verbose
    --case flag toggles case sensitivity
    --recursive flag toggles recursive directory search
    --verbose flag toggles output to show matches per file
2. "stats" command searches a directory or file for contents and size statistics.
    Structure -> toolkit stats [path] 
3. "hash" command computes SHA-256 values for a file or all files in a directory.
    Structure -> toolkit hash [path] --recursive
    --recursive flag toggles recursive directory hashing
4. "copy" command copies a source file or directory to another location.
    Structure -> toolkit copy [source] [destination] --force --recursive
    --force flag overwrites destination files if they already exist
    --recursive flag enables recursive directory copying
5. "move" command moves a file or directory from one location to another.
    Structure -> toolkit move [source] [destination] --force --recursive
    --force flag overwrites desination if it already exists
    --recursive flag allows recursive directory moving
6. "remove" command deletes a file or directory.
    Structure -> toolkit remove [path] --force --recursive
    --force flag deletes without the confirmation prompt
    --recursive flag required to recursively deleted directories
7. "tree" command displays a directory's structure as an ASCII tree.
    Structure -> toolkit tree [path] --dirs-only
    --dirs-only flag displays only subdirectories within the tree

From project root directory: 
1.	Configure -> cmake --preset default
2.	Build -> cmake --build build/default
3.	Run -> ./build/default/toolkit
4.	Build Tests -> cmake --build build/default --target unit_tests
5.	Run TestS -> ctest --test-dir build/default