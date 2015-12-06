
#ifndef SRC_MODEL_WORLD_ENTITY_PLAYER_H_
#define SRC_MODEL_WORLD_ENTITY_PLAYER_H_

#include <glm/glm.hpp>

#include <vector>

#include "ientity.h"
#include "aabb.h"
#include "../../../util/input.h"
#include "../../../graphics/viewDirection.h"
#include "../../../graphics/transform.h"

#include "../../../config/data.h"

namespace entity {

class Player : public IEntity {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	Player();

	virtual ~Player() {};

// ########################################################
// Member Functions########################################
// ########################################################

	void update(float timePassed) override;

	void setLocation(float x, float y, float z);

	void updateSpeed();

	void handlePhysics();

	void updateCameraAndTargetCube();

	void intersected(glm::vec3 movement, std::vector<std::pair<float, glm::vec3>> &collisions);

// ########################################################
// Instance Variables #####################################
// ########################################################

private:
	glm::vec3 location;
	float width = 1;
	float height = 1;
	float depth = 1;
	int selectCubeDistance = 5;
	glm::vec3 speed;
	graphics::ViewDirection viewDirection;
	float movementSpeed = 0.1;
	graphics::Transform transform;
};

} /* namespace entity */

#endif /* SRC_MODEL_WORLD_ENTITY_PLAYER_H_ */
