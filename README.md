
#Voxel Game

A project about creating a voxel based game.
The game currently uses the working title "Librecraft".

## Building

Currently only tested on my Ubuntu machine.
Instructions for Windows and Fedora will likely come sometime in the future.

The 'resources ' folder is not currently supplied for copyright reasons.
Cmake and OpenGL dependencies needs to be installed.

Instructions on how to build on Ubuntu:

```
sudo apt-get install cmake make g++ libx11-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxrandr-dev libxext-dev
git clone --recursive https://github.com/felixbarring/VoxelGame
cd VoxelGame/scripts
bash setupBuild.sh	
bash build.sh
```