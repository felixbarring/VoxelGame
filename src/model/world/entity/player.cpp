
#include "player.h"

#include <iostream>

#include "../../../graphics/camera.h"
#include "../chunk/chunkManager.h"


namespace entity {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

Player::Player(util::Input& in):
	location{0,0,0},
	speed{0,0,0},
	input(in),
	boundingBox{location.x - 0.5f, location.x + 0.5f, location.y - 1.0f, location.y + 1.0f, location.z - 0.5f, location.z + 0.5f}
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

	speed.x = 0;
	speed.z = 0;

	if (input.moveForward || input.moveBackward) {

		int direction = 1;
		if (input.moveBackward)
			direction = -1;

		glm::vec3 dummy = viewDirection.getViewDirection();
		dummy.y = 0;
		dummy = direction * movementSpeed * glm::normalize(dummy);

		speed.x = dummy.x;
		speed.z = dummy.z;
	}

	if (input.moveRight || input.moveLeft) {

		int direction = 1;
		if (input.moveLeft)
			direction = -1;

		glm::vec3 dummy = viewDirection.getRightDirection();
		dummy.y = 0;
		dummy = direction * movementSpeed * glm::normalize(dummy);

		speed.x += dummy.x;
		speed.z += dummy.z;
	}

	location += speed;

	graphics::Camera::getInstance().setLocation(location.x, location.y, location.z);
	graphics::Camera::getInstance().setViewDirection(viewDirection.getViewDirection());
	graphics::Camera::getInstance().setUpDirection(viewDirection.getUpDirection());

	if (input.action1) {
		chunk::ChunkManager::getInstance();
		if (chunk::ChunkManager::getInstance().intersectWithSolidCube(location, viewDirection.getViewDirection(), 5)) {
			glm::vec3 selectedCube = chunk::ChunkManager::getInstance().getLocationOfInteresectedCube();
			chunk::ChunkManager::getInstance().removeCube(selectedCube.x, selectedCube.y, selectedCube.z);
		}
	}


}

void Player::setLocation(float x, float y, float z)
{
	location = glm::vec3(x,y,z);
}

} /* namespace entity */
