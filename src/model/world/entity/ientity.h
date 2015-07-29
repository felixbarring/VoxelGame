
#ifndef SRC_MODEL_ENTITY_IENTITY_H_
#define SRC_MODEL_ENTITY_IENTITY_H_

namespace entity
{

class IEntity {

	virtual ~IEntity();

	virtual void update(float timePassed);

};

}

#endif /* SRC_MODEL_ENTITY_IENTITY_H_ */
