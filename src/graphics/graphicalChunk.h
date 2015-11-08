
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
			std::vector<std::vector<std::vector<Voxel>>> *back,
			std::vector<std::vector<std::vector<Voxel>>> *front
	);

	virtual ~GraphicalChunk() {};

// ########################################################
// Member Functions########################################
// ########################################################

	void draw();

	Transform& getTransform();

	float getxLocation();

	float getyLocation();

	Voxel* getVoxel(int x, int y, int z,
			std::vector<std::vector<std::vector<Voxel>>> &data,
			std::vector<std::vector<std::vector<Voxel>>> *right,
			std::vector<std::vector<std::vector<Voxel>>> *left,
			std::vector<std::vector<std::vector<Voxel>>> *back,
			std::vector<std::vector<std::vector<Voxel>>> *front);

// ########################################################
// Instance Variables #####################################
// ########################################################

private:

	struct CubeFaceData {
		int id;
		bool vissible, front, back, left, right, top, bottom;
		char lightValue; // For air blocks

		// lv means lightValue
		char lvFront_BottomLeft, lvFront_BottomRight, lvFront_TopRight, lvFront_TopLeft;
		char lvBack_BottomLeft, lvBack_BottomRight, lvBack_TopRight, lvBack_TopLeft;
		char lvLeft_BottomLeft, lvLeft_BottomRight, lvLeft_TopRight, lvLeft_TopLeft;
		char lvRight_BottomLeft, lvRight_BottomRight, lvRight_TopRight, lvRight_TopLeft;
		char lvTop_BottomLeft, lvTop_BottomRight, lvTop_TopRight, lvTop_TopLeft;
		char lvBottom_BottomLeft, lvBottom_BottomRight, lvBottom_TopRight, lvBottom_TopLeft;

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
