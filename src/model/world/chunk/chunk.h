
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

namespace chunk
{

class Chunk {
public:

	// ########################################################
	// Constructor/Destructor #################################
	// ########################################################

	Chunk(int x, int y, int z);

	virtual ~Chunk() {};

	// ########################################################
	// Member Functions########################################
	// ########################################################

	// Some kind of update method here

	Voxel& getVoxel(int x, int y, int z);

	char getCubeId(int x, int y, int z);

	void setCube(int x, int y, int z, char id);

	void setLeftNeighbor(std::shared_ptr<Chunk> chunk);

	void setRightNeighbor(std::shared_ptr<Chunk> chunk);

	void setFrontNeighbor(std::shared_ptr<Chunk> chunk);

	void setBackNeighbor(std::shared_ptr<Chunk> chunk);

	void updateGraphics();

private:

	void doSunLightning(std::vector<glm::vec3> &lightPropagate);

	void collectLightFromNeighbors(std::vector<glm::vec3> &lightPropagate);

	void propagateLight(int x, int y, int z);

	// ########################################################
	// Instance Variables #####################################
	// ########################################################

	int xLocation, yLocation, zLocation;

	std::vector<std::vector<std::vector<Voxel>>> vec;

	//Voxel voxels[config::chunk_data::CHUNK_WIDHT][config::chunk_data::CHUNK_HEIGHT][config::chunk_data::CHUNK_DEPTH];
	std::shared_ptr<graphics::GraphicalChunk> graphicalChunk;

	std::shared_ptr<Chunk> rightNeighbor;
	std::shared_ptr<Chunk> leftNeighbor;

	std::shared_ptr<Chunk> frontNeighbor;
	std::shared_ptr<Chunk> backNeighbor;

};

}

#endif /* SRC_MODEL_WORLD_CHUNK_CHUNK_H_ */
