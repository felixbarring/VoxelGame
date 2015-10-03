
#include "player.h"

#include <iostream>
#include <math.h>

#include "../../../graphics/camera.h"
#include "../chunk/chunkManager.h"
#include "../../../graphics/cubeBatcher.h"

#include "aabb.h"

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

// ########################################################
// Member Functions########################################
// ########################################################

void Player::update(float timePassed)
{

	viewDirection.changeViewDirection(input.mouseXMovement, input.mouseYMovement);

	speed.x = 0;
	speed.y = 0;
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

	if (input.jumpActive || input.goDownActive) {
		int direction = 1;
		if (input.goDownActive)
			direction = -1;

		speed.y = direction * movementSpeed;
	}

	//std::cout << "Location: " << location.x << " " << location.y << " " << location.z << "\n";
	//std::cout << "Speed: " << speed.x << " " << speed.y << " " << speed.z << "\n";

	glm::vec3 destination = location + speed;

	AABB player{ destination.x, destination.x + 1, destination.y - 1, destination.y, destination.z, destination.z + 1 };
	AABB box = AABB::getSweptBroadPhaseBox(player, speed);

	int xStart = std::floor(box.xMin);
	int yStart = std::floor(box.yMin);
	int zStart = std::floor(box.zMin);

	int xEnd = std::floor(box.xMax);
	int yEnd = std::floor(box.yMax);
	int zEnd = std::floor(box.zMax);

	//std::cout << "Start: " << xStart << " " << yStart << " " << zStart << "\n";
	//std::cout << "End: " << xEnd << " " << yEnd << " " << zEnd << "\n";

	int counter = 0;
	bool colided = false;

	for (int i = xStart; i <= xEnd; i++) {
		for (int j = yStart; j <= yEnd; j++) {
			for (int k = zStart; k <= zEnd; k++) {
				AABB cube{i, i + 1, j, j + 1, k, k + 1};

				if (chunk::ChunkManager::getInstance().getCubeId(i, j, k) != config::cube_data::AIR) {
					if (cube.intersects(player)) {
						colided = true;
					}
				}

				counter++;
			}
		}
	}

	if (!colided) {
		location += speed;
	}

	//std::cout << "Number of checks:" << counter << "\n";

	graphics::Camera::getInstance().updateView(glm::vec3(location.x, location.y, location.z),
			viewDirection.getViewDirection(), viewDirection.getUpDirection());

	if (chunk::ChunkManager::getInstance().intersectWithSolidCube(location, viewDirection.getViewDirection(), 5)) {

		glm::vec3 selectedCube = chunk::ChunkManager::getInstance().getLocationOfInteresectedCube();
		if (input.action1Pressed) {
			chunk::ChunkManager::getInstance().removeCube(selectedCube.x, selectedCube.y, selectedCube.z);
			return;
		}

		if (input.action2Pressed) {
			glm::vec3 cube = chunk::ChunkManager::getInstance().getCubeBeforeIntersectedCube();
			chunk::ChunkManager::getInstance().setCube(cube.x, cube.y, cube.z, 1);
		}

		// TODO Dirty hack :o
		transform.setLocation(selectedCube.x + 0.5, selectedCube.y + 0.5, selectedCube.z + 0.5);
		graphics::CubeBatcher::getInstance().addBatch(1, transform);

	}

}

void Player::setLocation(float x, float y, float z)
{
	location = glm::vec3(x,y,z);
}

} /* namespace entity */
