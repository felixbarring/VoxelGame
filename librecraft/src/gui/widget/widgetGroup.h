/**
 *  \file [widgetGroup.h]
 *  \author Felix Bärring
 *
 *  Copyright (c) 2015, Felix Bärring. All rights reserved.
 */

#ifndef SRC_GUI_WIDGET_WIDGETGROUP_H_
#define SRC_GUI_WIDGET_WIDGETGROUP_H_

#include <vector>
#include <memory>

#include "abstractWidget.h"
#include "iWidget.h"

#include "../../graphics/sprite.h"

namespace widget {

/**
 * \class WidgetGroup
 * \brief Handles a group of widgets
 *
 * A simple layer that redirects all function calls to its widgets.
 * Useful for handling groups of widgets that work together.
 *
 * \author Felix Bärring
 */
class WidgetGroup: public AbstractWidget {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	/**
	 * \brief Constructs a WidgetGroup
	 */
	WidgetGroup(int id, int x, int y, int width, int height, std::function<void(int)> observer, int layer = 0);

	virtual ~WidgetGroup() {};

// ########################################################
// Member Functions########################################
// ########################################################

	/**
	 * \brief Adds a widget to this group
	 *
	 * All other functinos in this class will act on the Widgets
	 * added by this function.
	 *
	 * \param[in] widget The widget that will be added to this group
	 */
	void addWidget(std::shared_ptr<IWidget> widget);

	void draw() override;

	void update(float timePassed) override;

// ########################################################
// Implementation #########################################
// ########################################################

private:

	std::shared_ptr<graphics::Sprite> m_sprite;
	std::vector<std::shared_ptr<IWidget>> m_widgets;

};

} /* namespace widget */

#endif /* SRC_GUI_WIDGET_WIDGETGROUP_H_ */
