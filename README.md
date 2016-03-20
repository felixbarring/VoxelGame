
#Voxel Game

A project about creating a voxel based game.
The game currently uses the working title "Librecraft".

## Building

Currently only tested on my Ubuntu machine.
Instructions for Windows and Fedora will likely come sometime in the future.

Cmake and SFML needs to be installed.

Instructions on how to build on Ubuntu:

```
sudo apt-get install cmake make g++ libsfml-dev
git clone --recursive https://github.com/felixbarring/VoxelGame
cd VoxelGame/scripts
bash setupBuild.sh	
bash build.sh
```