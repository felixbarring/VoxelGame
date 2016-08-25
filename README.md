
#Voxel Game

A project about creating a voxel based game.
The game currently uses the working title "Librecraft".

## Building

Cmake and SFML needs to be installed.
Currently only tested on my Ubuntu machine and a Fedora.
Instructions for Windows will likely come sometime in the future.

Instructions on how to get going.

```
Installing dependencies on Ubuntu:
	sudo apt-get install cmake make g++ libsfml-dev

	Should be something similar on Fedora

Buiding:
	git clone --recursive https://github.com/felixbarring/VoxelGame
	cd VoxelGame/scripts
	bash setupBuild.sh	
	bash build.sh
```