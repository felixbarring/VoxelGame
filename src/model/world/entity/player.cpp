
#include "player.h"

#include <iostream>
#include <math.h>
#include <memory>
#include <algorithm>

#include "../../../graphics/camera.h"
#include "../chunk/chunkManager.h"
#include "../../../graphics/cubeBatcher.h"

#include "aabb.h"

using namespace std;
using namespace glm;

using namespace util;
using namespace graphics;
using namespace chunk;
using namespace config;

namespace entity {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

Player::Player():
	location{0,0,0},
	speed{0,0,0},
	transform{0,0,0}
{
}

// ########################################################
// Member Functions########################################
// ########################################################

void Player::update(float timePassed)
{

	updateSpeed(timePassed);
	handlePhysics();
	updateCameraAndTargetCube(); // Updates the camera aswell

}

void Player::updateSpeed(float timePassed)
{
	shared_ptr<Input> input = Input::getInstance();
	viewDirection.changeViewDirection(input->mouseXMovement, input->mouseYMovement);

	speed.x = 0;
	speed.z = 0;

	vec3 movementDirection{0.0f, 0.0f, 0.0f};

	if (input->moveForwardActive || input->moveBackwardActive) {

		float direction = 1.0f;
		if (input->moveBackwardActive)
			direction = -1.0f;

		vec3 dummy = viewDirection.getViewDirection();
		dummy.y = 0;
		movementDirection += direction * dummy;

	}

	if (input->moveRightActive || input->moveLeftActive) {

		float direction = 1.0f;
		if (input->moveLeftActive)
			direction = -1.0f;

		vec3 dummy = viewDirection.getRightDirection();
		dummy.y = 0;
		movementDirection += direction * dummy;

	}

	vec3 normalizedMD = movementSpeed * timePassed * normalize(movementDirection);

	// Normalize will give nan if the length is 0
	if (length(movementDirection)) {
		speed.x = normalizedMD.x;
		speed.z = normalizedMD.z;
	}

	// Gravity
	speed.y -= gravity * timePassed;

	// Jump
	if(input->jumpPressed) {
		vector<pair<float, vec3>> collisions;
		intersected(vec3(0, -0.1, 0), collisions);

		// Only jump if the player stands on solid ground.
		if (collisions.size())
			speed.y = jumpSpeed;

	}

	/*
	if (input->jumpActive || input->goDownActive) {
		int direction = 1;
		if (input->goDownActive)
			direction = -1;

		speed.y = direction * movementSpeed;
	} else {
		//speed.y = 0;
	}
	*/

}

void Player::handlePhysics()
{

	vector<pair<float, vec3>> collisions;
	intersected(speed, collisions);

	while(collisions.size()) {

		sort(collisions.begin(), collisions.end(),
			[](pair<float, vec3> a, pair<float, vec3> b)
			{
				return a.first < b.first;
			}
		);

		if (collisions.size()) {
			auto c = collisions[0];
			speed += vec3(-c.second.x * speed.x, -c.second.y * speed.y, -c.second.z * speed.z);
		}

		collisions.clear();
		intersected(speed, collisions);

	}

	location += speed;

}

void Player::updateCameraAndTargetCube()
{
	shared_ptr<Input> input = Input::getInstance();

	Camera::getInstance().updateView(
		vec3(location.x, location.y, location.z),
		viewDirection.getViewDirection(),
		viewDirection.getUpDirection()
	);

	chunk::ChunkManager &chunkManager = chunk::ChunkManager::getInstance();

	vec3 selectedCube;
	vec3 previous;

	if (chunkManager.intersectWithSolidCube(location, viewDirection.getViewDirection(), selectedCube, previous, selectCubeDistance)) {

		if (input->action1Pressed) {
			chunkManager.removeCube(selectedCube.x, selectedCube.y, selectedCube.z);
		} else if (input->action2Pressed) {
			chunkManager.setCube(previous.x, previous.y, previous.z, config::cube_data::BRICK_WALL);
		}

		// TODO Remove hardcoded values
		transform.setLocation(selectedCube.x + 0.5, selectedCube.y + 0.5, selectedCube.z + 0.5);
		CubeBatcher::getInstance().addBatch(cube_data::SELECTED, transform);

	}

}

void Player::intersected(vec3 movement, vector<pair<float, vec3>> &collisions)
{

	AABB start{ location.x - 0.4, location.x + 0.4, location.y - 1.5, location.y, location.z - 0.4, location.z + 0.4 };
	AABB box = AABB::getSweptBroadPhaseBox(start, movement);

	int xStart = floor(box.xMin);
	int yStart = floor(box.yMin);
	int zStart = floor(box.zMin);

	int xEnd = floor(box.xMax);
	int yEnd = floor(box.yMax);
	int zEnd = floor(box.zMax);

	for (int i = xStart; i <= xEnd; i++) {
		for (int j = yStart; j <= yEnd; j++) {
			for (int k = zStart; k <= zEnd; k++) {

				AABB cube{i, i + 1, j, j + 1, k, k + 1};
				vec3 normal;

				if (ChunkManager::getInstance().getCubeId(i, j, k) != cube_data::AIR) {
					vec3 vec;
					float time = AABB::collisionTime(start, cube, vec, movement);

					if (time <= 1 && time >= 0)
						collisions.push_back({time, vec});

				}

			}
		}
	}

}

void Player::setLocation(float x, float y, float z)
{
	location = vec3(x,y,z);
}

} /* namespace entity */
