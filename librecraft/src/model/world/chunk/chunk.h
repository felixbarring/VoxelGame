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

	/**
	 * Makes sure that the graphics::GraphicalChunk used by this Chunk is destroyed by the graphics::ChunkBatcher.
	 */
	~Chunk();

// ########################################################
// Member Functions########################################
// ########################################################

	/**
	 * @name Creation Group
	 * These functions needs to be called in order to finish the creation of the Chunk. When loading a group of
	 * Chunks, first call Chunk::create. Then Chunk::doSunLightning followed by collecting the light from the neighbors
	 * that are old/not in the new group of created/loaded Chunks. Then call Chunk::pChunkropagateLights to complete the
	 * lightning work that is prepared by doSunLightning and collecLightFrom* functions. The reason why these are not a
	 * single function is to make it possible to run the code in parallel.
	 */
	///@{

	/**
	 * This function fills the Chunk with data for the cubes. If there exists a file that corresponds to the location
	 * and worldName, the data will be loaded from file. Otherwise it will be generated using noise algorithms. This
	 * function can be run in parallel with other instances of Chunk, it will not access data from neighbor Chunks or
	 * any other data that is not read only.
	 */
	void create();

	/**
	 * This function spreads light from the sky downwards. The result is stored in the Chunk and will be used by
	 * Chunk::propagateLights to do the lightning computations. This function can be run in parallel with other
	 * instances of Chunk, it will not access data from neighbor Chunks or any other data that is not read only.
	 */
	void doSunLightning();

	/**
	 * This function calls all of the four functions:
	 * Chunk::collectLightFromRightNeighbor, Chunk::collectLightFromLeftNeighbor,
	 * Chunk::collectLightFromBackNeighbor, Chunk::collectLightFromFrontNeighbor
	 */
	void collectLightFromAllNeighbors();

	/**
	 * This function checks if the light in the right neighbor should light up cubes in this Chunk, and if so adds
	 * light to the cubes that will be used in Chunk::propagateLights. It is not safe to run this in parallel with a
	 * neighbor. It is however safe to run in parallel with other Chunk instances that are not a neighbor.
	 */
	void collectLightFromRightNeighbor();

	/**
	 * This function checks if the light in the right neighbor should light up cubes in this Chunk, and if so adds
	 * light to the cubes that will be used in Chunk::propagateLights. It is not safe to run this in parallel with a
	 * neighbor. It is however safe to run in parallel with other Chunk instances that are not a neighbor.
	 */
	void collectLightFromLeftNeighbor();

	/**
	 * This function checks if the light in the right neighbor should light up cubes in this Chunk, and if so adds
	 * light to the cubes that will be used in Chunk::propagateLights. It is not safe to run this in parallel with a
	 * neighbor. It is however safe to run in parallel with other Chunk instances that are not a neighbor.
	 */
	void collectLightFromBackNeighbor();

	/**
	 * This function checks if the light in the right neighbor should light up cubes in this Chunk, and if so adds
	 * light to the cubes that will be used in Chunk::propagateLights. It is not safe to run this in parallel with a
	 * neighbor. It is however safe to run in parallel with other Chunk instances that are not a neighbor.
	 */
	void collectLightFromFrontNeighbor();

	/**
	 * This function uses the results from Chunk::doSunLightning and collecLightFrom* to calculate the lightning of the Chunk.
	 * It is not safe to run this in parallel with a neighbor or a neighbors neighbor. That is, there must be a
	 * distance of two chunks between two Chunks for it to be safe to run them in parallel.
	 */
	void propagateLights();

	///@}

	/**
	 * Forces all the graphical sections to update even if they are not dirty. Chunk::updateGraphics should be called after
	 * for it to have effect.
	 */
	void forceUpdateGraphics();

	/**
	 * Uses the graphics::ChunkBatcher to update all the graphical representations of this Chunk that are dirty.
	 */
	void updateGraphics(bool highPriority = false);

	/**
	 * @return The x location of the Chunk
	 */
	int getXLocation() { return m_xLocation; }

	/**
	 * @return The z location of the Chunk
	 */
	int getZLocation() { return m_zLocation; }

	/**
	 * Returns a Voxel at the specified location.
	 * It is the callers responsibility to make sure that the arguments are in the correct range.
	 * The range should be between 0 and the chunk width, height, depth specified in data::chunk_data.
	 *
	 * @param x The x location of the Voxel
	 * @param y The y location of the Voxel
	 * @param z The z location of the Voxel
	 * @return The Voxel at the specified location.
	 */
	Voxel getVoxel(int x, int y, int z);

	/**
	 * Changes the cube id for the Voxel at the specified location to the new id.
	 * It is the callers responsibility to make sure that the arguments are in the correct range.
	 * The range should be between 0 and the chunk width, height, depth specified in data::chunk_data.
	 * If the Voxel	id is data::cube_data::BED_ROCK the id will not be changed.
	 * Else the graphics will be updated and the Chunk will be marked as dirty, meaning that it needs
	 * to be saved to disk when destroyed.
	 *
	 * @param x The x location of the Voxel
	 * @param y The y location of the Voxel
	 * @param z The z location of the Voxel
	 * @param id The new id for the Voxel
	 */
	void setCube(int x, int y, int z, char id);


	/**
	 * Sets the right neighbor of this Chunk
	 * @param chunk The new right Chunk
	 */
	void setRightNeighbor(std::shared_ptr<Chunk> chunk) { m_rightNeighbor = chunk; }

	/**
	 * Sets the left neighbor of this Chunk
	 * @param chunk The new left neighbor
	 */
	void setLeftNeighbor(std::shared_ptr<Chunk> chunk) { m_leftNeighbor = chunk; }

	/**
	 * Sets the front neighbor of this Chunk
	 * @param chunk The new front neighbor
	 */
	void setFrontNeighbor(std::shared_ptr<Chunk> chunk) { m_frontNeighbor = chunk; }

	/**
	 * Sets the back neighbor of this Chunk
	 * @param chunk The new back neighbor
	 */
	void setBackNeighbor(std::shared_ptr<Chunk> chunk) { m_backNeighbor = chunk; }

	/**
	 * @return The right neighbor
	 */
	std::shared_ptr<Chunk> getRightNeighbor() { return m_rightNeighbor; }

	/**
	 * @return The left neighbor
	 */
	std::shared_ptr<Chunk> getLeftNeighbor() { return m_leftNeighbor; }

	/**
	 * @return The front neighbor
	 */
	std::shared_ptr<Chunk> getFrontNeighbor() { return m_frontNeighbor; }

	/**
	 * @return The back neighbor
	 */
	std::shared_ptr<Chunk> getBackNeighbor() { return m_backNeighbor; }

	/**
	 * Used when a chunk should be removed from memory. Clears all pointers from this chunk to its neighbors and also
	 * all the pointers in the neighbors pointing to this Chunk
	 */
	void removeAllNeighbors();

	/**
	 * Saved the chunk to disk
	 * @param worldName Used to generate the name of this Chunk
	 */
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

	const int m_xLocation;
	const int m_zLocation;
	bool m_isDirty;

	std::string m_name{};

	const int m_directSunlight = config::graphics_data::directSunlightValue;

	const int m_width = config::chunk_data::CHUNK_WIDTH_AND_DEPTH;
	const int m_height = config::chunk_data::CHUNK_HEIGHT;
	const int m_depth = config::chunk_data::CHUNK_WIDTH_AND_DEPTH;

	std::vector<glm::vec3> m_lightsToPropagate{};

	std::vector<std::vector<std::vector<Voxel>>> m_cubes{};

	std::vector<int> m_graphicalChunksIds{};

	std::shared_ptr<Chunk> m_rightNeighbor{};
	std::shared_ptr<Chunk> m_leftNeighbor{};

	std::shared_ptr<Chunk> m_frontNeighbor{};
	std::shared_ptr<Chunk> m_backNeighbor{};

	std::set<int> m_dirtyRegions{};
};

}

#endif /* SRC_MODEL_WORLD_CHUNK_CHUNK_H_ */
