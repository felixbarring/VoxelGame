#ifndef SRC_GUI_IMAGE_H_
#define SRC_GUI_IMAGE_H_

#include <string>

#include "../graphics/sprite.h"

namespace gui {

class Image {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	Image(int x, int y, int width, int height, const std::string image);

// ########################################################
// Member Functions########################################
// ########################################################

	void draw();

// ########################################################
// Implementation #########################################
// ########################################################

private:

	std::shared_ptr<graphics::Sprite> sprite;

};

} /* namespace gui */

#endif /* SRC_GUI_IMAGE_H_ */