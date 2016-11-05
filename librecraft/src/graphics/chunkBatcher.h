#ifndef SRC_GRAPHICS_CHUNKBATCHER_H_
#define SRC_GRAPHICS_CHUNKBATCHER_H_

#include <vector>
#include <memory>
#include <mutex>
#include <tuple>

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

    ChunkBatcher(ChunkBatcher const&) = delete;

    void operator=(ChunkBatcher const&) = delete;

// ########################################################
// Member Functions########################################
// ########################################################

public:

    using VoxelMatrix = std::vector<std::vector<std::vector<Voxel>>>;

    static ChunkBatcher& getInstance() {
        static ChunkBatcher INSTANCE;
        return INSTANCE;
    }

    /**
     * It is important that the batch is created by the batcher because threading errors...
     * returns an id that can be used to remove the batch with in removeBatch
     *
     * @param x
     * @param y
     * @param z
     * @param data
     * @param right
     * @param left
     * @param back
     * @param front
     * @param hightPriority
     * @return
     */
    int addNewBatch(float x, float y, float z,
            VoxelMatrix &data,
            VoxelMatrix *right,
            VoxelMatrix *left,
            VoxelMatrix *back,
            VoxelMatrix *front, bool hightPriority = false);

    /**
     * Replaces the batch with the id with a new batch.
     * The new batch will be added in the same frame as the old one is replace to avoid flickering.
     *
     * @param id
     * @param x
     * @param y
     * @param z
     * @param data
     * @param right
     * @param left
     * @param back
     * @param front
     * @param hightPriority
     * @return
     */
    int replaceBatch(
            int replaceId,
            float x, float y, float z,
            VoxelMatrix &data,
            VoxelMatrix *right,
            VoxelMatrix *left,
            VoxelMatrix *back,
            VoxelMatrix *front, bool hightPriority = false);

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

    /**
     *
     * @param value
     */
    void setSunStrenght(float value);

// ########################################################
// Implementation #########################################
// ########################################################

private:

    int m_idCounter{0};
    std::map<int, std::shared_ptr<GraphicalChunk>> m_batches{};

    float m_sunStrength{1};
    std::mutex m_mutex{};

    static constexpr int noRemove{-1};

    using batches = std::vector<std::tuple<int, int, std::shared_ptr<GraphicalChunk>>>;
    batches m_batchesToBeAdded{};
    batches m_batchesToBeAddedHighePriority{};

    std::vector<int> m_batchesToBeRemoved{};

    std::shared_ptr<ShaderProgram> m_program{};
    texture::TextureArray &m_texture;

};

}

#endif /* SRC_GRAPHICS_CHUNKBATCHER_H_ */
