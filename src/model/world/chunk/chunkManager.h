
#ifndef SRC_MODEL_WORLD_CHUNK_CHUNKMANAGER_H_
#define SRC_MODEL_WORLD_CHUNK_CHUNKMANAGER_H_

#include "chunk.h"
#include "../../../config/data.h"


class ChunkManager {
public:

	// ########################################################
	// Constructor/Destructor #################################
	// ########################################################

	ChunkManager();
	virtual ~ChunkManager();

	// ########################################################
	// Member Functions########################################
	// ########################################################

	// Should probably not be here
	void render();

	// ########################################################
	// Instance Variables #####################################
	// ########################################################

	//Chunk chunks[chunk_data::NUMBER_OF_CHUNKS_X][chunk_data::NUMBER_OF_CHUNKS_Y];

};

#endif /* SRC_MODEL_WORLD_CHUNK_CHUNKMANAGER_H_ */
