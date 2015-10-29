
#ifndef SRC_GRAPHICS_GRAPHICALCHUNK_H_
#define SRC_GRAPHICS_GRAPHICALCHUNK_H_

#include <memory>
#include <vector>

#include "mesh/meshElement.h"
#include "transform.h"

#include "../config/data.h"
#include "../util/voxel.h"

namespace graphics
{

class GraphicalChunk {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	GraphicalChunk(float x, float y, float z,
			std::vector<std::vector<std::vector<Voxel>>> &data,
			std::vector<std::vector<std::vector<Voxel>>> *right,
			std::vector<std::vector<std::vector<Voxel>>> *left,
			std::vector<std::vector<std::vector<Voxel>>> *front,
			std::vector<std::vector<std::vector<Voxel>>> *back
	);

	virtual ~GraphicalChunk() {};

// ########################################################
// Member Functions########################################
// ########################################################

	void draw();

	Transform& getTransform();

	float getxLocation();

	float getyLocation();

// ########################################################
// Instance Variables #####################################
// ########################################################

private:

	struct CubeFaceData {
		int id;
		bool vissible, front, back, left, right, top, bottom;
		char lightValue; // For air blocks
		// lv means lightValue
		char lvFront, lvBack, lvLeft, lvRight, lvTop, lvBottom;
	};

	std::unique_ptr<mesh::MeshElement> mesh;
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
