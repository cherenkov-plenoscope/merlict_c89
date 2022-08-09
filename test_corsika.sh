mv ./build/corsika/test_clang_c99 ./build/corsika/test_clang_c99.old

clang merlict/mli_corsika_test.c\
        -o build/corsika/test_clang_c99\
        -std=c99\
        -lm\
        -Wall\
        -pedantic\
        -Wextra\
        -Wstrict-prototypes

./build/corsika/test_clang_c99
