
#ifndef SRC_MODEL_WORLD_CHUNK_CHUNK_H_
#define SRC_MODEL_WORLD_CHUNK_CHUNK_H_

#include <iostream>
#include <memory>

#include "../../../config/data.h"
#include "../../../voxel.h"
#include "../../../graphics/graphicalChunk.h"

class Chunk {
public:

	// ########################################################
	// Constructor/Destructor #################################
	// ########################################################

	Chunk(int x, int y, int z);
	virtual ~Chunk();

	// ########################################################
	// Member Functions########################################
	// ########################################################

	// Some kind of update method here

	// ########################################################
	// Instance Variables #####################################
	// ########################################################

	int xLocation, yLocation, zLocation;
	Voxel voxels[chunk_data::CHUNK_WIDHT][chunk_data::CHUNK_HEIGHT][chunk_data::CHUNK_DEPTH];

	std::shared_ptr<GraphicalChunk> graphicalChunk;

};

#endif /* SRC_MODEL_WORLD_CHUNK_CHUNK_H_ */
