#ifndef SRC_GRAPHICS_SPRITEBATCHER_H_
#define SRC_GRAPHICS_SPRITEBATCHER_H_

#include <memory>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "sprite.h"

#include "shaderProgram.h"

namespace graphics {

class SpriteBatcher {
public:
  SpriteBatcher();

  SpriteBatcher(SpriteBatcher const&) = delete;

  void operator=(SpriteBatcher const&) = delete;

  void addBatch(Sprite& batch);

  void draw();

  void setProjection(glm::mat4 projection);

private:
  std::vector<Sprite*> m_batches;
  glm::mat4 m_projection;
  std::unique_ptr<ShaderProgram> m_program;
};

} /* namespace graphics */

#endif /* SRC_GRAPHICS_SPRITEBATCHER_H_ */
