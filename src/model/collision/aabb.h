
#ifndef SRC_MODEL_COLLISION_AABB_H_
#define SRC_MODEL_COLLISION_AABB_H_

class AABB {
public:

	// ########################################################
	// Constructor/Destructor #################################
	// ########################################################

	AABB(double xMinimum, double xMaximum, double yMinimum, double yMaximum, double zMinimum, double zMaximum);
	virtual ~AABB();

	// ########################################################
	// Member Functions########################################
	// ########################################################

	bool intersects(const AABB &that);

	// ########################################################
	// Instance Variables #####################################
	// ########################################################

	double xMin, xMax, yMin, yMax, zMin, zMax;

};

#endif /* SRC_MODEL_COLLISION_AABB_H_ */
