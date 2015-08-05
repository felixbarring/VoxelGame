
#ifndef SRC_MODEL_WORLD_COLLISION_AABB_H_
#define SRC_MODEL_WORLD_COLLISION_AABB_H_

namespace collision
{

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

	void setBounds(double xMinimum, double xMaximum, double yMinimum, double yMaximum, double zMinimum, double zMaximum);

	bool intersects(const AABB &that);

	// ########################################################
	// Instance Variables #####################################
	// ########################################################

private:

	double xMin, xMax, yMin, yMax, zMin, zMax;

};

}

#endif /* SRC_MODEL_WORLD_COLLISION_AABB_H_ */
