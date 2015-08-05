
#ifndef SRC_MODEL_WORLD_ENTITY_PLAYER_H_
#define SRC_MODEL_WORLD_ENTITY_PLAYER_H_

#include <glm/glm.hpp>

#include "ientity.h"
#include "../collision/aabb.h"
#include "../../../util/input.h"
#include "../../../graphics/viewDirection.h"

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
	glm::vec3 speed;
	graphics::ViewDirection viewDirection;
	util::Input& input;
	collision::AABB boundingBox;
	float movementSpeed = 0.1;

};

} /* namespace entity */

#endif /* SRC_MODEL_WORLD_ENTITY_PLAYER_H_ */
