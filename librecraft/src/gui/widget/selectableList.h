#ifndef SRC_GUI_WIDGET_SELECTABLELIST_H_
#define SRC_GUI_WIDGET_SELECTABLELIST_H_

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "graphics/graphicsManager.h"
#include "graphics/sprite.h"
#include "abstractWidget.h"
#include "toggleButton.h"

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
  ToggleButton* getButtonWithId(int i);

  unsigned m_layer;
  std::function<void(int)> m_observer;

  int idCounter{0};
  std::vector<ToggleButton> m_buttons;

  ToggleButton* m_currentlyToggled{nullptr};
  graphics::Sprite m_sprite;
};

} /* namespace widget */

#endif /* SRC_GUI_WIDGET_SELECTABLELIST_H_ */
