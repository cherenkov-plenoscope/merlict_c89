![merlict c89 logo](/readme/merlict_c89_logo_wide_32by9_low.jpg)

[![Build Status](https://travis-ci.org/cherenkov-plenoscope/merlict_development_kit.svg?branch=master)](https://travis-ci.org/cherenkov-plenoscope/merlict_c89)
[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

Merlict simulates light. It can find intersections of a ray in a complex 3D geometry with special emphasis on surface-normals. Merlict can propagate photons based on geometric optics according to Fresnel. Merlict comes with its own interactive viewer for the terminal. Merlict is a library written in ```c89```.

Merlict reads your scenery from text-files which describe your objects, the geometric relations among them, their materials, and their sourrounding media. Merlict can read a subset of the popular [object-wavefront format](https://en.wikipedia.org/wiki/Wavefront_.obj_file) ```.obj```.
You define photons with a creation-position, direction, and wavelength.
For each photon, merlict gives you the list of physical interactions up to its absorbtion.

Merlict would not exist without the author's past and present affiliations:

- Max-Planck-Institute for Nuclear Physics,
  Saupfercheckweg 1, 69117 Heidelberg, Germany
 
- Institute for Particle Physics and Astrophysics,
  ETH-Zurich, Otto-Stern-Weg 5, 8093 Zurich, Switzerland

- Experimental Physics Vb, Astroparticle Physics,
  TU-Dortmund, Otto-Hahn-Str. 4a, 44227 Dortmund, Germany

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

## object-wavefront format ```.obj```
Merlict supports a subset of the [```obj``` format](https://en.wikipedia.org/wiki/Wavefront_.obj_file) in ASCII-text. An object-wavefront defines a mesh of triangle-faces in a 3D space with special emphasis on the meshe's surface-normal. Each face ```f``` references its three vertices ```v```. The surface-normal of a face ```f``` will be interpolated between the face's three vertex-normals ```vn``` using the barycentrig weight of the intersection-position w.r.t the face.

- ```#``` comment-line. Any text in this line.
- ```v``` vertex-line. Three real numbers define one vertex in the mesh. 
- ```vn``` vertex-normal-line. Three reel numbers define one surface-normal in the mesh.
- ```f``` face-line. Exactly __three__ integer references to vertices define a face. And __three__ integer references to vertex-normals define the surface-normals on this face. References must be positive, backwards referencing with negative integers is not supported.
- ```usemtl``` material-reference. All following faces are assigned the same material. There must be __at least one__ ```usemtl``` before the first face.

Other features of ```obj``` such as e.g. vertex-texture-coordinates ```vt``` will be ignored. Blank lines are accepted but ignored.

A simple cube where each of the six sides references a different material:
```obj
# vertices
v 1.0 0.0 0.0
v 1.0 1.0 0.0
v 0.0 1.0 0.0
v 0.0 0.0 0.0
v 1.0 0.0 1.0
v 1.0 1.0 1.0
v 0.0 1.0 1.0
v 0.0 0.0 1.0
# vertex normals
vn 1.0 0.0 0.0
vn 0.0 1.0 0.0
vn 0.0 0.0 1.0
vn -1.0 0.0 0.0
vn 0.0 -1.0 0.0
vn 0.0 0.0 -1.0
# faces
usemtl px
f 1//1 2//1 6//1
f 6//1 5//1 1//1
usemtl py
f 2//2 3//2 6//2
f 6//2 3//2 7//2
usemtl pz
f 5//3 6//3 7//3
f 7//3 8//3 5//3
usemtl mx
f 3//4 7//4 4//4
f 7//4 8//4 4//4
usemtl my
f 1//5 4//5 8//5
f 8//5 5//5 1//5
usemtl mz
f 1//6 2//6 3//6
f 3//6 4//6 1//6
```

# Viewer
Merlict's viewer runs in the terminal and reads three formats:

- merlict's own ```scenery.tar```
- merlict's own ```scenery.bin```
- a standalone object-wavefront ```.obj```

Just like in a 3D video-game you control your viewing-direction and position via the keyboard. You can inspect the scenery and render high resolution images.
The viewer prints its rendering directly into the terminal in ASCII-art. When your terminal supports [```ANSI-escape-code```](https://en.wikipedia.org/wiki/ANSI_escape_code) you can switch to 24-bit true color. The terminal based viewer is especially useful when you run merlict on a remote computer via ```ssh```.
Merlict's viewer will try to set your terminal's ```stdin``` to a non canonical mode so that you do not have to press [Enter] after each keypress.

#### Compile
```bash
gcc merlict-c89-view.c -o view -lm
```

#### Run
```bash
./view tests/resources/sceneries/001.tar
```

# Build
Merlict uses the only buildsystem which does not try to murder you: No buildsystem.

Just 
```c
#include "merlict_c89/merlict_c89.h"
#include "merlict_c89/merlict_c89.c"
```
in your ```main.c``` and

```bash
gcc main.c -o exe -lm
```

Merlict is structured into pairs of ```merlict_c89/mli*.h``` and ```merlict_c89/mli*.c``` files which you can put into your buildsystem if you want to. However, in my projects building all sources over again from scratch for each executable was always fastest. So I just include the collective ```merlict_c89/merlict_c89.h``` in my project's headers, and the collective ```merlict_c89/merlict_c89.c``` in my project's sources.

## Unit-tests
```bash
./compile_and_test.sh
```
A script to compile with both ```gcc``` and ```clang```, in both ```c```, and ```c++``` mode. Also run the unit-tests.


## Code
- Write unit-tests.
- Obey ```std=c89``` standard.
- Avoid all Warnings in```gcc``` and ```clang``` in both ```c``` and ```c++``` mode.
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
