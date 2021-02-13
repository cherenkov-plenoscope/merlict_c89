![merlict c89 logo](/readme/merlict_c89_logo_wide_32by9_low.jpg)

[![Build Status](https://travis-ci.org/cherenkov-plenoscope/merlict_development_kit.svg?branch=master)](https://travis-ci.org/cherenkov-plenoscope/merlict_c89)
[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

Merlict simulates light. It can find intersections of rays and complex geometries in 3D with special emphasis on surface-normals. Merlict can propagate photons based on geometric optics according to Fresnel. Merlict is a library written in ```c89```.

Merlict reads your scenery from text-files which describe your objects, the geometric relations among them, their materials, and their sourrounding media. Merlict can read a subset of the popular [wavefront-object-fileformat](https://en.wikipedia.org/wiki/Wavefront_.obj_file) ```.obj```.
You define photons with a creation-position, direction, and wavelength.
For each photon, merlict gives you the list of physical interactions up to its absorbtion.

# Interface

## Scenery

### tree
A tree with cartesian frames as nodes and object-references as leafs. The tree defines the relative position and orientation of your object-references.

### object
A mesh of triangular ```faces``` which are defined by their ```vertices```, and ```vertex-normals```.
To approximate complex surfaces, especially complex surface-normals, you can control the ```vertex-normals``` of each ```face```. The surface-normal in an intersection will be the barycentric interpolation of the ```vertex-normals```.

### object-reference
A reference to an object. It bundles the object and the pysical properties of its surfaces and media.

### materials
You describe a medium by its transmissivity and its refractive index.

You define surfaces by their specular, and diffuse (lambertian) reflections approximating physical surfaces using the Phong-model.

## Photons
Photons are defined by their creation-position, their direction, their wavelength.
During propagation, merlict writes the history of the photon bouncing around in the scenery until it is absorbed.

## object-wavefront fileformat ```.obj```
Merlict supports a subset of the ```obj``` fileformat. ASCII-text with '\n' newlines. There may be blank lines to help formating.
- ```#``` comment
- ```v``` vertex
- ```vn``` vertex-normal
- ```f``` face
- ```usemtl``` material-reference
Other features of ```obj``` will be ignored.


# Build and tests
```
./compile_and_test.sh
```
A script to compile with both ```gcc``` and ```clang```, in both ```c```, and ```c++``` mode. Also run the unit-tests.

# Viewer
A minimal viewer in the terminal.

#### Compile
```
gcc merlict-c89-view.c -o view -lm
```

#### Run
```
./view tests/resources/sceneries/001.tar
```

## Code
- Write unit-tests.
- Obey ```std=c89``` standard.
- No Warnings in```gcc``` and ```clang``` compilers both in ```c``` and ```c++``` mode.
- Format according to ```.clang-format```.

# Suggested tools
- [Blender](https://www.blender.org/) to inspect and manipulate objects. It is especially useful to visualize surface-normals.

- [OpenScad](http://www.openscad.org/) to create meshes in a parametric way. Unfortunately it does not have a concept of vertex-normals.

# Acknowledgement

Finding intersections of triangles and cubes
- Voorhies, Douglas; Triangle-Cube Intersection; Graphics Gems III, p. 236-239, code: p. 521-526

Finding ray and triangle intersection
- Thomas, Moeller and Ben, Trumbore; ['Fast, Minimum Storage Ray-Triangle Intersection'](doi:10.1080/10867651.1997.10487468), Journal of Graphics Tools 2: 21-28 and Wikipedia

Mirroring direction-vectors on a surface-normal
- J.H., Bruge; University of Arizona; OPTI 421/521 – 'Introductory Optomechanical Engineering'

Traversing an octree with a ray
- Revelles, Jorge and Urena, Carlos and Lastra, Miguel; 'An efficient parametric algorithm for octree traversal'; Vaclav Skala-UNION Agency
- Jeroen, Baert; additional comments

Pseudo random number generator
- Makoto, Matsumoto and Takuji, Nishimura and [wikipedia](https://en.wikipedia.org/wiki/Mersenne_Twister); Mersenne Twister

Parsing JSON-strings
- Serge, Zaitsev; [JSMN](https://zserge.com/jsmn/)

Awesome debug MACROS
- Zed, Shawn; 'Learn C the hard way'

Sampling from random distributions
- Volker, Blobel; 'Statistical and numerical methods'

Homogenous Transformations
- Bruno, Siciliano and Lorenzo, Sciavicco and Luigi, Villani and Guiseppe, Oriolo; 'Robotics -- Modelling, Planning and Control'

Writing and reading tape-archives (```.tar```)
- rxi; [microtar](https://github.com/rxi/microtar)

Controlling stdin buffer using ```termios```
- Edwin, Buck;
