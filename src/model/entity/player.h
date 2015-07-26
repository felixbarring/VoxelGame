
#ifndef SRC_MODEL_ENTITY_PLAYER_H_
#define SRC_MODEL_ENTITY_PLAYER_H_

#include "ientity.h"

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

};

#endif /* SRC_MODEL_ENTITY_PLAYER_H_ */
