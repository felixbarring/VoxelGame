
#ifndef SRC_MODEL_WORLD_ENTITY_PLAYER_H_
#define SRC_MODEL_WORLD_ENTITY_PLAYER_H_

#include <glm/glm.hpp>

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

	Player(util::Input& in);
	virtual ~Player();

// ########################################################
// Member Functions########################################
// ########################################################

	void update(float timePassed) override;

	void setLocation(float x, float y, float z);

// ########################################################
// Instance Variables #####################################
// ########################################################

private:
	glm::vec3 location;
	float width = 1;
	float height = 1;
	float depth = 1;
	glm::vec3 speed;
	graphics::ViewDirection viewDirection;
	util::Input& input;
	AABB boundingBox; //{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
	float movementSpeed = 0.1;
	graphics::Transform transform;
};

} /* namespace entity */

#endif /* SRC_MODEL_WORLD_ENTITY_PLAYER_H_ */
