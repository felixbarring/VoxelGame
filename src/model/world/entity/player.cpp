
#include "player.h"

#include <iostream>

#include "../../../graphics/camera.h"

namespace entity {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

Player::Player(util::Input& in):
	xLocation{0},
	yLocation{0},
	zLocation{0},
	input(in)
{
}

Player::~Player()
{
}

// ########################################################
// Member Functions########################################
// ########################################################

void Player::update(float timePassed)
{

	graphics::Camera::getInstance().changeViewDirection(input.mouseXMovement, input.mouseYMovement);
	graphics::Camera::getInstance().setLocation(xLocation, yLocation, zLocation);

	if (input.action2) {
		std::cout<<"Player is jumping :-)";
	}

	if (input.moveForward) {
		xLocation++;
	}

}

void Player::setLocation(float x, float y, float z)
{
	xLocation = x;
	yLocation = y;
	zLocation = z;
}

} /* namespace entity */
