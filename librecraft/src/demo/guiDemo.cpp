
#include "guiDemo.h"

#include <iostream>
#include <memory>

#include <GL/glew.h>

#include <glm/glm.hpp>

#include "../graphics/spriteBatcher.h"
#include "../graphics/fontMeshBuilder.h"

#include "../util/fpsManager.h"
#include "../util/input.h"

#include "../gui/widget/selectableList.h"
#include "../gui/widget/button.h"
#include "../gui/widget/toggleButton.h"
#include "../gui/widget/slider.h"
#include "../gui/widget/label.h"
#include "../gui/widget/textInput.h"
#include "../gui/widget/widgetGroup.h"
#include "../gui/widget/iWidget.h"
#include "../gui/guiUtil.h"
#include "../gui/mouse.h"

#include "../config/data.h"

#include <SFML/Window.hpp>

using namespace std;
using namespace widget;
using namespace sf;

namespace demo {

// ########################################################
// Constructor/Destructor #################################
// ########################################################

// ########################################################
// Member Functions########################################
// ########################################################

void GuiDemo::runDemo()
{
	util::FPSManager fpsManager(60);
	const GLuint WIDTH = 1200, HEIGHT = 600;

	config::graphics_data::windowWidth = WIDTH;
	config::graphics_data::windowHeight = HEIGHT;


	// create the window
	ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 4;
	settings.majorVersion = 3;
	settings.minorVersion = 1;

	Window window(VideoMode(WIDTH, HEIGHT), "Voxel Game", Style::Default,
			settings);

	glewExperimental = true;
	if (glewInit() != GLEW_OK)
		cout << "Failed to initialize GLEW\n";

	glViewport(0, 0, WIDTH, HEIGHT);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	shared_ptr<TextInput> textInput;
	shared_ptr<Slider> slider;

	shared_ptr<WidgetGroup> currentWidgetGroup;
	shared_ptr<WidgetGroup> mainWidgetGroup;
	shared_ptr<WidgetGroup> playWidgetGroup;
	shared_ptr<WidgetGroup> settingsWidgetGroup;
	shared_ptr<WidgetGroup> listWidgetGroup;

	bool quit = false;
	function<void(int)> observer = [&](int id)
	{
		switch(id) {
			case 0: {
				currentWidgetGroup = playWidgetGroup;
				break;
			}
			case 1: {
				currentWidgetGroup = settingsWidgetGroup;
				break;
			}
			case 2: {
				quit = true;
				break;
			}
			case 3: {
				currentWidgetGroup = listWidgetGroup;
				break;
			}
			case 5: {
				currentWidgetGroup = mainWidgetGroup;
				break;
			}
			case 6: {
				currentWidgetGroup = mainWidgetGroup;
				break;
			}
			case 666: {
				textInput->setString(to_string(slider->getValue()));
			}
		}
		cout << "A button with id: " << id << " was pressed\n";
	};

	// ########################################################################

	shared_ptr<IWidget> button1(new Button{0, 325, 350, 150, 30, observer, "Play"});
	shared_ptr<IWidget> button2(new Button{1, 325, 310, 150, 30, observer, "Settings"});
	shared_ptr<IWidget> button3(new Button{2, 325, 270, 150, 30, observer, "Quit"});
	shared_ptr<IWidget> button8(new Button{3, 325, 230, 150, 30, observer, "List"});
	shared_ptr<IWidget> toggleButton(new ToggleButton{666, 325, 190, 150, 30, observer, "Toggle"});

	mainWidgetGroup.reset(new WidgetGroup{0, 0, 0, 800, 600, observer});

	mainWidgetGroup->addWidget(button1);
	mainWidgetGroup->addWidget(button2);
	mainWidgetGroup->addWidget(button3);
	mainWidgetGroup->addWidget(button8);
	mainWidgetGroup->addWidget(toggleButton);

	// ########################################################################

	shared_ptr<IWidget> label1(new Label{325, 390, 150, 30, " - Play - "});
	shared_ptr<IWidget> button4(new Button{666, 325, 350, 150, 30, observer, "New World"});
	shared_ptr<IWidget> button5(new Button{666, 325, 310, 150, 30, observer, "Load World"});
	shared_ptr<IWidget> button6(new Button{5, 325, 270, 150, 30, observer, "Back"});

	playWidgetGroup.reset(new WidgetGroup{0, 0, 0, 800, 600, observer});

	playWidgetGroup->addWidget(label1);
	playWidgetGroup->addWidget(button4);
	playWidgetGroup->addWidget(button5);
	playWidgetGroup->addWidget(button6);

	// ########################################################################

	shared_ptr<IWidget> label2(new Label{325, 390, 150, 30, " - Settings - "});
	textInput.reset(new TextInput{666, 325, 350, 150, 30});
	slider.reset(new Slider{666, 325, 310, 150, 30, observer});
	shared_ptr<IWidget> button7(new Button{6, 325, 270, 150, 30, observer, "Back"});

	settingsWidgetGroup.reset(new WidgetGroup{0, 0, 0, 800, 600, observer});

	settingsWidgetGroup->addWidget(label2);
	settingsWidgetGroup->addWidget(slider);
	settingsWidgetGroup->addWidget(textInput);
	settingsWidgetGroup->addWidget(button7);

	// ########################################################################

	shared_ptr<Button> button1337(new Button(666, 325, 270, 150, 30, observer, "DERP"));

	shared_ptr<SelectableList> derp(new SelectableList{666, 100, 100, 600, 400, observer, 2});
	derp->addListItem("BLOOL");
	derp->addListItem("Jesus");
	derp->addListItem("Satan");
	derp->addListItem("Tor");
	derp->addListItem("BLOOL");
	derp->addListItem("Jesus");
	derp->addListItem("Satan");
	derp->addListItem("Tor");

	shared_ptr<IWidget> button9(new Button{6, 325, 0, 150, 30, observer, "Back"});

	listWidgetGroup.reset(new WidgetGroup{0, 0, 0, 800, 600, observer});
	listWidgetGroup->addWidget(derp);
	listWidgetGroup->addWidget(button9);

	// ########################################################################

	currentWidgetGroup = mainWidgetGroup;

	util::Input::createInstance(WIDTH / 2.0, HEIGHT / 2.0);
	util::Input::getInstance()->setWindow(&window);

	shared_ptr<util::Input> input = util::Input::getInstance();
	input->unlockMouse();

	glm::mat4 matrix = gui::createVirtualToScreen(800, 600, 1200, 600);
	glm:: mat4 matrix2 = glm::ortho(0.0f, 1200.0f, 0.0f, 600.0f, -1.0f, 1.0f) * matrix;
	graphics::SpriteBatcher::getInstance().setProjection(matrix2);

	while (!quit && window.isOpen()) {

		fpsManager.frameStart();
		glClear(GL_COLOR_BUFFER_BIT);

		input->updateValues();

		widget::Mouse::getInstance().update();
		widget::Mouse::getInstance().draw();

		if (input->escapeKeyPressed)
			window.close();

		currentWidgetGroup->update(0.01);
		currentWidgetGroup->draw();

		graphics::SpriteBatcher::getInstance().draw();

		fpsManager.sync();
		window.display();

        sf::Event event;
		while (window.pollEvent(event))
			if (event.type == sf::Event::Closed)
				window.close();

	}

}

} /* namespace graphics */
