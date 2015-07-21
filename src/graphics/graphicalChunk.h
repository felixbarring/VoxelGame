
#ifndef SRC_GRAPHICS_GRAPHICALCHUNK_H_
#define SRC_GRAPHICS_GRAPHICALCHUNK_H_

#include "mesh/meshVNT.h"
#include "transform.h"

#include "../config/data.h"
#include "../voxel.h"


struct CubeFaceData {
	int id;
	bool vissible, front, back, left, right, top, bottom;
};

class GraphicalChunk {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	GraphicalChunk(float x, float y, float z,
			Voxel data[chunk_data::GRAPHICAL_CHUNK_WIDTH][chunk_data::GRAPHICAL_CHUNK_HEIGHT][chunk_data::GRAPHICAL_CHUNK_DEPTH]);
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

	int width = chunk_data::GRAPHICAL_CHUNK_WIDTH;
	int height = chunk_data::GRAPHICAL_CHUNK_HEIGHT;
	int depth = chunk_data::GRAPHICAL_CHUNK_DEPTH;

	Transform transform;
};

#endif /* SRC_GRAPHICS_GRAPHICALCHUNK_H_ */
