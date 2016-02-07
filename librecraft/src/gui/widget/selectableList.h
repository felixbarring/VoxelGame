
#ifndef SRC_GUI_WIDGET_SELECTABLELIST_H_
#define SRC_GUI_WIDGET_SELECTABLELIST_H_

#include <string>
#include <vector>
#include <memory>

#include "abstractWidget.h"
#include "toggleButton.h"


#include "../../graphics/sprite.h"

namespace widget {

class SelectableList : public AbstractWidget {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	SelectableList(int id, int x, int y, int width, int height,
			std::function<void(int)> observer, int layer = 0);

	virtual ~SelectableList() {};

// ########################################################
// Member Functions########################################
// ########################################################

	void addListItem(std::string item);

	void deleteListItem(std::string item);

	std::string getSelectedListItem();

	void clear();

	void reset();

	void draw() override;

	void update() override;

//	void mouseClicked(int button, float x, float y) override;
//
//	void mouseMoved(float x, float y) override;
//
//	void keyTyped(char value) override;

// ########################################################
// Instance Variables #####################################
// ########################################################

private:

	std::shared_ptr<ToggleButton> getButtonWithId(int i);

	std::function<void(int)> m_observer;
	int m_layer;

	int idCounter{0};
	std::vector<std::shared_ptr<ToggleButton>> m_buttons;

	std::shared_ptr<ToggleButton> m_currentlyToggled{nullptr};

	std::shared_ptr<graphics::Sprite> m_sprite;
	std::shared_ptr<graphics::Sprite> m_highlight;
	std::shared_ptr<graphics::Sprite> m_text;

};

} /* namespace widget */

#endif /* SRC_GUI_WIDGET_SELECTABLELIST_H_ */
