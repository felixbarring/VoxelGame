#ifndef SRC_GRAPHICS_CHUNKBATCHER_H_
#define SRC_GRAPHICS_CHUNKBATCHER_H_

#include <memory>
#include <mutex>
#include <tuple>
#include <vector>

#include "camera.h"
#include "graphicalChunk.h"

#include "shaderProgram.h"
#include "texture/textureArray.h"

namespace graphics {

/**
 * @brief This class is used to manage and draw chunks efficiently.
 *
 * Batches can be added and removed using addBatch and removeBatch. The draw
 * function will draw all the currently added batches. It will also use the
 * current state set functions that will affect how the drawing is done. The
 * addBatch and removeBatch is designed so that they can be called concurrently
 * and saves work that must be done on a thread that has an opengl context for
 * in queues that the draw function will be responsible for handling.
 */
class ChunkBatcher {
public:
  ChunkBatcher(Camera& camera);

  ChunkBatcher(ChunkBatcher const&) = delete;

  void operator=(ChunkBatcher const&) = delete;

  using VoxelMatrix = std::vector<std::vector<std::vector<Voxel>>>;

  /**
   * @brief Adds a batch that will be used to render when calling draw.
   *
   * The added batch will be rendered when calling the draw function. The
   * batch will be used for rendering as long as the user does not call remove
   * batch with the returned id, so a batch should only be added once.
   *
   * The 'uploadData' function of the GraphicalChunk should not have been called
   * before adding it. It is this ChunkBatcher intance responsibility to call
   * it with the main thread with the opengl context in order to avoid opengl
   * errors. The batch's data will be uploaded when calling the draw function.
   *
   * If the batch replaces an existing batch, the old batch id should be
   * provided so that the old batch can be removed in the same frame as the
   * new one is added to avoid flickering where a batch is removed and a frame
   * is drawn without any replacement batch. If there was no previous batch,
   * -1 should be used to indicate that there is no previous batch to remove.
   *
   * This function is thread safe in that it can be called by several threads
   * concurrently.
   *
   * Returns an id that can be used as a handle to remove the batch with the
   * removeBatch function.
   *
   * @param replaceId id of the batch that the new batch replaces. If there is
   *                  no such batch, -1 should be provided.
   * @param batch The graphical chunk that will be rendered by the draw
   *              function.
   * @param highPriority Indicates that this batch needs to be created in the
   *                     current frame. If false the chunk will be put in a
   *                     queue that adds a limited amount of batches each
   *                     frame. This is to provided a smoother workload when
   *                     adding many batches.
   * @return An id that should be used with removeBatch when the batch should
   *         be removed.
   */
  int addBatch(int replaceId,
               std::shared_ptr<GraphicalChunk> batch,
               bool highPriority = false);

  /**
   * @brief Used to remove a batch that is no longer supposed to be drawn.
   *
   * When calling this function, the batch will be added to a queue for
   * removal. It will not be done until the draw function is called. This is
   * because the thread doing opengl calls needs to have an opengl context,
   * which only the thread that calls draw should have.
   *
   * It is safe to call this function from a thread different from the main.
   *
   * @param id The id handle that was returned from addBatch when creating the
   *           batch that should be removed.
   */
  void removeBatch(int id);

  /**
   * @brief Draws all of the batched GraphicalChunks.
   *
   * This function should be called with a thread that has an opengl context.
   */
  void draw();

  /**
   * @brief Sets the sun strength of the rendering.
   *
   * This function will affect how bright the world is.
   *
   * @param value The value of the sun strength. The value should be in the
   *              range (0, 1) where 0 means no sunlight and 1 means max
   *              sunlight.
   */
  void setSunStrenght(double value);

private:
  void addAndRemoveBatches();

  Camera& m_camera;

  int m_idCounter{0};
  std::map<int, std::shared_ptr<GraphicalChunk>> m_batches{};

  double m_sunStrength{1};
  std::mutex m_mutex{};

  static constexpr int m_noRemove{-1};

  using batches =
    std::vector<std::tuple<int, int, std::shared_ptr<GraphicalChunk>>>;
  batches m_batchesToAdd{};
  batches m_batchesToAddHP{};

  std::vector<int> m_batchesToBeRemoved{};

  std::unique_ptr<ShaderProgram> m_program{};
  texture::TextureArray& m_texture;
};
}

#endif /* SRC_GRAPHICS_CHUNKBATCHER_H_ */
