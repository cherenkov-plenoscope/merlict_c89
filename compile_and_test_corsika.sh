mv ./build/corsika/test_clang_c89 ./build/corsika/test_clang_c89.old

clang src/mli_corsika_test.c\
        -o build/corsika/test_clang_c89\
        -std=c89\
        -lm\
        -Wall\
        -pedantic\
        -Wextra\
        -Wstrict-prototypes

./build/corsika/test_clang_c89
