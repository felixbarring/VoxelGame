
#ifndef SRC_DEMO_GUIDEMO_H_
#define SRC_DEMO_GUIDEMO_H_

#include <iostream>
#include <memory>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "graphics/fontMeshBuilder.h"
#include "graphics/spriteBatcher.h"
#include "graphics/graphicsManager.h"

#include "util/fpsManager.h"
#include "util/input.h"

#include "gui/guiUtil.h"
#include "gui/mouse.h"
#include "gui/widget/button.h"
#include "gui/widget/iWidget.h"
#include "gui/widget/label.h"
#include "gui/widget/selectableList.h"
#include "gui/widget/slider.h"
#include "gui/widget/textInput.h"
#include "gui/widget/toggleButton.h"
#include "gui/widget/widgetGroup.h"

#include "config/data.h"

#include <SFML/Window.hpp>

using namespace std;
using namespace widget;
using namespace sf;

namespace demo {

class GuiDemo {
public:
  void runDemo() {
    util::FPSManager fpsManager(60);
    const GLuint width{1200};
    const GLuint height{600};

    config::graphics_data::windowWidth = width;
    config::graphics_data::windowHeight = height;

    // create the window
    ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 4;
    settings.majorVersion = 3;
    settings.minorVersion = 1;

    Window window(
      VideoMode(width, height), "Voxel Game", Style::Default, settings);

    glewExperimental = true;
    if (glewInit() != GLEW_OK)
      cout << "Failed to initialize GLEW\n";

    glViewport(0, 0, width, height);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    unique_ptr<TextInput> textInput;
    unique_ptr<Slider> slider;

    WidgetGroup* currentWidgetGroup;
    unique_ptr<WidgetGroup> mainWidgetGroup;
    unique_ptr<WidgetGroup> playWidgetGroup;
    unique_ptr<WidgetGroup> settingsWidgetGroup;
    unique_ptr<WidgetGroup> listWidgetGroup;

    bool quit = false;
    function<void(int)> observer = [&](int id) {
      switch (id) {
        case 0: {
          currentWidgetGroup = playWidgetGroup.get();
          break;
        }
        case 1: {
          currentWidgetGroup = settingsWidgetGroup.get();
          break;
        }
        case 2: {
          quit = true;
          break;
        }
        case 3: {
          currentWidgetGroup = listWidgetGroup.get();
          break;
        }
        case 5: {
          currentWidgetGroup = mainWidgetGroup.get();
          break;
        }
        case 6: {
          currentWidgetGroup = mainWidgetGroup.get();
          break;
        }
        case 666: {
          textInput->setString(to_string(slider->getValue()));
          break;
        }
      }
      cout << "A button with id: " << id << " was pressed\n";
    };

    graphics::GraphicsManager graphicsmanager{};

    // ########################################################################

    shared_ptr<IWidget> button1 = make_unique<Button>(
      0, 325, 350, 150, 30, graphicsmanager, observer, "Play");
    shared_ptr<IWidget> button2 = make_unique<Button>(
      1, 325, 310, 150, 30, graphicsmanager, observer, "Settings");
    shared_ptr<IWidget> button3 = make_unique<Button>(
      2, 325, 270, 150, 30, graphicsmanager, observer, "Quit");
    shared_ptr<IWidget> button8 = make_unique<Button>(
      3, 325, 230, 150, 30, graphicsmanager, observer, "List");
    shared_ptr<IWidget> toggleButton = make_unique<ToggleButton>(
      666, 325, 190, 150, 30, graphicsmanager, observer, "Toggle");

    mainWidgetGroup =
      make_unique<WidgetGroup>(0, 0, 0, 800, 600, graphicsmanager);

    mainWidgetGroup->addWidget(*button1);
    mainWidgetGroup->addWidget(*button2);
    mainWidgetGroup->addWidget(*button3);
    mainWidgetGroup->addWidget(*button8);
    mainWidgetGroup->addWidget(*toggleButton);

    // ########################################################################

    unique_ptr<IWidget> label1 =
      make_unique<Label>(325, 390, 150, 30, " - Play - ", graphicsmanager);
    unique_ptr<IWidget> button4 = make_unique<Button>(
      666, 325, 350, 150, 30, graphicsmanager, observer, "New World");
    unique_ptr<IWidget> button5 = make_unique<Button>(
      666, 325, 310, 150, 30, graphicsmanager, observer, "Load World");
    unique_ptr<IWidget> button6 = make_unique<Button>(
      5, 325, 270, 150, 30, graphicsmanager, observer, "Back");

    playWidgetGroup =
      make_unique<WidgetGroup>(0, 0, 0, 800, 600, graphicsmanager);

    playWidgetGroup->addWidget(*label1);
    playWidgetGroup->addWidget(*button4);
    playWidgetGroup->addWidget(*button5);
    playWidgetGroup->addWidget(*button6);

    // ########################################################################

    unique_ptr<IWidget> label2 =
      make_unique<Label>(325, 390, 150, 30, " - Settings - ", graphicsmanager);
    textInput = make_unique<TextInput>(666, 325, 350, 150, 30, graphicsmanager);
    slider =
      make_unique<Slider>(666, 325, 310, 150, 30, graphicsmanager, observer);
    unique_ptr<IWidget> button7 = make_unique<Button>(
      6, 325, 270, 150, 30, graphicsmanager, observer, "Back");

    settingsWidgetGroup =
      make_unique<WidgetGroup>(0, 0, 0, 800, 600, graphicsmanager);

    settingsWidgetGroup->addWidget(*label2);
    settingsWidgetGroup->addWidget(*slider);
    settingsWidgetGroup->addWidget(*textInput);
    settingsWidgetGroup->addWidget(*button7);

    // ########################################################################

    unique_ptr<Button> button1337 = make_unique<Button>(
      666, 325, 270, 150, 30, graphicsmanager, observer, "DERP");

    unique_ptr<SelectableList> derp = make_unique<SelectableList>(
      666, 100, 100, 600, 400, graphicsmanager, observer, 2);
    derp->addListItem("BLOOL");
    derp->addListItem("Jesus");
    derp->addListItem("Satan");
    derp->addListItem("Tor");
    derp->addListItem("BLOOL");
    derp->addListItem("Jesus");
    derp->addListItem("Satan");
    derp->addListItem("Tor");

    unique_ptr<IWidget> button9 = make_unique<Button>(
      6, 325, 0, 150, 30, graphicsmanager, observer, "Back");

    listWidgetGroup =
      make_unique<WidgetGroup>(0, 0, 0, 800, 600, graphicsmanager);
    listWidgetGroup->addWidget(*derp);
    listWidgetGroup->addWidget(*button9);

    // ########################################################################

    currentWidgetGroup = mainWidgetGroup.get();

    util::Input::createInstance(width / 2.0, height / 2.0);
    util::Input::getInstance()->setWindow(&window);

    shared_ptr<util::Input> input = util::Input::getInstance();
    input->unlockMouse();

    glm::mat4 matrix = gui::createVirtualToScreen(800, 600, 1200, 600);
    glm::mat4 matrix2 =
      glm::ortho(0.0f, 1200.0f, 0.0f, 600.0f, -1.0f, 1.0f) * matrix;
    graphicsmanager.getSpriteBatcher().setProjection(matrix2);

    gui::Mouse mouse{graphicsmanager};

    while (!quit && window.isOpen()) {

      fpsManager.frameStart();
      glClear(GL_COLOR_BUFFER_BIT);

      input->updateValues();

      mouse.update();
      mouse.draw();

      if (input->escapeKeyPressed)
        window.close();

      currentWidgetGroup->update(0.01);
      currentWidgetGroup->draw();

      graphicsmanager.getSpriteBatcher().draw();

      fpsManager.sync();
      window.display();

      sf::Event event;
      while (window.pollEvent(event))
        if (event.type == sf::Event::Closed)
          window.close();
    }
  }
};

} /* namespace demo */

#endif /* SRC_DEMO_GUIDEMO_H_ */
