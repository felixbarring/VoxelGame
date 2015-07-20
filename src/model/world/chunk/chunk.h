
#ifndef SRC_MODEL_WORLD_CHUNK_CHUNK_H_
#define SRC_MODEL_WORLD_CHUNK_CHUNK_H_

#include "../../../config/chunkData.h"

struct Voxel {
	char id;
};


class Chunk {
public:

	// ########################################################
	// Constructor/Destructor #################################
	// ########################################################

	Chunk();
	virtual ~Chunk();

	// ########################################################
	// Member Functions########################################
	// ########################################################

	// ########################################################
	// Instance Variables #####################################
	// ########################################################

	Voxel voxels[chunk_data::CHUNK_WIDHT][chunk_data::CHUNK_HEIGHT][chunk_data::CHUNK_DEPTH];

};

#endif /* SRC_MODEL_WORLD_CHUNK_CHUNK_H_ */
