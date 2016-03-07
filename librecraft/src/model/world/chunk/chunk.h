#ifndef SRC_MODEL_WORLD_CHUNK_CHUNK_H_
#define SRC_MODEL_WORLD_CHUNK_CHUNK_H_

#include <iostream>
#include <memory>
#include <vector>
#include <array>
#include <glm/glm.hpp>
#include <map>
#include <set>

#include "../../../config/data.h"
#include "../../../util/voxel.h"
#include "../../../graphics/graphicalChunk.h"

using std::vector;

namespace chunk {

class Chunk {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	Chunk(int x, int z);

	Chunk(std::string name, int x, int z);

	virtual ~Chunk();

// ########################################################
// Member Functions########################################
// ########################################################

	Voxel getVoxel(int x, int y, int z);

	char getCubeId(int x, int y, int z);

	void setCube(int x, int y, int z, char id);

	void setLeftNeighbor(std::shared_ptr<Chunk> chunk);

	void setRightNeighbor(std::shared_ptr<Chunk> chunk);

	void setFrontNeighbor(std::shared_ptr<Chunk> chunk);

	void setBackNeighbor(std::shared_ptr<Chunk> chunk);

	void removeAllNeighbors();

	void updateLightning();

	void updateGraphics();

	// Loading/Storing

	void storeChunk(std::string worldName, int x, int z);

// ########################################################
// Implementation #########################################
// ########################################################

private:

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
	void doSunLightning(std::vector<glm::vec3> &lightPropagate, int x, int y,
			int z);

	// Much Optimized version of updateLightning
	void updateLightning2();

	void collectLightFromRightNeighbor(std::vector<glm::vec3> &lightPropagate);

	void collectLightFromLeftNeighbor(std::vector<glm::vec3> &lightPropagate);

	void collectLightFromBackNeighbor(std::vector<glm::vec3> &lightPropagate);

	void collectLightFromFrontNeighbor(std::vector<glm::vec3> &lightPropagate);

	void propagateLight(int x, int y, int z);

	void updateDirtyRegions(int y);

	void dePropagateLight(int x, int y, int z);

	int highestLightValueFromNeighbors(int x, int y, int z);

	bool isInDirectSunlight(int x, int y, int z);

	int m_xLocation, m_zLocation;
	bool m_isDirty;

	// Should be somewhere else?
	const int m_directSunlight = 15;

	int m_width = config::chunk_data::CHUNK_WIDTH;
	int m_height = config::chunk_data::CHUNK_HEIGHT;
	int m_depth = config::chunk_data::CHUNK_DEPTH;

	std::vector<std::vector<std::vector<Voxel>>> m_vec;

	std::vector<std::shared_ptr<graphics::GraphicalChunk>> m_graphicalChunks;

	std::shared_ptr<Chunk> m_rightNeighbor;
	std::shared_ptr<Chunk> m_leftNeighbor;

	std::shared_ptr<Chunk> m_frontNeighbor;
	std::shared_ptr<Chunk> m_backNeighbor;

	std::set<int> dirtyRegions{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
		14, 15,};

};

}

#endif /* SRC_MODEL_WORLD_CHUNK_CHUNK_H_ */
