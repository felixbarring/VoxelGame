
#ifndef SRC_CONFIG_DATA_H_
#define SRC_CONFIG_DATA_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <string>

namespace config
{

	namespace chunk_data
	{

		static constexpr int GRAPHICAL_CHUNK_WIDTH = 16;
		static constexpr int GRAPHICAL_CHUNK_HEIGHT = 128;
		static constexpr int GRAPHICAL_CHUNK_DEPTH = 16;

		static constexpr int CHUNK_WIDTH = 16;
		static constexpr int CHUNK_HEIGHT = 128;
		static constexpr int CHUNK_DEPTH = 16;

		static constexpr const int NUMBER_OF_CHUNKS_X = 10;
		static constexpr const int NUMBER_OF_CHUNKS_Y = 1;
		static constexpr const int NUMBER_OF_CHUNKS_Z = 10;

	}

	namespace cube_data
	{

		static constexpr char AIR = 255;

		static constexpr char GRASS = 0;
		static constexpr char DIRT = 1;
		static constexpr char ROCK = 2;
		static constexpr char BRICK_WALL = 3;

		static constexpr char LAST_CUBE = BRICK_WALL; // Better name needed

		static constexpr char BED_ROCK = 4;

		static constexpr char SELECTED = 5;

		// Used for looping over all types, except for air.
		static constexpr char LAST_BLOCK = SELECTED;

		static constexpr int SIDE_TEXTURE = 0;
		static constexpr int TOP_TEXTURE =1;
		static constexpr int BOTTOM_TEXTURE = 2;

		// 0 = Side, 1 = Top, 2 = Bottom
		static constexpr char BLOCK_TEXTURES[6][3]{
			{0, 1, 2},
			{2, 2, 2},
			{3, 3, 3},
			{4, 4, 4},
			{5, 5, 5},
			{6, 6, 6}
		};

		static constexpr int TEXTURE_WIDTH = 32;
		static constexpr int TEXTURE_HEIGHT = 32;

		static const std::vector<std::string> textures {
			"../resources/cubes/grass_side.png",
			"../resources/cubes/grass_top.png",
			"../resources/cubes/dirt.png",
			"../resources/cubes/rock.png",
			"../resources/cubes/brick.png",
			"../resources/cubes/bedrock.png",
			"../resources/cubes/selected.png",
		};

	}

	namespace graphics_data
	{

		extern int fps;

		extern int windowWidth;
		extern int windowHeight;

		extern float fov; // degree to radians

		static float virtualWidth = 800;
		static float virtualHeight = 600;

	}

	namespace input_data
	{

		// Keyboard #####################################################

		static int moveForwardButton = GLFW_KEY_W;
		static int moveLeftButton = GLFW_KEY_A;
		static int moveRightButton = GLFW_KEY_D;
		static int moveBackwardButton = GLFW_KEY_S;

		static int jumpButton = GLFW_KEY_SPACE;
		static int goDownButton = GLFW_KEY_LEFT_SHIFT;

		static int eraseTextButton = GLFW_KEY_BACKSPACE;

		// Mouse ########################################################

		static int action1Button = GLFW_MOUSE_BUTTON_LEFT;
		static int action2Button = GLFW_MOUSE_BUTTON_RIGHT;

	}

	namespace texture_paths
	{
		// Remove THESE ?
		static std::string scout =  "../resources/cubes/grass_side.png";
		static std::string scout2 = "../resources/cubes/grass_top.png";

	}

	namespace gui_data
	{
		static const std::string title = "../resources/gui/title.png";

		static const std::string guiBox = "../resources/gui/gui_box.png";
		static const std::string button = "../resources/gui/button.png";
		static const std::string highlight = "../resources/gui/highlight.png";

		static const std::string slider = "../resources/gui/slider_body.png";
		static const std::string sliderKnob = "../resources/gui/slider_knob.png";

	}

	namespace font_data
	{

		static constexpr int charsInFont = 256;
		static constexpr int fontAtlasWidth = 1024;
		static constexpr int fontAtlasHeight = 1024;

		static std::string font = "../resources/font/font1.png";
		static std::string fontLayout = "../resources/font/font1.txt";

	}

	namespace cube_map_data
	{

		static std::vector<std::string> cubeMap1{
			"../resources/skybox/right.jpg",
			"../resources/skybox/left.jpg",
			"../resources/skybox/top.jpg",
			"../resources/skybox/bottom.jpg",
			"../resources/skybox/back.jpg",
			"../resources/skybox/front.jpg"};

		static constexpr int cubeMap1Width = 2048;
		static constexpr int cubeMap1Height = 2048;

	}

}

#endif /* SRC_CONFIG_DATA_H_ */
