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

	virtual ~Player() {};

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

	glm::vec3 getViewingDirection();

	glm::vec3 getLastSelectedCube();

	int getBuildingCube() { return m_cubeUsedForBuilding; }

// ########################################################
// Implementation #########################################
// ########################################################

private:
	glm::vec3 m_location{0, 0, 0};
	float m_width{1};
	float m_height{1};
	float m_depth{1};
	float m_gravity{30};
	float m_jumpSpeed{12};
	int m_selectCubeDistance{5};
	float m_movementSpeed{8};

	glm::vec3 m_speed{0, 0, 0};
	glm::vec3 m_frameSpeed{};
	glm::vec3 m_lastSelecteCube{};
	graphics::ViewDirection m_viewDirection{};
	graphics::Transform m_transform{0, 0, 0};
	int m_cubeUsedForBuilding{0};
};

} /* namespace entity */

#endif /* SRC_MODEL_WORLD_ENTITY_PLAYER_H_ */
