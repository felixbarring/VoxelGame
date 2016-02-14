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
class Label: public AbstractWidget {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	Label(int x, int y, int width, int height, const std::string &name,
			int layer = 0);

	virtual ~Label() {
	}
	;

// ########################################################
// Member Functions########################################
// ########################################################

	void draw() override;

// ########################################################
// Implementation #########################################
// ########################################################

private:

	std::shared_ptr<graphics::Sprite> m_text;

};

} /* namespace widget */

#endif /* SRC_GUI_WIDGET_LABEL_H_ */