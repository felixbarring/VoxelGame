
#ifndef SRC_MODEL_WORLD_CHUNK_CHUNKMANAGER_H_
#define SRC_MODEL_WORLD_CHUNK_CHUNKMANAGER_H_

#include <memory>
#include <vector>

#include "chunk.h"
#include "../../../config/data.h"
#include "../../../util/voxel.h"

namespace chunk
{

class ChunkManager {
private:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	ChunkManager();

	virtual ~ChunkManager() {};

// ########################################################
// Member Functions########################################
// ########################################################

public:

	static ChunkManager& getInstance()
	{
		static ChunkManager INSTANCE;
		return INSTANCE;
	}

	void createNewWorld();

	void loadWorld(std::string& worldName);

	void saveWorld(std::string& worldName);

	Voxel getVoxel(int x, int y, int z);

	char getCubeId(int x, int y, int z);

	bool isSolid(int x, int y, int z);

	bool isAir(int x, int y, int z);

	void removeCube(int x, int y, int z);

	void setCube(int x, int y, int z, char id);

	// Return all the data instead of querying
	bool intersectWithSolidCube(glm::vec3 origin, glm::vec3 direction,
			glm::vec3 &intersected, glm::vec3 &previous, float searchLength);

	// ########################################################
	// Instance Variables #####################################
	// ########################################################

private:

	std::shared_ptr<Chunk> chunks
		[config::chunk_data::NUMBER_OF_CHUNKS_X]
		[config::chunk_data::NUMBER_OF_CHUNKS_Y]
		[config::chunk_data::NUMBER_OF_CHUNKS_Z];

	glm::vec3 intersectedCube;
	glm::vec3 previousCube;

};

} /* namespace chunk */

#endif /* SRC_MODEL_WORLD_CHUNK_CHUNKMANAGER_H_ */
