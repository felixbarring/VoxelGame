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

  Label(int x, int y, int width, int height, const std::string &name,
      unsigned layer = 0);

  virtual ~Label() = default;

  void draw() override;

private:

  std::shared_ptr<graphics::Sprite> m_text;

};

} /* namespace widget */

#endif /* SRC_GUI_WIDGET_LABEL_H_ */
