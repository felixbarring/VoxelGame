/**
 *  \file [iWidget.h]
 *  \brief An Interface for Widgets
 *  \author Felix Bärring
 *
 *  Copyright (c) 2015, Felix Bärring. All rights reserved.
 */

#ifndef SRC_GUI_WIDGET_IWIDGET_H_
#define SRC_GUI_WIDGET_IWIDGET_H_

namespace widget {

/**
 *  \class IWidget
 *
 * A widget is a visual component of a Graphical user interface
 * (abbreviated GUI). Its purpose is to be interactable and provide
 * means to perform some task. This interface defines all the basic
 * functionality that all widgets must implement.
 *
 * All Widgets should have a mode where they ignore methods calls if
 * they are set into an inactive state. This state can be controlled
 * with setActive and deactivate.
 *
 *  \author Felix Bärring
 */
class IWidget {
public:
  virtual ~IWidget(){};

  /**
   *  \brief Makes the Widget draw itself
   *
   *  To actually render the sprite batcher's draw function
   *  should be called.
   *
   */
  virtual void draw() = 0;

  /**
   *  \brief Returns the button's ID
   */
  virtual int getId() = 0;

  /**
   * \brief If the Widget needs to be updated each frame, it should implement
   * 	this method
   */
  virtual void update(float timePassed) = 0;

  /**
   * \brief Set the absolute position of the widget
   *
   * \param[in] x The new horizontal location
   * \param[in] y The new vertical location
   * \param[in] z The 'layer', place holder for now
   */
  virtual void setLocation(float x, float y) = 0;

  /**
   * Sets the Widget to an active state.
   * If it is not in an active state all other methods will be ignored.
   */
  virtual void setActive() = 0;

  /**
   * Sets the widget to an inactive state.
   * All other methods will be ignored except for setActive.
   */
  virtual void setInactive() = 0;

  /**
   * Checks if the widget is active or not
   *
   * \return Whether the Widget is active or not
   */
  virtual bool isActive() = 0;
};
}

#endif /* SRC_GUI_WIDGET_IWIDGET_H_ */
