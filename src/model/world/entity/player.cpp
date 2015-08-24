
#include "player.h"

#include <iostream>

#include "../../../graphics/camera.h"
#include "../chunk/chunkManager.h"
#include "../../../graphics/cubeBatcher.h"

namespace entity {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

Player::Player(util::Input& in):
	location{0,0,0},
	speed{0,0,0},
	input(in),
	boundingBox{location.x - 0.5f, location.x + 0.5f, location.y - 1.0f, location.y + 1.0f, location.z - 0.5f, location.z + 0.5f},
	transform{0,0,0}
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

	if (input.moveForwardActive || input.moveBackwardActive) {

		int direction = 1;
		if (input.moveBackwardActive)
			direction = -1;

		glm::vec3 dummy = viewDirection.getViewDirection();
		dummy.y = 0;
		dummy = direction * movementSpeed * glm::normalize(dummy);

		speed.x = dummy.x;
		speed.z = dummy.z;
	}

	if (input.moveRightActive || input.moveLeftActive) {

		int direction = 1;
		if (input.moveLeftActive)
			direction = -1;

		glm::vec3 dummy = viewDirection.getRightDirection();
		dummy.y = 0;
		dummy = direction * movementSpeed * glm::normalize(dummy);

		speed.x += dummy.x;
		speed.z += dummy.z;
	}

	location += speed;

	graphics::Camera::getInstance().updateView(glm::vec3(location.x, location.y, location.z), viewDirection.getViewDirection(), viewDirection.getUpDirection());

	if (chunk::ChunkManager::getInstance().intersectWithSolidCube(location, viewDirection.getViewDirection(), 5)) {
		glm::vec3 selectedCube = chunk::ChunkManager::getInstance().getLocationOfInteresectedCube();
		if (input.action1Pressed) {
			chunk::ChunkManager::getInstance().removeCube(selectedCube.x, selectedCube.y, selectedCube.z);
			return;
		}
		transform.setLocation(selectedCube.x, selectedCube.y, selectedCube.z);
		graphics::CubeBatcher::getInstance().addBatch(1, transform);
	}

}

void Player::setLocation(float x, float y, float z)
{
	location = glm::vec3(x,y,z);
}

} /* namespace entity */
