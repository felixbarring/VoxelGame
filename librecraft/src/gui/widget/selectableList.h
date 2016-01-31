
#ifndef SRC_GUI_WIDGET_SELECTABLELIST_H_
#define SRC_GUI_WIDGET_SELECTABLELIST_H_

#include <string>
#include <vector>
#include <memory>

#include "abstractWidget.h"
#include "button.h"

#include "../../graphics/sprite.h"

namespace widget {

class SelectableList : public AbstractWidget {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	SelectableList(int id, int x, int y, int width, int height,
			std::function<void(int)> observer);

	virtual ~SelectableList() {};

// ########################################################
// Member Functions########################################
// ########################################################

	void addListItem(std::string item);

	void deleteListItem(std::string item);

	void draw() override;

	void mouseClicked(int button, float x, float y) override;

	void mouseMoved(float x, float y) override;

	void keyTyped(char value) override;

// ########################################################
// Instance Variables #####################################
// ########################################################

private:

	std::function<void(int)> observer;

	std::vector<Button> m_buttons;

	std::shared_ptr<graphics::Sprite> m_sprite;
	std::shared_ptr<graphics::Sprite> m_highlight;
	std::shared_ptr<graphics::Sprite> m_text;

};

} /* namespace widget */

#endif /* SRC_GUI_WIDGET_SELECTABLELIST_H_ */
