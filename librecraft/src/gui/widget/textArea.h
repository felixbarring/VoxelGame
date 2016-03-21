#ifndef SRC_GUI_WIDGET_TEXTAREA_H_
#define SRC_GUI_WIDGET_TEXTAREA_H_

#include <memory>
#include <string>
#include <vector>

#include "../../graphics/sprite.h"
#include "abstractWidget.h"

namespace widget {

class TextArea: public AbstractWidget {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	TextArea(int id, int x, int y, int width, int height);

	virtual ~TextArea() { };

// ########################################################
// Member Functions########################################
// ########################################################

	void draw() override;

	void update(float timePassed) override;

	void add(std::string str);

// ########################################################
// Implementation #########################################
// ########################################################

private:

	std::vector<std::string> m_rows;
	std::vector<std::shared_ptr<graphics::Sprite>> m_sprites;

};

} /* namespace widget */

#endif /* SRC_GUI_WIDGET_TEXTAREA_H_ */
