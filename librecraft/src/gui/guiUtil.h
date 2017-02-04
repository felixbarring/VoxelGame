#ifndef SRC_GUI_GUIUTIL_H_
#define SRC_GUI_GUIUTIL_H_

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace gui {

glm::mat4 createVirtualToScreen(int virtualWidth, int virtualHeight,
		int realWidth, int realHeight);

glm::vec2 adjustMouse(int virtualWidth, int virtualHeight, int realWidth,
		int realHeight, int mouseX, int mouseY);

}

#endif /* SRC_GUI_GUIUTIL_H_ */
