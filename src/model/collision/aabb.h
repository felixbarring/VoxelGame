
#ifndef SRC_MODEL_COLLISION_AABB_H_
#define SRC_MODEL_COLLISION_AABB_H_

class AABB {
public:

	// ########################################################
	// Constructor/Destructor #################################
	// ########################################################

	AABB();
	virtual ~AABB();

	// ########################################################
	// Member Functions########################################
	// ########################################################

	glm::mat4 getViewMatrix();

	void moveForward(float amount);

	void changeViewDirection(float xOffset, float yOffset);

	// ########################################################
	// Instance Variables #####################################
	// ########################################################

};

#endif /* SRC_MODEL_COLLISION_AABB_H_ */
