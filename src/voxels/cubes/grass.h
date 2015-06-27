
#ifndef SRC_VOXELS_CUBES_GRASS_H_
#define SRC_VOXELS_CUBES_GRASS_H_

class Grass : public virtual Cube{
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	Grass();
	virtual ~Grass();

// ########################################################
// Member Functions########################################
// ########################################################

	virtual int getFrontTextureID() override {return 240;};

	virtual int getBackTextureID() override {return 240;};

	virtual int getLeftTextureID() override {return 240;};

	virtual int getRightTextureID() override {return 240;};

	virtual int getTopTextureID() override {return 240;};

	virtual int getBottomTextureID() override {return 240;};

// ########################################################
// Instance Variables #####################################
// ########################################################
};

#endif /* SRC_VOXELS_CUBES_GRASS_H_ */
