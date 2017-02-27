#ifndef SRC_GUI_WIDGET_SELECTABLELIST_H_
#define SRC_GUI_WIDGET_SELECTABLELIST_H_

#include <string>
#include <vector>
#include <memory>

#include "abstractWidget.h"
#include "toggleButton.h"

#include "../../graphics/sprite.h"

namespace widget {

class SelectableList: public AbstractWidget {
public:

	SelectableList(int id, int x, int y, int width, int height, std::function<void(int)> observer, unsigned layer = 0);

	virtual ~SelectableList() {};

	void addListItem(std::string item);

	void deleteListItem(std::string item);

	std::string getSelectedListItem();

	void clear();

	void reset();

	void draw() override;

	void update(float timePassed) override;

private:

	std::shared_ptr<ToggleButton> getButtonWithId(int i);

	std::function<void(int)> m_observer;
	unsigned m_layer;

	int idCounter {0};
	std::vector<std::shared_ptr<ToggleButton>> m_buttons;

	std::shared_ptr<ToggleButton> m_currentlyToggled {nullptr};

	std::shared_ptr<graphics::Sprite> m_sprite;
	std::shared_ptr<graphics::Sprite> m_highlight;
	std::shared_ptr<graphics::Sprite> m_text;

};

} /* namespace widget */

#endif /* SRC_GUI_WIDGET_SELECTABLELIST_H_ */
