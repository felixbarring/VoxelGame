
A project about creating a voxel based game.

Instructions on how to build on Ubuntu:

Cmake and OpenGL dependencies needs to be installed.
sudo apt-get install cmake make g++ libx11-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxrandr-dev libxext-dev

git clone --recursive https://github.com/felixbarring/VoxelGame
cd VoxelGame
mkdir build
cd build
cmake ..
make -j8