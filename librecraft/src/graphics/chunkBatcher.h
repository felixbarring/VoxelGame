#ifndef SRC_GRAPHICS_CHUNKBATCHER_H_
#define SRC_GRAPHICS_CHUNKBATCHER_H_

#include <vector>
#include <memory>
#include <mutex>

#include "graphicalChunk.h"
#include "camera.h"

#include "texture/textureArray.h"
#include "shaderProgram.h"

namespace graphics {

class ChunkBatcher {
private:

	// ########################################################
	// Constructor/Destructor #################################
	// ########################################################

	ChunkBatcher();

	virtual ~ChunkBatcher() { };

	ChunkBatcher(ChunkBatcher const&) = delete;

	void operator=(ChunkBatcher const&) = delete;

	// ########################################################
	// Member Functions########################################
	// ########################################################

public:

	static ChunkBatcher& getInstance() {
		static ChunkBatcher INSTANCE;
		return INSTANCE;
	}

	/**
	 * Adds a GraphicalChunk to the batcher. It will be rendered as long as it is not removed using the removeBatch
	 * function.
	 */
	void addBatch(std::shared_ptr<GraphicalChunk> batch);

	/**
	 * This function is thread safe. When calling this function, the batch will not be removed until the draw function
	 * is called. This is because the thread doing opengl calls, like destroying a mesh needed for the GraphicalChunk
	 * needs to have an opengl context, which only the main thread does.
	 */
	void removeBatch(std::shared_ptr<GraphicalChunk> batch);

	/**
	 * Draws all of the batched GraphicalChunks.
	 */
	void draw();

	// ########################################################
	// Instance Variables #####################################
	// ########################################################

	std::vector<std::shared_ptr<GraphicalChunk>> m_batches{};

	std::shared_ptr<ShaderProgram> m_program{};
	texture::TextureArray &m_texture;

	std::mutex m_mutex{};
	std::vector<std::shared_ptr<GraphicalChunk>> m_batchesToBeRemoved{};

};

}

#endif /* SRC_GRAPHICS_CHUNKBATCHER_H_ */
