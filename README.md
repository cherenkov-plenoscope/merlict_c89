![merlict c89 logo](/readme/merlict_c89_logo_wide_32by9_low.jpg)

[![Build Status](https://travis-ci.org/cherenkov-plenoscope/merlict_development_kit.svg?branch=master)](https://travis-ci.org/cherenkov-plenoscope/merlict_c89)
[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

Merlict simulates light. It propagates photons in a scenery, and it can render simple images. Merlict is a ```C```-library written in ```c89```. 

# Abstract
Merlict propagates photons in a scenery. You can define your scenery with a basic set of primitives, or with meshes of triangles from e.g. CAD-files. You define the physical properties of the surfaces in your scenery. You can also query the first intersection along a three-dimensional ray with the surfaces of your scenery. You can describe your scenery in a ```json```-file. Merlict takes your scenery and reorganizes it with space-partitioning to allow efficient queries for raytracing. A minimal viewer allows you to explore your scenery.

# Interface

## Scenery
Your scenery might just be a draft in your thoughts, or it might be a detailed structure in one of your other tools.
We call this the user-scenery. It represents your objects with the hirachy that is relevant to you.
In code, you define this using the ```mliUserScenery```. Or, you write a ```json```-file which merict can read and load into the ```mliUserScenery```. Next, merlict will flatten all your hirachy, and copy your scenery into a dedicated raytracin-scenery named ```mliScenery```. The ```mliScenery``` is written to minimize, and to compress the memory footprint. Finally, merlict will grow an octree ```mliOctree``` for space-partitioning.

### Writing the user-scenery in a json-file

```json-file``` -> ```mliUserScenery``` -> ```mliScenery``` -> ```mliOctree``` 

### Directly setting the raytracing-scenery

```mliScenery``` -> ```mliOctree``` 

## Photons

## Rays



# Tests
#### compile
```
gcc merlict-c89-test.c -o merlict-c89-test -lm
```

#### run
```
./merlict-c89-test
```

#### compile and run tests with muliple compilers
```
./compile_and_test.sh
```
To find compilation-warnings, and -errors early, the ```gcc``` and ```clang``` compilers are called in both ```c```, and ```c++``` configuration. 

# Viewer
A minimal viewer for your command-line.

#### compile
```
gcc merlict-c89-view.c -o merlict-c89-view -lm
```

#### run
```
./merlict-c89-view tests/resources/small_scenery.json
```

## Code and library
- Follow the ```std=c89``` standard.
- Do not allow warnings in ```gcc``` and ```clang``` compilers.
- Write unit-tests.
- Format according to ```.clang-format```.

