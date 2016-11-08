
#include "graphicsManager.h"

#include "chunkBatcher.h"
#include "cubeBatcher.h"

namespace graphics {

void GraphicsManager::setSunStrenght(float value)
{
    ChunkBatcher::getInstance().setSunStrenght(value);
    CubeBatcher::getInstance().setSunStrenght(value);
}


} /* namespace graphics */
