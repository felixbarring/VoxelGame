
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
		static constexpr int GRAPHICAL_CHUNK_HEIGHT = 16;
		static constexpr int GRAPHICAL_CHUNK_DEPTH = 16;

		static constexpr int CHUNK_WIDHT = 16;
		static constexpr int CHUNK_HEIGHT = 16;
		static constexpr int CHUNK_DEPTH = 16;

		static constexpr int NUMBER_OF_CHUNKS_X = 5;
		static constexpr int NUMBER_OF_CHUNKS_Y = 1;
		static constexpr int NUMBER_OF_CHUNKS_Z = 5;

	}

	namespace cube_data
	{

		static constexpr char AIR = 255;

		static constexpr char GRASS = 0;
		static constexpr char DIRT = 1;
		static constexpr char ROCK = 2;
		static constexpr char BRICK_WALL = 2;

		// Used for looping over all types, except for air.
		static constexpr char LAST_BLOCK = BRICK_WALL;

		static constexpr int SIDE_TEXTURE = 0;
		static constexpr int TOP_TEXTURE =1;
		static constexpr int BOTTOM_TEXTURE = 2;

		// 0 = Side, 1 = Top, 2 = Bottom
		static char BLOCK_TEXTURES[4][3]{
			{0, 1, 2},
			{2, 2, 2},
			{3, 3, 3},
			{4, 4, 4}
		};

		static int TEXTURE_WIDTH = 32;
		static int TEXTURE_HEIGHT = 32;

		static std::vector<std::string> textures {
			"../resources/0001_grass_side.png",
			"../resources/0002_grass_top.png",
			"../resources/0003_dirt.png",
			"../resources/0004_rock.png",
			"../resources/0005_brick_wall.png",
		};

	}

	namespace graphics_data
	{

		static int fps = 60;
		static int windowWidth = 800;
		static int windowHeight = 600;
		static float fov = 80;

	}

	namespace input_data
	{

		// Keyboard #####################################################

		static int moveForwardButton = GLFW_KEY_W;
		static int moveLeftButton = GLFW_KEY_A;
		static int moveRightButton = GLFW_KEY_D;
		static int moveBackwardButton = GLFW_KEY_S;

		static int jumpButton = GLFW_KEY_SPACE;

		// Mouse ########################################################

		static int action1Button = GLFW_MOUSE_BUTTON_LEFT;
		static int action2Button = GLFW_MOUSE_BUTTON_RIGHT;

	}

	namespace texture_paths
	{
		static std::string scout =  "../resources/ScoutSmal.png";
		static std::string scout2 = "../resources/ScoutSmal2.png";
	}

}

#endif /* SRC_CONFIG_DATA_H_ */
