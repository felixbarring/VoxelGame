
#ifndef SRC_GRAPHICS_CHUNKBATCHER_H_
#define SRC_GRAPHICS_CHUNKBATCHER_H_

#include <vector>
#include <memory>

#include "graphicalChunk.h"
#include "camera.h"

class ChunkBatcher {
private:

	// Should be singleton

	// ########################################################
	// Constructor/Destructor #################################
	// ########################################################

	ChunkBatcher();
	virtual ~ChunkBatcher();

private:

	ChunkBatcher(ChunkBatcher const&) = delete;
	void operator=(ChunkBatcher const&) = delete;

	// ########################################################
	// Member Functions########################################
	// ########################################################

public:

	static ChunkBatcher& getInstance()
	{
		static ChunkBatcher INSTANCE;
		return INSTANCE;
	}

	void addBatch(std::shared_ptr<GraphicalChunk> batch);

	void removeBatch(std::shared_ptr<GraphicalChunk> batch);

	void draw(Camera &camera);

	// ########################################################
	// Instance Variables #####################################
	// ########################################################

	std::vector<std::shared_ptr<GraphicalChunk>> batches;

};

#endif /* SRC_GRAPHICS_CHUNKBATCHER_H_ */
