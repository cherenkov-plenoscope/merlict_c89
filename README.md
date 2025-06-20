![merlict c89 logo](/docs/merlict_c89_logo_wide_32by9_low.jpg)

[![Build Status](https://github.com/cherenkov-plenoscope/merlict_c89/actions/workflows/test.yml/badge.svg?branch=main)](https://github.com/cherenkov-plenoscope/merlict_c89/actions/workflows/test.yml)
[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

Merlict simulates light. It finds the intersections of a ray and a complex 3D
geometry with special emphasis on surface normals. Merlict can propagate photons
based on geometric optics according to Fresnel. Merlict can render images and
has an interactive viewer for the terminal. Merlict is a library written in
```c89```.

Merlict reads your scenery from text files which describe your objects, the
geometric relations between them, their materials, and their sourrounding media.
Merlict can read a subset of the popular [object-wavefront
format](https://en.wikipedia.org/wiki/Wavefront_.obj_file) ```.obj```. Merlict
reads the spectral properties of your material's (e.g. reflection or refraction)
from ```.csv``` files. You define photons with a creation position, direction,
and wavelength. For each photon, merlict gives you the list of physical
interactions up to the photon's absorption.

Independent of light and photons, merlict can query the intersections
(including the surface normals) of rays in complex sceneries. You can use this
to empower your own particle or photon propagator.

Merlict would not exist without the author's past and present affiliations:

- Max-Planck-Institute for Nuclear Physics,
  Saupfercheckweg 1, 69117 Heidelberg, Germany :de:

- Institute for Particle Physics and Astrophysics,
  ETH-Zurich, Otto-Stern-Weg 5, 8093 Zurich, Switzerland :switzerland:

- Experimental Physics Vb, Astroparticle Physics,
  TU-Dortmund, Otto-Hahn-Str. 4a, 44227 Dortmund, Germany :de:

# Python :snake:
Checkout [```merlict```](https://github.com/cherenkov-plenoscope/merlict) in ```python```!

# Build :hammer:

Merlict is organized in ['The Pitchfork Layout'](https://github.com/vector-of-bool/pitchfork).
It uses the 'merged Header Placement' because it has no private headers and it
uses the 'merged Tests Placement'. This hopefully gives you a reasonable chance
to wrap your build system around it. Merlict's sources are split into modules
of which many have very limited scopes.

```
    merlict_c89
    |
    |-> src
        |
        |-> ray
        |   |
        |   |-> ray.h
        |   |-> ray.c
        |   |-> ray.test.c
        |
        |-> photon
        ...
```

## Makefile

A ```Makefile``` will build the interactive viewer in ```build/bin/viewer```.
Take a look here for what a makefile based build system might look like.

```bash
make
```

## Almagamate into a single header and a single source

Merlict has ```tool/almagamate.py``` which can combine the headers and sources
into a single header and source file. This is a poor man's build system which is
primarily used to almagamate the sources for a ```python``` (```cython```)
build. It makes the ```cython``` side of things very easy. You can use the
almagamated sources for your build system as well. This way you only have to:

```c
#include "mli.h"
```

in your headers, and

```c
#include "mli.c"
```

in your sources. Thats it. :checkered_flag:

With the ```--test``` flag the ```tool/almagamate.py``` further creates the
```main()``` for unit testing. See ```./compile_and_test.py``` for a minimal
example. The test's ```main()``` does not use the almagamated sources but
includes the source files from the ```src/```directory directly. This is done to
ease debugging so that filenames and linenumber match the original sources.


# Viewer :eyes:
Merlict's viewer runs in the terminal and reads three formats:

- a standalone object-wavefront ```.obj```
- merlict's own ```scenery.tar```
- merlict's own ```scenery.bin```

Control your viewing direction and position via the keyboard. You can inspect
the scenery and render high resolution images. The viewer prints into the
terminal using ASCII-art. When your terminal supports
[```ANSI-escape-code```](https://en.wikipedia.org/wiki/ANSI_escape_code) you can
switch to 24-bit true color. The viewer is especially useful when you run
merlict on a remote computer via ```ssh``` without an ```X```-server. Merlict's
viewer will try to set your terminal's ```stdin``` to a non canonical mode so
that you do not have to press [Enter] after each keypress.

### Build :hammer:

See ```./examples/mli_viewer/viewer.main.c```.

### Run
```bash
./build/viewer data/sceneries/001.tar
```

 ASCII-art                 | ANSI-escape-codes
:-------------------------:|:-------------------------:
![viewer-ascii-art](/docs/viewer/teapot_ascii_v1-0-0.png)  |  ![viewer-ansi-escape](/docs/viewer/teapot_ansi_v1-0-0.png)

![viewer-rendering-ppm](/docs/viewer/teapot_full_v1-0-0.png)

### Colors :rainbow:
Merlict's viewer and visual path tracer do not use the conventional red-green-blue color space.
Instead merlict uses the phiysical spectra directly.
The light source has the spectrum of the sun, and the camera (or `eye') perceives
colors according to the [CIE-1931](https://en.wikipedia.org/wiki/CIE_1931_color_space)
tri-stimulus color theory.

# Scenery

Merlict reads the scenery from text files inside a tape archive (```.tar```).
To get started it is best to create a directory, fill it with text files,
wrap it into a tape archive, and finally feed it to ```merlict```.
The scenery directory and the textfiles must follow a fixed structure.
See e.g. ```./data/sceneries/optics_focussing_mirror``` for an example
scenery used in the unit tests. To set up the scenery entirely in
```python```, see [```merlict```](https://github.com/cherenkov-plenoscope/merlict).

## structure
Merlict's structure separates the scenery into its ```geometry/``` and the
physical properties of its ```materials/```.
In ```geometry/relations.json``` the geometry is linked to the materials.

```
    <my_scenery>/
    |-> geometry/
    |   |-> objects/
    |   |   |-> sphere.obj
    |   |   |-> mirror.obj
    |   |   |-> lens.obj
    |   |   |-> ...
    |   |
    |   |-> relations.json
    |
    |-> materials/
    |   |-> spectra/
    |   |   |-> vacuum_refraction.csv
    |   |   |-> vacuum_absorption.csv
    |   |   |-> water_refraction.csv
    |   |   |-> aluminium_mirror_reflection.csv
    |   |   |-> schott_n_kzfs2_refraction.csv
    |   |   |-> ...
    |   |
    |   |-> media/
    |   |   |-> schott_n_kzfs2.json
    |   |   |-> vacuum.json
    |   |   |-> ...
    |   |
    |   |-> surfaces/
    |   |   |-> glass.json
    |   |   |-> screen.json
    |   |   |-> ...
    |   |  
    |   |-> boundary_layers/
    |
    |-> README.md
```

## ```./geometry/```

All objects in the scenery are represented using meshes of triangles.
It is up to you if you want to represent the scenery using many small
and simple objects or rather like to represent the scenery using few and
complex objects. Internally, the ray tracer in ```merlict``` does not care.
A tree of relations in ```geometry/relations.json``` defines the relative
positions and orientations of the objects in the scenery and links their
surfaces to physical boundary layers which represent the materials.

### ```./geometry/objects/```
An object in merlict is a mesh of triangle ```faces```. Each ```face```
references its three ```vertices``` and ```vertex-normals```.
To simulate smooth optical surfaces, merlict uses meshes with
```vertex-normals```. Also each ```face``` can reference a different material.
Thus a single object can contain multiple physical surfaces and media.
Merlict reads objects from object-wavefront files (```.obj```)

### Object-Wavefront Format ```.obj```
Merlict supports a subset of the [```obj```
format](https://en.wikipedia.org/wiki/Wavefront_.obj_file) in ASCII-text. An
object-wavefront defines a mesh of triangle-faces in a 3D space with special
emphasis on the meshe's surface-normal. Each face ```f``` references its three
vertices ```v```. The surface-normal of a face ```f``` will be interpolated
between the face's three vertex-normals ```vn``` using the barycentrig weight of
the intersection-position w.r.t the face.

- ```#``` comment-line. Any text in this line.
- ```v``` vertex-line. Three real numbers define one vertex in the mesh.
- ```vn``` vertex-normal-line. Three reel numbers define one surface-normal in
  the mesh.
- ```f``` face-line. Exactly __three__ integer references to vertices define a
  face. And __three__ integer references to vertex-normals define the
  surface-normals on this face. References must be positive, backwards
  referencing with negative integers is not supported.
- ```usemtl``` material-reference. All following faces are assigned the same
  material. There must be __at least one__ ```usemtl``` before the first face.

Other features of ```obj``` such as e.g. vertex-texture-coordinates ```vt```
will be ignored. Blank lines are accepted but ignored.

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

### ```./geometry/relations.json```
A tree with cartesian frames as nodes and object-references as leafs. The tree
defines the relative position and orientation of your objects.


## ```./materials/```
The physical properties of surfaces and media are stored in the ```materials/```
directory.
In ```Merlict```, every triangle face represents a physical boundary layer.
A boundary layer represents the boundary between two media such as air and water.
Further, a boundary layer might also reference surface effects such as specular
reflection.
In merlict, ```boundary_layers``` reference ```media``` and ```surfaces```.
And in turn a medium references a ```spectra``` such as a refraction spectra,
and a surface in turn references a ```spectra``` such as a reflection spectra.

### ```./materials/spectra/```
A ```spectra``` is a one dimensional function of a physical property versus
the wavelength. Merlict stores spectra in comma seperated value files (```.csv```)
according to ```[# RFC 4180]```.

Example spectrum with the refractive index of a specific glass:

```csv
wavelength/m, refraction/1
 334.1e-9, 1.59259
 365.0e-9, 1.58382
 404.7e-9, 1.57580
 435.8e-9, 1.57114
 480.0e-9, 1.56612
 486.1e-9, 1.56553
 587.6e-9, 1.55836
 589.3e-9, 1.55827
 632.8e-9, 1.55617
 643.8e-9, 1.55570
 656.3e-9, 1.55519
 706.5e-9, 1.55337
 852.1e-9, 1.54944
1014.0e-9, 1.54625
1060.0e-9, 1.54546
1529.6e-9, 1.53798
1970.1e-9, 1.53011
2325.4e-9, 1.52239
```

The header line is mandatory but merlict will not interpret its content.
The content of the header only serves the users.
Whitespaces can be used for padding but will be stripped by merlict.

### ```./materials/surfaces/```
Merlict describes a surface using the `Cook-Torrance' model and it describes
media using its refraction and absorbtion. Both surfaces and media reference
spectra 

### ```./materials/media/```

### ```./materials/boundary_layers/```


# Photons
Photons are defined by their creation-position, their direction, their
wavelength. During propagation, merlict writes the history of the photon
bouncing around in the scenery until it is absorbed.

# Unit-Tests
Merlict has a ```python``` script ```./tools/compile_and_test.py```
to prepare, build, and run the unit-tests. Run this script to check whether
merlict builds and runs fine on your platform.

The script
- prepares the resources as it ```tar```s the sceneries into tape-archives,
- builds with both ```gcc``` and ```clang```, in both ```c```, and ```c++```
  mode,
- and runs the tests.

# Development
Open an issue and describe your problem or wish.

Then its the core developer's job to:
- Write unit-tests.
- Credit and acknowledge original authors.
- Obey ```std=c89``` standard.
- Keep the header's namespace clean. No ```typedef```s! Merlict uses ```mli```
  prefix.
- Avoid all Warnings in```gcc``` and ```clang``` in both ```c``` and ```c++```
  mode.
- Format ```c``` according to ```./tools/.clang-format```. Run the linter using
  ```python ./tools/lint.py```.

But of course the author will happily recive pull-requests that meet such
expectations :wink:.

# Suggested Tools
- [Blender](https://www.blender.org/) to inspect and manipulate objects. It is
  especially useful to visualize surface-normals.

- [OpenScad](http://www.openscad.org/) to create meshes in a parametric way.
  As openscad does not have a concept of vertex-normals, it is mostly only
  useful for flat surfaces or non optical surfaces.

- [refractive-index-database](https://refractiveindex.info/) a database of
  refractive indices of various materials.

- [optic-object-wavefronts](https://github.com/cherenkov-plenoscope/optic_object_wavefronts)
  makes accurate triangle meshes with vertex normals of the most common optical surfaces such
  as spherical lenses or more complex objects such as Schmidt corrector plates for Schmidt
  telescopes.

- [triangle_mesh_io](https://github.com/cherenkov-plenoscope/triangle_mesh_io) can read,
  write and partly convert common triangle mesh formats such as ```.stl```, ```.off```,
  and ```.obj```. It can further add ```vertex-normals```.

# Acknowledgement

Finding intersections of triangles and cubes
- Voorhies, Douglas; Triangle-Cube Intersection; Graphics Gems III, p. 236-239,
  code: p. 521-526

Finding ray and triangle intersection
- Thomas, Moeller and Ben, Trumbore; ['Fast, Minimum Storage Ray-Triangle
  Intersection'](doi:10.1080/10867651.1997.10487468), Journal of Graphics Tools
  2: 21-28 and Wikipedia

Mirroring direction-vectors on a surface-normal
- J.H., Bruge; University of Arizona; OPTI 421/521 – 'Introductory
  Optomechanical Engineering'

Traversing an octree with a ray
- Revelles, Jorge and Urena, Carlos and Lastra, Miguel; 'An efficient parametric
  algorithm for octree traversal'; Vaclav Skala-UNION Agency
- Jeroen, Baert; additional comments

Pseudo random number generator
- Makoto, Matsumoto and Takuji, Nishimura and
  [wikipedia](https://en.wikipedia.org/wiki/Mersenne_Twister); Mersenne Twister
- Melissa E. O'Neill; Harvey Mudd College;
  [pcg-random.org](http://www.pcg-random.org); Permuted Congruential Generator
  (PCG)

Parsing JSON-strings
- Serge, Zaitsev; [JSMN](https://zserge.com/jsmn/)

Awesome debug MACROS
- Zed, Shawn; 'Learn C the hard way'

Sampling from random distributions
- Volker, Blobel; 'Statistical and numerical methods'

Homogenous Transformations
- Bruno, Siciliano and Lorenzo, Sciavicco and Luigi, Villani and Guiseppe,
  Oriolo; 'Robotics -- Modelling, Planning and Control'

Writing and reading tape-archives (```.tar```)
- rxi; [microtar](https://github.com/rxi/microtar)

Controlling stdin buffer using ```termios```
- Edwin, Buck;

A fast voxel traversal algorithm for ray tracing
- John Amanatides and Andrew Woo, Dept. of Computer Science University of
  Toronto, Toronto, Ontario, Canada M5S 1A4
