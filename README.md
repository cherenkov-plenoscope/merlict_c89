![merlict c89 logo](/readme/merlict_c89_logo_wide_32by9_low.jpg)

[![Build Status](https://travis-ci.org/cherenkov-plenoscope/merlict_development_kit.svg?branch=master)](https://travis-ci.org/cherenkov-plenoscope/merlict_c89)
[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

Merlict simulates light. It propagates photons in a scenery, and it can render simple images. Merlict is a ```C```-library written in ```c89```. 

# Abstract
Merlict propagates photons in a scenery. You can define your scenery with a basic set of primitives, or with meshes of triangles from e.g. CAD-files. You define the physical properties of the surfaces in your scenery. You can also query the first intersection along a three-dimensional ray with the surfaces of your scenery. You can describe your scenery in a ```json```-file. Merlict takes your scenery and reorganizes it with space-partitioning to allow efficient queries for raytracing. A minimal viewer allows you to explore your scenery.

# Interface

## Scenery

### Writing a json-file

```json-file``` -> ```mliUserScenery``` -> ```mliScenery``` -> ```mliOctree``` 

### Setting the 

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

