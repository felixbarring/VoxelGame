
#ifndef SRC_MODEL_WORLD_ENTITY_AABB_H_
#define SRC_MODEL_WORLD_ENTITY_AABB_H_

namespace entity {

class AABB {
public:

	// ########################################################
	// Constructor/Destructor #################################
	// ########################################################

	AABB(double xMinimum, double xMaximum, double yMinimum, double yMaximum, double zMinimum, double zMaximum);
	//AABB();
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

} /* namespace entity */

#endif /* SRC_MODEL_WORLD_ENTITY_AABB_H_ */
