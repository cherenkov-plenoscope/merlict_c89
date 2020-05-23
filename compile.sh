#!/bin/bash
# Build merlict with gcc and clang, look for warnings early.
# At some point this should go to .travis.yml

echo "gcc_c89"
gcc merlict-c89-test.c -o test_gcc_c89 -std=c89 -lm -Wall -pedantic -Wstrict-prototypes
./test_gcc_c89  > test_gcc_c89.o 2> test_gcc_c89.e
tail -n 1 test_gcc_c89.o

echo "gcc"
gcc merlict-c89-test.c -o test_gcc -lm -Wall -pedantic -Wstrict-prototypes
./test_gcc > test_gcc.o 2> test_gcc.e
tail -n 1 test_gcc.o

echo "g++"
g++ merlict-c89-test.c -o test_g++ -lm -Wall -pedantic
./test_g++ > test_g++.o 2> test_g++.e
tail -n 1 test_g++.o

echo "clang_c89"
clang merlict-c89-test.c -o test_clang_c89 -std=c89 -lm -Wall -pedantic -Wstrict-prototypes
./test_clang_c89 > test_clang_c89.o 2> test_clang_c89.e
tail -n 1 test_clang_c89.o

echo "clang"
clang merlict-c89-test.c -o test_clang -lm -Wall -pedantic -Wstrict-prototypes
./test_clang > test_clang.o 2> test_clang.e
tail -n 1 test_clang.o
