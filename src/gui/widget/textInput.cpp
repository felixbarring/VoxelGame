
#include "textInput.h"

namespace widget {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

TextInput::TextInput(int id, int x, int y, int width, int height, std::function<void(int, const std::string&)> observer) :
	AbstractWidget(id, x, y, width, height)
{
	this->observer = observer;

}

// ########################################################
// Member Functions########################################
// ########################################################

void TextInput::draw()
{

}

void TextInput::mouseClicked(int button, float x, float y)
{

}

void TextInput::mouseMoved(float x, float y)
{

}

void TextInput::keyPressed(int key, char c)
{

}

} /* namespace widget */
