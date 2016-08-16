#ifndef SRC_MODEL_WORLD_CHUNK_CHUNK_H_
#define SRC_MODEL_WORLD_CHUNK_CHUNK_H_

#include <iostream>
#include <memory>
#include <vector>
#include <array>
#include <glm/glm.hpp>
#include <map>
#include <set>
#include <thread>
#include <mutex>

#include "../../../config/data.h"
#include "../../../util/voxel.h"
#include "../../../graphics/graphicalChunk.h"
#include "ThreadPool.h"

using std::vector;

namespace chunk {

class Chunk {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	/**
	 * Creates a chunk. The function create needs to be called to complete the creation.
	 *
	 * @param name The name of the world that the chunk is in
	 * @param x The x coordinate of the chunk
	 * @param z The z coordinate of the chunk
	 */
	Chunk(std::string worldName, int x, int z);

	virtual ~Chunk();

// ########################################################
// Member Functions########################################
// ########################################################

	/**
	 * This function should be called in order to finish the creation of the chunk. Forgetting to call this function is
	 * an error. If there exists a file that corresponds to the location and worldName, the data will be loaded from
	 * file. Otherwise it will be generated using noice algorithms. This function can be run in parallel, it will not
	 * try to access data from neighbor chunks or any other data that is not read only.
	 */
	void create();

	/**
	 * @return The x location of the chunk
	 */
	int getXLocation() { return m_xLocation; }

	/**
	 * @return The z location of the chunk
	 */
	int getZLocation() { return m_zLocation; }

	Voxel getVoxel(int x, int y, int z);

	char getCubeId(int x, int y, int z);

	void setCube(int x, int y, int z, char id);

	void setLeftNeighbor(std::shared_ptr<Chunk> chunk);

	void setRightNeighbor(std::shared_ptr<Chunk> chunk);

	void setFrontNeighbor(std::shared_ptr<Chunk> chunk);

	void setBackNeighbor(std::shared_ptr<Chunk> chunk);

	/**
	 * Used when a chunk should be removed from memory. Clears all pointers from this chunk to its neighbors and also
	 * all the pointers in the neighbors pointing to this chunk
	 */
	void removeAllNeighbors();

	void updateLightning();

	void updateGraphics();

	void storeChunk(std::string worldName);

// ########################################################
// Implementation #########################################
// ########################################################

private:

	std::string createChunkName(std::string worldName);

	void loadChunk();

	void generateChunk();

	/**
	 * Gets the voxel at the given x, y, z location.
	 * Will work if the voxel is in a neighbor chunk and adjacent to the chunk.
	 *
	 */
	Voxel* getVoxel2(int x, int y, int z);

	void updateLightningCubeRemoved(Voxel& voxel, int x, int y, int z);

	void updateLightningCubeAdded(int x, int y, int z);

	void updateNeighborGraphics();

	void doSunLightning(std::vector<glm::vec3> &lightPropagate);

	/**
	 * Only does the lightning for one row.
	 *
	 * @param lightPropagate
	 * @param x
	 * @param y
	 * @param z
	 */
	void doSunLightning(std::vector<glm::vec3> &lightPropagate, int x, int y, int z);

	void collectLightFromRightNeighbor(std::vector<glm::vec3> &lightPropagate);

	void collectLightFromLeftNeighbor(std::vector<glm::vec3> &lightPropagate);

	void collectLightFromBackNeighbor(std::vector<glm::vec3> &lightPropagate);

	void collectLightFromFrontNeighbor(std::vector<glm::vec3> &lightPropagate);

	void propagateLight(int x, int y, int z);

	void updateDirtyRegions(int y);

	void dePropagateLight(int x, int y, int z, int _lightValue = -1);

	int highestLVFromNeighbors(int x, int y, int z);

	bool isInDirectSunlight(int x, int y, int z);

	bool containsOnlyAir(int region);

	const int m_xLocation;
	const int m_zLocation;
	bool m_isDirty;
//	bool m_

	std::string m_name{};

	// Should be somewhere else?
	const int m_directSunlight = 15;

	const int m_width = config::chunk_data::CHUNK_WIDTH_AND_DEPTH;
	const int m_height = config::chunk_data::CHUNK_HEIGHT;
	const int m_depth = config::chunk_data::CHUNK_WIDTH_AND_DEPTH;

	std::vector<std::vector<std::vector<Voxel>>> m_vec{};

	std::vector<std::shared_ptr<graphics::GraphicalChunk>> m_graphicalChunks{};

	std::shared_ptr<Chunk> m_rightNeighbor{};
	std::shared_ptr<Chunk> m_leftNeighbor{};

	std::shared_ptr<Chunk> m_frontNeighbor{};
	std::shared_ptr<Chunk> m_backNeighbor{};

	std::set<int> m_dirtyRegions{};

	std::condition_variable m_condition{};
};

}

#endif /* SRC_MODEL_WORLD_CHUNK_CHUNK_H_ */
