mv ./build/tests/mli_corsika/test_clang_c89 ./build/tests/mli_corsika/test_clang_c89.old

clang mli_corsika/tests/main_entry_point.c\
        -o build/tests/mli_corsika/test_clang_c89\
        -std=c89\
        -lm\
        -Wall\
        -pedantic\
        -Wextra\
        -Wstrict-prototypes

./build/tests/mli_corsika/test_clang_c89
