
#ifndef SRC_MODEL_ENTITY_PLAYER_H_
#define SRC_MODEL_ENTITY_PLAYER_H_

#include "ientity.h"

namespace entity
{

class Player : public IEntity {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	Player();
	virtual ~Player();

// ########################################################
// Member Functions########################################
// ########################################################

	void update(float timePassed) override;

// ########################################################
// Instance Variables #####################################
// ########################################################

	float xLocation, yLocation, zLocation;

};

}

#endif /* SRC_MODEL_ENTITY_PLAYER_H_ */
