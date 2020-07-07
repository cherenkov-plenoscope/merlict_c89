#!/bin/bash
# Build merlict with gcc and clang, look for warnings early.
# At some point this should go to .travis.yml

echo "__gcc_c89__"
gcc merlict-c89-test.c\
        -o test_gcc_c89\
        -std=c89\
        -lm\
        -Wall\
        -pedantic\
        -Wextra\
        -Wstrict-prototypes
gcc_c89_rc=$?
./test_gcc_c89 > test_gcc_c89.o 2> test_gcc_c89.e
gcc_c89_test_rc=$?
echo "compile" $gcc_c89_rc "test" $gcc_c89_test_rc

echo "__gcc__"
gcc merlict-c89-test.c\
        -o test_gcc\
        -lm\
        -Wall\
        -pedantic\
        -Wextra\
        -Wstrict-prototypes
gcc_rc=$?
./test_gcc > test_gcc.o 2> test_gcc.e
gcc_test_rc=$?
echo "compile" $gcc_rc "test" $gcc_test_rc

echo "__g++__"
g++ merlict-c89-test.c\
        -o test_g++\
        -lm\
        -Wall\
        -pedantic\
        -Wextra
gpp_rc=$?
./test_g++ > test_g++.o 2> test_g++.e
gpp_test_rc=$?
echo "compile" $gpp_rc "test" $gpp_test_rc


echo "__clang_c89__"
clang merlict-c89-test.c\
        -o test_clang_c89\
        -std=c89\
        -lm\
        -Wall\
        -pedantic\
        -Wextra\
        -Wstrict-prototypes
clang_c89_rc=$?
./test_clang_c89 > test_clang_c89.o 2> test_clang_c89.e
clang_c89_test_rc=$?
echo "compile" $clang_c89_rc "test" $clang_c89_test_rc

echo "__clang__"
clang merlict-c89-test.c\
        -o test_clang\
        -lm\
        -Wall\
        -pedantic\
        -Wextra
clang_rc=$?
./test_clang > test_clang.o 2> test_clang.e
clang_test_rc=$?
echo "compile" $clang_rc "test" $clang_test_rc


if      [ "$gcc_c89_rc" -ne 0 ] ||\
        [ "$gcc_c89_test_rc" -ne 0 ]||\
        [ "$gcc_rc" -ne 0 ] ||\
        [ "$gcc_test_rc" -ne 0 ]||\
        [ "$gpp_rc" -ne 0 ] ||\
        [ "$gpp_test_rc" -ne 0 ]||\
        [ "$clang_c89_rc" -ne 0 ] ||\
        [ "$clang_c89_test_rc" -ne 0 ]||\
        [ "$clang_rc" -ne 0 ] ||\
        [ "$clang_test_rc" -ne 0 ]
then
        exit 1
fi
