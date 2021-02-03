![merlict c89 logo](/readme/merlict_c89_logo_wide_32by9_low.jpg)

[![Build Status](https://travis-ci.org/cherenkov-plenoscope/merlict_development_kit.svg?branch=master)](https://travis-ci.org/cherenkov-plenoscope/merlict_c89)
[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

Merlict simulates light. It propagates photons in a scenery, and it can render simple images. Merlict is a ```C```-library written in ```c89```. 

# Abstract
Merlict propagates photons in a scenery. You define a scenery with meshes of triangles and their surface-normals. You define the physical properties of the surfaces and media such as reflectivity and transmissivity. Independent of light, you can query the intersection of a ray with the surfaces in your scenery. You describe the objects in your scenery using object-wavefront ```.obj```, and ```.json``` files. Merlict uses space-partitioning for efficient queries for raytracing. A minimal viewer allows you to explore your scenery on the command-line and allows you to export high resolution images with depth-of-field.

# Interface
Merlict reads your scenery from a tape-archive ```.tar``` containing human readable text-files which describe both the geometry and the materials.
You can edit the text-files in a directory-tree and bundle them into a ```.tar```. Later you can also serialize and deserialize your entire scenery in binary for faster loading. 

## Objects
A mesh defined by ```verices```, ```vertex-normals```, and triangular ```faces```.

## R-Object
A reference to an object bundeling the Object and the properties of its surfaces and media.

## Scenery
A tree with frames as nodes and R-Objects as leafs defining the relative position and orientation of the R-Objects.

## materials

# media

# surfaces


# Photons


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

# Suggestions
The Blender toolbox to create and manipulate meshes, especially the surface-normals. 
