#!/bin/bash
# Build merlict with gcc and clang, look for warnings early.

printf "prepare    "

scenery_ids=( 000 001 002 optics_prism optics_focussing_mirror )

for scenery_id in "${scenery_ids[@]}";
do
    tar -cvf \
    "./tests/resources/sceneries/$scenery_id.tar"\
    --directory \
    "./tests/resources/sceneries/$scenery_id"\
    .\
    > "./build/tar_$scenery_id.o" 2>&1

    tar_rc=$?
    printf " %d" $tar_rc
done
printf "\n"


printf "gcc c89     "
gcc mli_core/tests/main_entry_point.c\
        -o build/tests/mli_core/test_gcc_c89\
        -std=c89\
        -lm\
        -Wall\
        -pedantic\
        -Wextra\
        -Wstrict-prototypes
gcc_c89_rc=$?
./build/tests/mli_core/test_gcc_c89 > ./build/tests/mli_core/test_gcc_c89.o 2>&1
gcc_c89_test_rc=$?
printf "%d %d\n" $gcc_c89_rc $gcc_c89_test_rc

printf "gcc         "
gcc mli_core/tests/main_entry_point.c\
        -o build/tests/mli_core/test_gcc\
        -lm\
        -Wall\
        -pedantic\
        -Wextra\
        -Wstrict-prototypes
gcc_rc=$?
./build/tests/mli_core/test_gcc > ./build/tests/mli_core/test_gcc.o 2>&1
gcc_test_rc=$?
printf "%d %d\n" $gcc_rc $gcc_test_rc

printf "g++         "
g++ mli_core/tests/main_entry_point.c\
        -o build/tests/mli_core/test_g++\
        -lm\
        -Wall\
        -pedantic\
        -Wextra
gpp_rc=$?
./build/tests/mli_core/test_g++ > ./build/tests/mli_core/test_g++.o 2>&1
gpp_test_rc=$?
printf "%d %d\n" $gpp_rc $gpp_test_rc

printf "clang c89   "
clang mli_core/tests/main_entry_point.c\
        -o build/tests/mli_core/test_clang_c89\
        -std=c89\
        -lm\
        -Wall\
        -pedantic\
        -Wextra\
        -Wstrict-prototypes
clang_c89_rc=$?
./build/tests/mli_core/test_clang_c89 > ./build/tests/mli_core/test_clang_c89.o 2>&1
clang_c89_test_rc=$?
printf "%d %d\n" $clang_c89_rc $clang_c89_test_rc

printf "clang       "
clang mli_core/tests/main_entry_point.c\
        -o build/tests/mli_core/test_clang\
        -lm\
        -Wall\
        -pedantic\
        -Wextra
clang_rc=$?
./build/tests/mli_core/test_clang > ./build/tests/mli_core/test_clang.o 2>&1
clang_test_rc=$?
printf "%d %d\n" $clang_rc $clang_test_rc


if      [ $gcc_c89_test_rc -ne 0 ]
then
        printf "\n"
        cat "./build/tests/mli_core/test_gcc_c89.o"
fi

if      [ $gcc_test_rc -ne 0 ]
then
        printf "\n"
        cat "./build/tests/mli_core/test_gcc.o"
fi

if      [ $gpp_test_rc -ne 0 ]
then
        printf "\n"
        cat "./build/tests/mli_core/test_gpp.o"
fi

if      [ $clang_c89_test_rc -ne 0 ]
then
        printf "\n"
        cat "./build/tests/mli_core/test_clang_c89.o"
fi

if      [ $clang_test_rc -ne 0 ]
then
        printf "\n"
        cat "./build/tests/mli_core/test_clang.o"
fi

if      [ $tar_rc -ne 0 ] ||\
        [ $gcc_c89_rc -ne 0 ] ||\
        [ $gcc_c89_test_rc -ne 0 ]||\
        [ $gcc_rc -ne 0 ] ||\
        [ $gcc_test_rc -ne 0 ]||\
        [ $gpp_rc -ne 0 ] ||\
        [ $gpp_test_rc -ne 0 ]||\
        [ $clang_c89_rc -ne 0 ] ||\
        [ $clang_c89_test_rc -ne 0 ]||\
        [ $clang_rc -ne 0 ] ||\
        [ $clang_test_rc -ne 0 ]
then
    printf "\n"
    printf ".......VERSIONS.........\n"
    tar --version
    printf "........................\n"
    gcc --version
    printf "........................\n"
    g++ --version
    printf "........................\n"
    clang --version
    printf "........................\n"
    printf "\n"
    printf "....TAR verbose.........\n"

    for scenery_id in "${scenery_ids[@]}";
    do
        printf "\n%s.tar:\n" "$scenery_id"
        cat "./build/tests/mli_core/tar_$scenery_id.o"
    done

    exit 1
else
    exit 0
fi
