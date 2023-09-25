mv ./build/corsika/test_clang_c89 ./build/corsika/test_clang_c89.old

clang corsika/tests/mli_corsika_tests_entry_point.c\
        -o build/corsika/test_clang_c89\
        -std=c89\
        -lm\
        -Wall\
        -pedantic\
        -Wextra\
        -Wstrict-prototypes

./build/corsika/test_clang_c89
