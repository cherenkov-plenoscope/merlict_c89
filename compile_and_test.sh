#!/bin/bash
# Build merlict with gcc and clang, look for warnings early.

printf -- "prepare    "
for scenery_id in 000 001
do
    tar -cf \
    ./tests/resources/sceneries/$scenery_id.tar\
    --directory \
    ./tests/resources/sceneries/$scenery_id\
    --sort name\
    .

    tar_rc=$?
    printf -- " $tar_rc"
done
    printf -- "\n"


printf -- "gcc c89     "
gcc merlict-c89-test.c\
        -o build/test_gcc_c89\
        -std=c89\
        -lm\
        -Wall\
        -pedantic\
        -Wextra\
        -Wstrict-prototypes
gcc_c89_rc=$?
./build/test_gcc_c89 > ./build/test_gcc_c89.o 2> ./build/test_gcc_c89.e
gcc_c89_test_rc=$?
printf -- "$gcc_c89_rc $gcc_c89_test_rc\n"

printf -- "gcc         "
gcc merlict-c89-test.c\
        -o build/test_gcc\
        -lm\
        -Wall\
        -pedantic\
        -Wextra\
        -Wstrict-prototypes
gcc_rc=$?
./build/test_gcc > ./build/test_gcc.o 2> ./build/test_gcc.e
gcc_test_rc=$?
printf -- "$gcc_rc $gcc_test_rc\n"

printf -- "g++         "
g++ merlict-c89-test.c\
        -o build/test_g++\
        -lm\
        -Wall\
        -pedantic\
        -Wextra
gpp_rc=$?
./build/test_g++ > ./build/test_g++.o 2> ./build/test_g++.e
gpp_test_rc=$?
printf -- "$gpp_rc $gpp_test_rc\n"

printf -- "clang c89   "
clang merlict-c89-test.c\
        -o build/test_clang_c89\
        -std=c89\
        -lm\
        -Wall\
        -pedantic\
        -Wextra\
        -Wstrict-prototypes
clang_c89_rc=$?
./build/test_clang_c89 > ./build/test_clang_c89.o 2> ./build/test_clang_c89.e
clang_c89_test_rc=$?
printf "$clang_c89_rc $clang_c89_test_rc\n"

printf -- "clang       "
clang merlict-c89-test.c\
        -o build/test_clang\
        -lm\
        -Wall\
        -pedantic\
        -Wextra
clang_rc=$?
./build/test_clang > ./build/test_clang.o 2> ./build/test_clang.e
clang_test_rc=$?
printf "$clang_rc $clang_test_rc\n"


if      [ "$gcc_c89_test_rc" -ne 0 ]
then
        (cat "./build/test_gcc_c89.o" && cat "./build/test_gcc_c89.e")
fi

if      [ "$gcc_test_rc" -ne 0 ]
then
        (cat "./build/test_gcc.o" && cat "./build/test_gcc.e")
fi

if      [ "$gpp_test_rc" -ne 0 ]
then
        (cat "./build/test_gpp.o" && cat "./build/test_gpp.e")
fi

if      [ "$clang_c89_test_rc" -ne 0 ]
then
        (cat "./build/test_clang_c89.o" && cat "./build/test_clang_c89.e")
fi

if      [ "$clang_test_rc" -ne 0 ]
then
        (cat "./build/test_clang.o" && cat "./build/test_clang.e")
fi

if      [ "$tar_rc" -ne 0 ] ||\
        [ "$gcc_c89_rc" -ne 0 ] ||\
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
else
        exit 0
fi
