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
	 * It is important that the batch is created by the batcher because threading errors...
	 * returns an id that can be used to remove the batch with in removeBatch
	 *
	 */
	int createBatch(float x, float y, float z,
			std::vector<std::vector<std::vector<Voxel>>> &data,
			std::vector<std::vector<std::vector<Voxel>>> *right,
			std::vector<std::vector<std::vector<Voxel>>> *left,
			std::vector<std::vector<std::vector<Voxel>>> *back,
			std::vector<std::vector<std::vector<Voxel>>> *front);

	/**
	 * This function is thread safe. When calling this function, the batch will not be removed until the draw function
	 * is called. This is because the thread doing opengl calls, like destroying a mesh needed for the GraphicalChunk
	 * needs to have an opengl context, which only the main thread does.
	 */
	void removeBatch(int id);

	/**
	 * Draws all of the batched GraphicalChunks.
	 */
	void draw();

	// ########################################################
	// Instance Variables #####################################
	// ########################################################

	int m_idCounter{0};
	std::map<int, std::shared_ptr<GraphicalChunk>> m_batches{};

	std::mutex m_mutex{};
	std::vector<std::pair<int, std::shared_ptr<GraphicalChunk>>> m_batchesToBeAdded{};
	std::vector<int> m_batchesToBeRemoved{};

	std::shared_ptr<ShaderProgram> m_program{};
	texture::TextureArray &m_texture;

};

}

#endif /* SRC_GRAPHICS_CHUNKBATCHER_H_ */
