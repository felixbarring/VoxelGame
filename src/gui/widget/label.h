
/**
 *  \file [label.h]
 *  \brief A label displays text in GUIs
 *  \author Felix Bärring
 *
 *  Copyright (c) 2015, Felix Bärring. All rights reserved.
 */

#ifndef SRC_GUI_WIDGET_LABEL_H_
#define SRC_GUI_WIDGET_LABEL_H_

#include <string>

#include "abstractWidget.h"

#include "../../graphics/sprite.h"

namespace widget {

/**
 * \class Label
 *
 * A label can be used to provide information in GUIs by displaying text
 *
 */
class Label : public AbstractWidget {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################


	Label(int x, int y, int width, int height, const std::string &name);

	virtual ~Label() {};

// ########################################################
// Member Functions########################################
// ########################################################

	void draw() override;

	void mouseClicked(int button, float x, float y) override;

	void mouseMoved(float x, float y) override;

	void keyPressed(int key, char c) override;

// ########################################################
// Instance Variables #####################################
// ########################################################

	std::shared_ptr<graphics::Sprite> text;

};

} /* namespace widget */

#endif /* SRC_GUI_WIDGET_LABEL_H_ */
