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
	 * @name Creation Group
	 * These functions needs to be called in order to finish the creation of the chunk. When loading a group of
	 * chunks, first call create. Then doSunLightning followed by collecting the light with from the neighbors that
	 * are old/not in the new group of created/loaded chunks. Then call propagateLights to complete the lightning
	 * work that is prepared by doSunLightning and collecLightFrom* functions. The reason why these are not a single
	 * function is to make it possible to run the code in parallel.
	 */
	///@{

	/**
	 * This function fills the chunk with data for the cubes. If there exists a file that corresponds to the location
	 * and worldName, the data will be loaded from file. Otherwise it will be generated using noice algorithms. This
	 * function can be run in parallel with other instances of chunk, it will not access data from neighbor chunks or
	 * any other data that is not read only.
	 */
	void create();

	/**
	 * This function spreads light from the sky downwards. The result is stored in the chunk and will be used by
	 * propagateLights to do the lightning computations. This function can be run in parallel with other instances of
	 * chunk, it will not access data from neighbor chunks or any other data that is not read only.
	 */
	void doSunLightning();

	/**
	 * This function calls all of the four functions:
	 * collectLightFromRightNeighbor, collectLightFromLeftNeighbor,
	 * collectLightFromBackNeighbor, collectLightFromFrontNeighbor
	 */
	void collectLightFromAllNeighbors();

	/**
	 * This function checks if the light in the right neighbor should light up cubes in this chunk, and if so adds
	 * light to the cubes that will be used in propagateLights. It is not safe to run this in parallel with a
	 * neighbor. It is however safe to run in parallel with other chunk instances that are not a neighbor.
	 */
	void collectLightFromRightNeighbor();

	/**
	 * This function checks if the light in the left neighbor should light up cubes in this chunk, and if so adds
	 * light to the cubes that will be used in propagateLights. It is not safe to run this in parallel with a
	 * neighbor. It is however safe to run in parallel with other chunk instances that are not a neighbor.
	 */
	void collectLightFromLeftNeighbor();

	/**
	 * This function checks if the light in the back neighbor should light up cubes in this chunk, and if so adds
	 * light to the cubes that will be used in propagateLights. It is not safe to run this in parallel with a
	 * neighbor. It is however safe to run in parallel with other chunk instances that are not a neighbor.
	 */
	void collectLightFromBackNeighbor();

	/**
	 * This function checks if the light in the front neighbor should light up cubes in this chunk, and if so adds
	 * light to the cubes that will be used in propagateLights. It is not safe to run this in parallel with a
	 * neighbor. It is however safe to run in parallel with other chunk instances that are not a neighbor.
	 */
	void collectLightFromFrontNeighbor();

	/**
	 * This function uses the results from doSunLightning and collecLightFrom* to calculate the lightning of the chunk.
	 * It is not safe to run this in parallel with a neighbor or a neighbors neighbor. That is, there must be a
	 * distance of two chunks between to chunks for it to be safe to run them in parallel.
	 */
	void propagateLights();

	///@}

	/**
	 * Does the work on updating the graphicalChunks that is safe to do in parallel with another instance of chunk.
	 * updateGraphics needs to be called after this function for it to have effect.
	 */
	void prepareUpdateGraphics();

	/**
	 * Forces all the graphical sections to update even if they are not dirty. updateGraphics should be called after
	 * for it to have effect.
	 */
	void forcePrepareUpdateGraphics();

	/**
	 * Updates the graphical chunks in the chunkBatcher. prepareUpdateGraphics should have been called before.
	 * Not thread safe.
	 */
	void updateGraphics();

	/**
	 * @return The x location of the chunk
	 */
	int getXLocation() { return m_xLocation; }

	/**
	 * @return The z location of the chunk
	 */
	int getZLocation() { return m_zLocation; }

	Voxel getVoxel(int x, int y, int z);

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

	/**
	 * Only does the lightning for one row.
	 *
	 * @param lightPropagate
	 * @param x
	 * @param y
	 * @param z
	 */
	void doSunLightning(std::vector<glm::vec3> &lightPropagate, int x, int y, int z);

	void propagateLight(int x, int y, int z);

	void updateDirtyRegions(int y);

	void dePropagateLight(int x, int y, int z, int _lightValue = -1);

	int highestLVFromNeighbors(int x, int y, int z);

	bool isInDirectSunlight(int x, int y, int z);

	bool containsOnlyAir(int region);

	const int m_xLocation;
	const int m_zLocation;
	bool m_isDirty;

	std::string m_name{};

	// TODO Should be somewhere else?
	const int m_directSunlight = 15;

	const int m_width = config::chunk_data::CHUNK_WIDTH_AND_DEPTH;
	const int m_height = config::chunk_data::CHUNK_HEIGHT;
	const int m_depth = config::chunk_data::CHUNK_WIDTH_AND_DEPTH;

	std::vector<glm::vec3> m_lightsToPropagate{};

	std::vector<std::vector<std::vector<Voxel>>> m_cubes{};

	std::vector<std::shared_ptr<graphics::GraphicalChunk>> m_graphicalChunks{};
	std::vector<std::shared_ptr<graphics::GraphicalChunk>> m_graphicalChunksToBeRemoved{};
	std::vector<std::shared_ptr<graphics::GraphicalChunk>> m_graphicalChunksToBeAdded{};


	std::shared_ptr<Chunk> m_rightNeighbor{};
	std::shared_ptr<Chunk> m_leftNeighbor{};

	std::shared_ptr<Chunk> m_frontNeighbor{};
	std::shared_ptr<Chunk> m_backNeighbor{};

	std::set<int> m_dirtyRegions{};
};

}

#endif /* SRC_MODEL_WORLD_CHUNK_CHUNK_H_ */
