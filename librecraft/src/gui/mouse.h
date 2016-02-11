
#ifndef SRC_GUI_MOUSE_H_
#define SRC_GUI_MOUSE_H_

#include <memory>

#include "../graphics/sprite.h"
#include "../gui/guiUtil.h"
#include "../util/input.h"

namespace widget {

class Mouse {


// ########################################################
// Constructor/Destructor #################################
// ########################################################

	Mouse();

	virtual ~Mouse() {};

// ########################################################
// Member Functions########################################
// ########################################################

public:

	static Mouse& getInstance()
	{
		static Mouse instance;
		return instance;
	}

	void update();

	void draw();

	void lock();

	void unlock();

// ########################################################
// Instance Variables #####################################
// ########################################################

	std::shared_ptr<util::Input> m_input;
	bool m_locked{false};

	std::shared_ptr<graphics::Sprite> m_sprite;
	int m_width{20};
	int m_height{20};
};

} /* namespace widget */

#endif /* SRC_GUI_MOUSE_H_ */
