
#ifndef SRC_GRAPHICS_GRAPHICALCHUNK_H_
#define SRC_GRAPHICS_GRAPHICALCHUNK_H_

#include "mesh/meshVNT.h"
#include "transform.h"

#include "../config/data.h"
#include "../util/voxel.h"

namespace graphics
{

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
			Voxel data[config::chunk_data::GRAPHICAL_CHUNK_WIDTH][config::chunk_data::GRAPHICAL_CHUNK_HEIGHT][config::chunk_data::GRAPHICAL_CHUNK_DEPTH]);
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

	// TODO Use smart pointer here?
	// Why pointer?
	mesh::MeshVNT *mesh;
	float xLocation;
	float yLocation;
	float zLocation;

	int width = config::chunk_data::GRAPHICAL_CHUNK_WIDTH;
	int height = config::chunk_data::GRAPHICAL_CHUNK_HEIGHT;
	int depth = config::chunk_data::GRAPHICAL_CHUNK_DEPTH;

	Transform transform;
};

}

#endif /* SRC_GRAPHICS_GRAPHICALCHUNK_H_ */
