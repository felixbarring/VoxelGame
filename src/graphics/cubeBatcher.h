
#ifndef SRC_GRAPHICS_CUBEBATCHER_H_
#define SRC_GRAPHICS_CUBEBATCHER_H_

#include <vector>
#include <memory>

#include "texturedCube.h"
#include "camera.h"

namespace graphics {

class CubeBatcher {
private:

	// ########################################################
	// Constructor/Destructor #################################
	// ########################################################

	CubeBatcher();

	virtual ~CubeBatcher();

	CubeBatcher(CubeBatcher const&) = delete;

	void operator=(CubeBatcher const&) = delete;

public:

	static CubeBatcher& getInstance()
	{
		static CubeBatcher INSTANCE;
		return INSTANCE;
	}

	// ########################################################
	// Member Functions########################################
	// ########################################################

	void addBatch(std::shared_ptr<TexturedCube> batch);

	void removeBatch(std::shared_ptr<TexturedCube> batch);

	void draw();

	// ########################################################
	// Instance Variables #####################################
	// ########################################################

	std::vector<std::shared_ptr<TexturedCube>> batches;

};

} /* namespace graphics */

#endif /* SRC_GRAPHICS_CUBEBATCHER_H_ */
