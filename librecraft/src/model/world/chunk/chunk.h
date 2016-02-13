#ifndef SRC_MODEL_WORLD_CHUNK_CHUNK_H_
#define SRC_MODEL_WORLD_CHUNK_CHUNK_H_

#include <iostream>
#include <memory>
#include <vector>
#include <array>
#include <glm/glm.hpp>

#include "../../../config/data.h"
#include "../../../util/voxel.h"
#include "../../../graphics/graphicalChunk.h"

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

	// Some kind of update method here

	Voxel getVoxel(int x, int y, int z);

	Voxel* getVoxel2(int x, int y, int z);

	char getCubeId(int x, int y, int z);

	void setCube(int x, int y, int z, char id);

	void updateLightningCubeRemoved(Voxel& voxel, int x, int y, int z);

	void updateLightningCubeAdded(int x, int y, int z);

	void setLeftNeighbor(std::shared_ptr<Chunk> chunk);

	void setRightNeighbor(std::shared_ptr<Chunk> chunk);

	void setFrontNeighbor(std::shared_ptr<Chunk> chunk);

	void setBackNeighbor(std::shared_ptr<Chunk> chunk);

	void removeAllNeighbores();

	void updateGraphics();

	void updateNeighborGraphics();

// Private shit here

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

	void updateLightning();

	// Much Optimized version of updateLightning
	void updateLightning2();

	void collectLightFromRightNeighbor(std::vector<glm::vec3> &lightPropagate);

	void collectLightFromLeftNeighbor(std::vector<glm::vec3> &lightPropagate);

	void collectLightFromBackNeighbor(std::vector<glm::vec3> &lightPropagate);

	void collectLightFromFrontNeighbor(std::vector<glm::vec3> &lightPropagate);

	void propagateLight(int x, int y, int z);

	// Removed light that has been propagated from the voxel at x y z
	void dePropagateLight(int x, int y, int z);

	int highestLightValueFromNeighbors(int x, int y, int z);

	bool isInDirectSunlight(int x, int y, int z);

	// Loading/Storing

	void storeChunk(std::string worldName, int x, int z);

// ########################################################
// Implementation #########################################
// ########################################################

private:

	int m_xLocation, m_zLocation;
	bool m_isDirty;

	// Should be somewhere else?
	const int m_directSunlight = 15;

	int m_width = config::chunk_data::CHUNK_WIDTH;
	int m_height = config::chunk_data::CHUNK_HEIGHT;
	int m_depth = config::chunk_data::CHUNK_DEPTH;

	std::vector<std::vector<std::vector<Voxel>>> m_vec;

	std::shared_ptr<graphics::GraphicalChunk> m_graphicalChunk;

	std::shared_ptr<Chunk> m_rightNeighbor;
	std::shared_ptr<Chunk> m_leftNeighbor;

	// TODO Can not refactor, fix this !?!
	std::shared_ptr<Chunk> frontNeighbor;
	std::shared_ptr<Chunk> backNeighbor;

};

}

#endif /* SRC_MODEL_WORLD_CHUNK_CHUNK_H_ */
