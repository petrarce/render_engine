Current `master` branch status (Build and test):

[![petrarce](https://circleci.com/gh/petrarce/render_engine/tree/master.svg?style=svg)](https://app.circleci.com/pipelines/github/petrarce/render_engine?branch=master)

## TODO'S

### Features

#### General

* [ ] Loading 3d meshes from blender/3dmax/... model files
* [ ] Shadows
* [ ] Screen space AO
* [ ] Screnn space reflections
* [ ] Generation of open space environments
* [ ] Level of details maps

#### Shaders

* [ ] Phong/Blinn-Phong/GGX models
* [ ] Ambient occlusions maps
* [ ] Normal maps
* [ ] Metalikness maps
* [ ] Specularity maps
* [ ] Bump maps
* [ ] Roughness maps

#### Meshes

* [ ] Cube texture for surrounding
* [ ] Heightmap meshes

#### Procedural textures

* [ ] Cellular texturing (Worley noise)
* [ ] Solid texturing (Perlin noise)
* [ ] Self-organized textures?

####  Algorythms

* [ ] GPU-based sort algorythms.

### Improvements

* [ ] Cache scope state. Don't prepare it every draw call 
* [ ] Cache uniform. Don't aply them agin unless unchanged
* [ ] Call OpenGL bind functions only if different object is in current scope
* [ X ] Create wrapper for OpenGL state changes 