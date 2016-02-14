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

class Player: public IEntity {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	Player();

	virtual ~Player() {
	}
	;

// ########################################################
// Member Functions########################################
// ########################################################

	void update(float timePassed) override;

	void setLocation(float x, float y, float z);

	void updateSpeed(float timePassed);

	void handlePhysics();

	void updateCameraAndTargetCube();

	void intersected(glm::vec3 movement,
			std::vector<std::pair<float, glm::vec3>> &collisions);

// ########################################################
// Implementation #########################################
// ########################################################

private:
	glm::vec3 m_location;
	float m_width {1};
	float m_height {1};
	float m_depth {1};
	float m_gravity {0.3};
	float m_jumpSpeed {0.08};
	int m_selectCubeDistance {5};
	glm::vec3 m_speed;
	graphics::ViewDirection m_viewDirection;
	float m_movementSpeed {7};
	graphics::Transform m_transform;
	int m_cubeUsedForBuilding {0};
};

} /* namespace entity */

#endif /* SRC_MODEL_WORLD_ENTITY_PLAYER_H_ */