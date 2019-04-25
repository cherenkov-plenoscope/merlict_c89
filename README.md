# merlict_c89
Exploring the merlict raytracing-project rewritten in C89 aka. ANSI-C

### Not sure where this is going to...
This project might never be finished. It is just an exploration to find out what it feels like to rewrite the merlict raytracing-project in ANSI-C aka. C89.
Reasons in no particular order:

- Better compatibility compared to C++11
- Hopefully better wrappable with python using e.g. SWIG
- Learning new (oldschool) concepts beside todays widespread classes

I have the feeling that esspecially classes and inheritance cause most of the current issues in the merlict raytracer which make e.g. python wrapping very difficult. 


### compile
```
gcc test.c -o test -std=c89 -lm -Wall -pedantic
```

### run tests
```
./test
```
