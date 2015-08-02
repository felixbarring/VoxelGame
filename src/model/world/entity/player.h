
#ifndef SRC_MODEL_WORLD_ENTITY_PLAYER_H_
#define SRC_MODEL_WORLD_ENTITY_PLAYER_H_

#include "ientity.h"
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

	float xLocation, yLocation, zLocation;
	graphics::ViewDirection viewDirection;
	util::Input& input;

};

} /* namespace entity */

#endif /* SRC_MODEL_WORLD_ENTITY_PLAYER_H_ */
