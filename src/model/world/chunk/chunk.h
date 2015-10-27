
#ifndef SRC_MODEL_WORLD_CHUNK_CHUNK_H_
#define SRC_MODEL_WORLD_CHUNK_CHUNK_H_

#include <iostream>
#include <memory>
#include <vector>
#include <glm/glm.hpp>

#include "../../../config/data.h"
#include "../../../util/voxel.h"
#include "../../../graphics/graphicalChunk.h"

namespace chunk
{

class Chunk {
public:

	// ########################################################
	// Constructor/Destructor #################################
	// ########################################################

	Chunk(int x, int y, int z);

	virtual ~Chunk() {};

	// ########################################################
	// Member Functions########################################
	// ########################################################

	// Some kind of update method here

	char getCubeId(int x, int y, int z);

	void setCube(int x, int y, int z, char id);

	void doSunLightning();

	void propagateLight(int x, int y, int z);

	// ########################################################
	// Instance Variables #####################################
	// ########################################################

	int xLocation, yLocation, zLocation;
	Voxel voxels[config::chunk_data::CHUNK_WIDHT][config::chunk_data::CHUNK_HEIGHT][config::chunk_data::CHUNK_DEPTH];
	std::shared_ptr<graphics::GraphicalChunk> graphicalChunk;

};

}

#endif /* SRC_MODEL_WORLD_CHUNK_CHUNK_H_ */
