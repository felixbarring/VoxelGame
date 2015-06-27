
#ifndef GAME_MODEL_ENTITIES_CUBE_H_
#define GAME_MODEL_ENTITIES_CUBE_H_

#include "mesh/meshVNT.h"
#include "transform.h"

class TexturedCube {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	TexturedCube(float xOffset, float yOffset, float zOffset, int id);
	virtual ~TexturedCube();

// ########################################################
// Member Functions########################################
// ########################################################

	void draw();

	Transform& getTransform() { return transform; }

	float getxLocation() { return xLocation; }

	float getyLocation() { return yLocation; }

// ########################################################
// Instance Variables #####################################
// ########################################################

private:
	MeshVNT *mesh;
	float xLocation;
	float yLocation;
	float zLocation;

	Transform transform;
};

#endif /* GAME_MODEL_ENTITIES_CUBE_H_ */
