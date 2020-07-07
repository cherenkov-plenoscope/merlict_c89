![merlict c89 logo](/readme/merlict_c89_logo_wide_32by9_low.jpg)

[![Build Status](https://travis-ci.org/cherenkov-plenoscope/merlict_development_kit.svg?branch=master)](https://travis-ci.org/cherenkov-plenoscope/merlict_c89)
[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

Merlict simulates light. It propagates photons in a scenery, and it can render simple images. Merlict is a ```C```-library written in ```c89```. 

## Science
- Accurate propagation of photons in complex sceneries.
- Support for basic primitives common in science such as spheres, lenses, parabolic dishes, cylinders.
- Support for large triangle meshes from e.g. CAD-files.
- SI-units

## Library
- Wide compatibility with ```c89```.
- No need for build-system.
- Unit tests.
- Style according to ```.clang-format```.
- Minimize warnings when compiled with ```gcc``` and ```clang```.

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
./merlict-c89-view tests/resources/small_scenery.json
```

### Why
Exploring the merlict raytracing-project rewritten in C89 aka. C90, aka. ANSI-C.
This project might never be finished. It is just an exploration to find out what it feels like to rewrite the merlict raytracing-project in ANSI-C aka. C89.
Reasons in no particular order:

- Better compatibility compared to C++11
- Hopefully better wrappable with python using e.g. SWIG
- Learning new (oldschool) concepts beside todays widespread classes

I have the feeling that esspecially classes and inheritance cause most of the current issues in the merlict raytracer which make e.g. python wrapping very difficult. 
