# merlict_c89 [![Build Status](https://travis-ci.org/cherenkov-plenoscope/merlict_development_kit.svg?branch=master)](https://travis-ci.org/cherenkov-plenoscope/merlict_c89)
We :hearts: light! 

## Features

### Scenery
Represent your geometry with triangle-meshes. merlict also supports few surfaces common in optics, such as sperical caps.  Use triangle-meshes to approximate your geometry. When you need more performance, or run out of memory, use existing primitives, or add your own primitives.

| Primitives                 |             |
|----------------------------|:-----------:|
| Triangle                   | Native design for large meshes |
| Spherical cap, hexagonal   |             |
| Sphere                     |             |
| Cylinder                   |             |
| Disc                       |             |
| Plane, bi-circular         |             |

Define your scenery in '''json''' 

### Photons

### Rendering

## Tests
#### Compile
```
gcc merlict-c89-test.c -o merlict-c89-test -std=c89 -lm -Wall -pedantic
```

#### Run
```
./merlict-c89-test
```

## Viewer
A terminal-viewer using ANSI-escape-sequences.
#### Compile
```
gcc merlict-c89-view.c -o merlict-c89-view -std=c89 -lm -Wall -pedantic
```

#### Run
```
./merlict-c89-view my_scenery.mli.tmp
```
Here ``` my_scenery.mli.tmp``` is scenery for testing created by ```./merlict-c89-test```.

### Not sure where this is going to...
Exploring the merlict raytracing-project rewritten in C89 aka. C90, aka. ANSI-C.
This project might never be finished. It is just an exploration to find out what it feels like to rewrite the merlict raytracing-project in ANSI-C aka. C89.
Reasons in no particular order:

- Better compatibility compared to C++11
- Hopefully better wrappable with python using e.g. SWIG
- Learning new (oldschool) concepts beside todays widespread classes

I have the feeling that esspecially classes and inheritance cause most of the current issues in the merlict raytracer which make e.g. python wrapping very difficult. 
