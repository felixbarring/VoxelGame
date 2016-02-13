#ifndef SRC_GRAPHICS_CHUNKBATCHER_H_
#define SRC_GRAPHICS_CHUNKBATCHER_H_

#include <vector>
#include <memory>

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

	virtual ~ChunkBatcher() {
	}
	;

private:

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

	void addBatch(std::shared_ptr<GraphicalChunk> batch);

	void removeBatch(std::shared_ptr<GraphicalChunk> batch);

	void draw();

	// ########################################################
	// Instance Variables #####################################
	// ########################################################

	std::vector<std::shared_ptr<GraphicalChunk>> batches;

	std::shared_ptr<ShaderProgram> program;
	texture::TextureArray &texture;

};

}

#endif /* SRC_GRAPHICS_CHUNKBATCHER_H_ */
