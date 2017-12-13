#ifndef SRC_MODEL_ENTITY_IENTITY_H_
#define SRC_MODEL_ENTITY_IENTITY_H_

namespace entity {

class IEntity {
public:
  virtual ~IEntity(){};

  virtual void update(float timePassed) = 0;
};
}

#endif /* SRC_MODEL_ENTITY_IENTITY_H_ */
