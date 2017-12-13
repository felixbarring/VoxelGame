#ifndef SRC_GUI_WIDGET_SELECTABLELIST_H_
#define SRC_GUI_WIDGET_SELECTABLELIST_H_

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "../../graphics/graphicsManager.h"
#include "abstractWidget.h"
#include "toggleButton.h"

#include "../../graphics/sprite.h"

namespace widget {

/**
 * @brief
 */
class SelectableList : public AbstractWidget {
public:
  /**
   * @brief Constructs a selectable list.
   *
   * @param id Id of the
   * @param x
   * @param y
   * @param width
   * @param height
   * @param observer
   * @param layer
   */
  SelectableList(int id,
                 int x,
                 int y,
                 int width,
                 int height,
                 graphics::GraphicsManager& graphicsManager,
                 std::function<void(int)> observer,
                 unsigned layer = 0);

  virtual ~SelectableList() = default;

  /**
   * @brief Adds a new item to the list.
   *
   * If addListItem is called with a string value that has already been added,
   * then it will silently ignore and not add a new item.
   *
   * @param item The new item will have the value as a visible string.
   */
  void addListItem(std::string value);

  /**
   * @brief Removes the item that has the string value as a visible string.
   *
   * @param item
   */
  void deleteListItem(std::string value);

  /**
   * @brief Returns the string of the currently selected item.
   *
   * @return If no item is selected, an empty string will be returned.
   *         Otherwise the string value of the selected list item will be
   *         returned.
   */
  std::string getSelectedListItem();

  /**
   * @brief Removes all items.
   */
  void clear();

  /**
   * @brief Resets the list so that no item is toggled.
   */
  void reset();

  void draw() override;

  void update(float timePassed) override;

private:
  std::shared_ptr<ToggleButton> getButtonWithId(int i);

  std::function<void(int)> m_observer;
  unsigned m_layer;

  int idCounter{ 0 };
  std::vector<std::shared_ptr<ToggleButton>> m_buttons;

  std::shared_ptr<ToggleButton> m_currentlyToggled{ nullptr };

  std::shared_ptr<graphics::Sprite> m_sprite;
  std::shared_ptr<graphics::Sprite> m_highlight;
  std::shared_ptr<graphics::Sprite> m_text;
};

} /* namespace widget */

#endif /* SRC_GUI_WIDGET_SELECTABLELIST_H_ */
