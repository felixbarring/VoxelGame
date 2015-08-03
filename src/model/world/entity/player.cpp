
#include "player.h"

#include <iostream>

#include "../../../graphics/camera.h"


namespace entity {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

Player::Player(util::Input& in):
	location{0,0,0},
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

	viewDirection.changeViewDirection(input.mouseXMovement, input.mouseYMovement);
	graphics::Camera::getInstance().setLocation(location.x, location.y, location.z);
	graphics::Camera::getInstance().setViewDirection(viewDirection.getViewDirection());
	graphics::Camera::getInstance().setUpDirection(viewDirection.getUpDirection());

	if (input.action2) {
		std::cout<<"Player is jumping :-)";
	}

	if (input.moveForward) {
		location += movementSpeed * viewDirection.getViewDirection();
	}
	if (input.moveBackward) {
		location -= movementSpeed * viewDirection.getViewDirection();
	}
	if (input.moveRight) {
		location += movementSpeed * viewDirection.getRightDirection();
	}
	if (input.moveLeft) {
		location -= movementSpeed * viewDirection.getRightDirection();
	}


}

void Player::setLocation(float x, float y, float z)
{
	location = glm::vec3(x,y,z);
}

} /* namespace entity */
