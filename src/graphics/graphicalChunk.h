
#ifndef SRC_GRAPHICS_GRAPHICALCHUNK_H_
#define SRC_GRAPHICS_GRAPHICALCHUNK_H_

#include "mesh/meshVNT.h"
#include "transform.h"

class GraphicalChunk {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	GraphicalChunk(float xOffset, float yOffset, float zOffset, char data[16][16][16]);
	virtual ~GraphicalChunk();

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

#endif /* SRC_GRAPHICS_GRAPHICALCHUNK_H_ */
