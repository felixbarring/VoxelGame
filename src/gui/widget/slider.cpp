
#include "slider.h"

namespace widget {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

Slider::Slider(int id, int x, int y, int width, int height, std::function<void(int)> observer, const std::string &name) :
	AbstractWidget(id, x, y, width, height)
{

}

Slider::~Slider()
{

}

// ########################################################
// Member Functions########################################
// ########################################################

} /* namespace widget */
