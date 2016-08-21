#ifndef SRC_MODEL_WORLD_CHUNK_CHUNKMANAGER_H_
#define SRC_MODEL_WORLD_CHUNK_CHUNKMANAGER_H_

#include <memory>
#include <vector>

#include "chunk.h"
#include "../../../config/data.h"
#include "../../../util/voxel.h"
#include "ThreadPool.h"

namespace chunk {

class ChunkManager {
private:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	ChunkManager() {};

	virtual ~ChunkManager() {};

// ########################################################
// Member Functions########################################
// ########################################################

public:

	static ChunkManager& getInstance() {
		static ChunkManager INSTANCE;
		return INSTANCE;
	}

	void createWorld(std::string worldName);

	void saveWorld();

	/**
	 * Should be called each frame.
	 */
	void update();

	Voxel getVoxel(int x, int y, int z);

	char getCubeId(int x, int y, int z);

	bool isSolid(int x, int y, int z);

	bool isAir(int x, int y, int z);

	void removeCube(int x, int y, int z);

	void setCube(int x, int y, int z, char id);

	/**
	 * Sets the center chunk
	 * If the new center is different from the previous,
	 * chunks that are no longer in the distance bounds from
	 * the center chunk will be unloaded and chunks that entered the
	 * bounds will be loaded.
	 *
	 * @param x The x coordinate that will be used to determine the
	 *          center chunks x location
	 * @param z The z coordinate that will be used to determine the
	 *          center chunks z location
	 */
	void setCenter(float x, float z);

	// Return all the data instead of querying
	bool intersectWithSolidCube(glm::vec3 origin, glm::vec3 direction, glm::vec3 &intersected,
			glm::vec3 &previous, float searchLength);

// ########################################################
// Implementation #########################################
// ########################################################

private:

	void connectChunks();

	void moveChunksRight();

	void moveChunksLeft();

	void moveChunksUp();

	void moveChunksDown();

	static constexpr int m_lenghtAcrossMatrix = config::chunk_data::NUMBER_OF_CHUNKS_FROM_MIDDLE_TO_BORDER * 2 + 1;

	std::shared_ptr<Chunk> chunks
		[m_lenghtAcrossMatrix]
		[config::chunk_data::NUMBER_OF_CHUNKS_Y]
		[m_lenghtAcrossMatrix];

	int m_xOffset{0};
	int m_zOffset{0};

	std::string m_worldName{};

	glm::vec3 m_center{4, 0, 4};
	glm::vec3 m_intersectedCube{};
	glm::vec3 m_previousCube{};

	// TODO Figure out better names
	ThreadPool m_threadPool{8};
	ThreadPool m_threadPool2{1};

	std::mutex m_graphicUpdateMutex{};
	std::vector<std::shared_ptr<Chunk>> m_graphicUpdate{};

};

} /* namespace chunk */

#endif /* SRC_MODEL_WORLD_CHUNK_CHUNKMANAGER_H_ */
