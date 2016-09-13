#ifndef SRC_CONFIG_DATA_H_
#define SRC_CONFIG_DATA_H_

#include <GL/glew.h>

#include <vector>
#include <string>

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

namespace config {

	static std::string dataFolder = "data/";
	static std::string worldMetaFile = "data/meta";

	namespace chunk_data {
		static constexpr int GRAPHICAL_CHUNK_WIDTH = 16;
		static constexpr int GRAPHICAL_CHUNK_HEIGHT = 64;

		static constexpr int CHUNK_WIDTH_AND_DEPTH = 16;
		static constexpr int CHUNK_HEIGHT = 128;

		static constexpr const int NUMBER_OF_CHUNKS_FROM_MIDDLE_TO_BORDER = 8;
		static constexpr const int NUMBER_OF_CHUNKS_Y = 1;
	}

	namespace cube_data {
		static constexpr char AIR = 255;

		static constexpr char GRASS = 0;
		static constexpr char DIRT = 1;
		static constexpr char ROCK = 2;
		static constexpr char MORE_ROCK = 3;
		static constexpr char BRICK_WALL = 4;
		static constexpr char SAND = 5;
		static constexpr char WATER = 6;

		static constexpr char LAST_CUBE_USED_FOR_BUILDING_ = SAND; // Better name needed

		static constexpr char BED_ROCK = 7;
		static constexpr char SELECTED = 8;

		// Used for looping over all types, except for air.
		static constexpr char LAST_CUBE = SELECTED;

		static constexpr int SIDE_TEXTURE = 0;
		static constexpr int TOP_TEXTURE = 1;
		static constexpr int BOTTOM_TEXTURE = 2;

		// 0 = Side, 1 = Top, 2 = Bottom
		static constexpr char BLOCK_TEXTURES[9][3] {
			{ 0, 1, 2 }, // Grass
			{ 2, 2, 2 }, // Dirt
			{ 3, 3, 3 }, // Rock
			{ 4, 4, 4 }, // More rock
			{ 5, 5, 5 }, // Brick wall
			{ 6, 6, 6 }, // Sand
			{ 7, 7, 7 }, // Water
			{ 8, 8, 8 }, // Bedrock
			{ 9, 9, 9 }, // Selected
		};

		static constexpr int TEXTURE_WIDTH = 32;
		static constexpr int TEXTURE_HEIGHT = 32;

		static const std::vector<std::string> textures {
			"../resources/cubes/grass_side.png",
			"../resources/cubes/grass_top.png",
			"../resources/cubes/dirt.png",
			"../resources/cubes/rock.png",
			"../resources/cubes/more_rock.png",
			"../resources/cubes/brick.png",
			"../resources/cubes/sand.png",

			"../resources/cubes/water.png",
			"../resources/cubes/bedrock.png",
			"../resources/cubes/selected.png",
		};

		// TODO Fix better thumbnails
		static const std::vector<std::string> thumbnails {
			"../resources/cubes/grass_side.png",
			"../resources/cubes/dirt.png",
			"../resources/cubes/rock.png",
			"../resources/cubes/more_rock.png",
			"../resources/cubes/brick.png",
			"../resources/cubes/sand.png",
		};

	}

	namespace graphics_data {
		extern int fps;

		extern int windowWidth;
		extern int windowHeight;

		extern float fov; // degree to radians

		static float virtualWidth{800};
		static float virtualHeight{600};
	}

	namespace input_data {
		// Keyboard #####################################################

		static sf::Keyboard::Key moveForwardButton = sf::Keyboard::W;
		static sf::Keyboard::Key moveLeftButton = sf::Keyboard::A;
		static sf::Keyboard::Key moveRightButton = sf::Keyboard::D;
		static sf::Keyboard::Key moveBackwardButton = sf::Keyboard::S;

		static sf::Keyboard::Key jumpButton = sf::Keyboard::Space;
		static sf::Keyboard::Key goDownButton = sf::Keyboard::LShift;

		static sf::Keyboard::Key eraseTextButton = sf::Keyboard::BackSpace;

		static sf::Keyboard::Key switchBuildingCube = sf::Keyboard::E;

		static sf::Keyboard::Key openTerminal = sf::Keyboard::T;
		static sf::Keyboard::Key tab = sf::Keyboard::Tab;
		static sf::Keyboard::Key enter = sf::Keyboard::Return;

		// Mouse ########################################################
		static sf::Mouse::Button action1Button = sf::Mouse::Left;
		static sf::Mouse::Button action2Button = sf::Mouse::Right;

		extern float mouseSensitivityX;
		extern float mouseSensitivityY;

	}

	namespace texture_paths {
		// Remove THESE ?
		static std::string scout = "../resources/cubes/grass_side.png";
		static std::string scout2 = "../resources/cubes/grass_top.png";
	}

	namespace gui_data {
		static const std::string title = "../resources/gui/title.png";

		static const std::string solidBlack = "../resources/gui/solid_black.png";
		static const std::string solidWhite = "../resources/gui/solid_white.png";

		static const std::string guiBox = "../resources/gui/gui_box.png";
		static const std::string button = "../resources/gui/button.png";
		static const std::string highlight = "../resources/gui/highlight.png";

		static const std::string slider = "../resources/gui/slider_body.png";
		static const std::string sliderKnob = "../resources/gui/slider_knob.png";

		static const std::string crossHair = "../resources/gui/crosshair.png";

		static const std::string mouse = "../resources/gui/mouse.png";
	}

	namespace font_data {
		static constexpr int charsInFont = 256;
		static constexpr int fontAtlasWidth = 1024;
		static constexpr int fontAtlasHeight = 1024;

		static std::string font = "../resources/font/font1.png";
		static std::string fontLayout = "../resources/font/font1.txt";
	}

	namespace cube_map_data {
		static std::vector<std::string> cubeMap1 {
			"../resources/skybox/right.jpg",
			"../resources/skybox/left.jpg",
			"../resources/skybox/top.jpg",
			"../resources/skybox/bottom.jpg",
			"../resources/skybox/back.jpg",
			"../resources/skybox/front.jpg"
		};

		static constexpr int cubeMap1Width = 2048;
		static constexpr int cubeMap1Height = 2048;
	}

	namespace souds {
		static const std::string buttonPressed = "../resources/sound/button.wav";
		static const std::string cubeAdded = "../resources/sound/cube_place.wav";
		static const std::string cubeRemoved = 	"../resources/sound/cube_remove.wav";
	}

	namespace music {
		static const std::string menuMusic = "../resources/music/Continue Life.ogg";
	}

}

#endif /* SRC_CONFIG_DATA_H_ */
