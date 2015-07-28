
#ifndef SRC_MODEL_WORLD_CHUNK_CHUNKMANAGER_H_
#define SRC_MODEL_WORLD_CHUNK_CHUNKMANAGER_H_

#include <memory>
#include <vector>

#include "chunk.h"
#include "../../../config/data.h"

class ChunkManager {
private:

	// ########################################################
	// Constructor/Destructor #################################
	// ########################################################

	ChunkManager();
	virtual ~ChunkManager();

	// ########################################################
	// Member Functions########################################
	// ########################################################

public:

	static ChunkManager& getInstance()
	{
		static ChunkManager INSTANCE;
		return INSTANCE;
	}

	char getCubeId(int x, int y, int z);

	void removeCube(int x, int y, int z);

	void setCube(int x, int y, int z, char id);

	// ########################################################
	// Instance Variables #####################################
	// ########################################################

private:

	std::unique_ptr<Chunk> chunks[config::chunk_data::NUMBER_OF_CHUNKS_X][config::chunk_data::NUMBER_OF_CHUNKS_Y][config::chunk_data::NUMBER_OF_CHUNKS_Z];

};

#endif /* SRC_MODEL_WORLD_CHUNK_CHUNKMANAGER_H_ */
