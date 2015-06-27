
#ifndef SRC_VOXELS_CUBES_CUBE_H_
#define SRC_VOXELS_CUBES_CUBE_H_

class Cube {
public:
	virtual ~Cube();

	virtual int getFrontTextureID() = 0;

	virtual int getBackTextureID() = 0;

	virtual int getLeftTextureID() = 0;

	virtual int getRightTextureID() = 0;

	virtual int getTopTextureID() = 0;

	virtual int getBottomTextureID() = 0;

};

#endif /* SRC_VOXELS_CUBES_CUBE_H_ */
