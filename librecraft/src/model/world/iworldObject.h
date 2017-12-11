#ifndef SRC_MODEL_WORLD_IWORLDOBJECT_H_
#define SRC_MODEL_WORLD_IWORLDOBJECT_H_

class IWorldObject
{

  virtual ~IWorldObject();

  virtual void update(float timePassed);
};

#endif /* SRC_MODEL_WORLD_IWORLDOBJECT_H_ */
