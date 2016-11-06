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

/**
 * \brief This class is used to manage an draw chunks efficiently.
 *
 * Batches can be added and removed using addBatch and removeBatch. The draw function will draw all the currently
 * added batches. It will also use the current state set functions that will affect how the drawing is done.
 * The addBatch and removeBatch is designed so that they can be called concurrently and saves work that must be
 * done on a thread that has an opengl context for in queues that the draw function will be responsible for
 * handling.
 *
 * This class uses the singleton pattern since there should only ever be one instance.
 *
 */
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

    /**
     * \brief Returns the single instance of this class
     *
     * This class uses the singleton pattern.
     *
     * @return The single instance of this class.
     */
    static ChunkBatcher& getInstance() {
        static ChunkBatcher INSTANCE;
        return INSTANCE;
    }

    /**
     *
     * \brief Creates a batch that will be used to render when calling draw.
     *
     * The added batch will be rendered when calling the draw function. The batch will be used for rendering
     * as long as the user does not call remove batch with the returned id, so a batch should only be added once.
     *
     * It is important that the batch is created by the ChunkBatcher because only a thread with an opengl context
     * can upload data to the graphics card. The batch's data will be uploaded when calling the draw function.
     * By default the GraphicalChunk uploading will be spread out on several frames to make the workload on
     * the main thread more stable. If it is important that the change is visible immediately @highPriority should
     * be set to true. All batches with high priority will be uploaded the current frame.
     *
     * If the batch replaces an existing batch, the old batch id should be provided so that the old batch can be
     * removed in the same frame as the new one is added to avoid flickering where a batch is removed and a frame
     * is drawn without any replacement batch. If there was no previous batch, -1 should be used to indicate that
     * there is no previous batch to remove.
     *
     * This function is thread safe in that it can be called by several threads concurrently, but the voxeldata in the
     * arguments should not be modified or read by another thread.
     *
     * Returns an id that can be used as a handle to remove the batch with the removeBatch function.
     *
     * @param id The id of the batch that the new batch replaces. If there is no such batch, -1 should be provided.
     * @param x The x location of the batch.
     * @param y The y location of the batch.
     * @param z The z location of the batch.
     * @param data The data for the chunk, will be used to crate the graphical representation of the Chunk.
     * @param right The data of the right neighbor chunk, can be nullptr if there is no right neighbor.
     * @param left The data of the left neighbor chunk, can be nullptr if there is no left neighbor.
     * @param back The data of the back neighbor chunk, can be nullptr if there is no back neighbor.
     * @param front The data of the front neighbor chunk, can be nullptr if there is no front neighbor.
     * @param hightPriority Indicates that this batch needs to be created in the current frame. If false
     *        the chunk will be put in a queue that adds a limited amount of batches each frame. This is
     *        to provided a smoother workload when adding many batches.
     * @return An id that should be used with removeBatch when the batch shold be removed.
     */
    int addBatch(
            int replaceId,
            float x, float y, float z,
            VoxelMatrix &data,
            VoxelMatrix *right,
            VoxelMatrix *left,
            VoxelMatrix *back,
            VoxelMatrix *front, bool highPriority = false);

    /**
     * \brief Used to remove a batch that is no longer supposed to be drawn.
     *
     * When calling this function, the batch will be added to a queue for removal. It will not be done until the draw
     * function is called. This is because the thread doing opengl calls needs to have an opengl context, which only
     * the thread that calls draw should have.
     *
     * It is safe to call this function from a thread different from the main.
     *
     * @param id The id handle that was returned from addBatch when creating the batch that should be removed.
     */
    void removeBatch(int id);

    /**
     * \brief Draws all of the batched GraphicalChunks.
     *
     * This function should be called with a thread that has an opengl context.
     */
    void draw();

    /**
     * \brief Sets the sun strength of the rendering.
     *
     * This function will affect how bright the world is.
     *
     * @param value The value of the sun strength. The value should be in the range (0, 1) where 0 means no sunlight
     *        and 1 means max sunlight.
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
    batches m_batchesToAdd{};
    batches m_batchesToAddHP{};

    std::vector<int> m_batchesToBeRemoved{};

    std::shared_ptr<ShaderProgram> m_program{};
    texture::TextureArray &m_texture;
};

}

#endif /* SRC_GRAPHICS_CHUNKBATCHER_H_ */
