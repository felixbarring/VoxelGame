
#ifndef SRC_CONFIG_DATA_H_
#define SRC_CONFIG_DATA_H_

#include <GL/glew.h>

#include <vector>
#include <string>

namespace chunk_data
{

	static constexpr int GRAPHICAL_CHUNK_WIDTH = 16;
	static constexpr int GRAPHICAL_CHUNK_HEIGHT = 16;
	static constexpr int GRAPHICAL_CHUNK_DEPTH = 16;

	static constexpr int CHUNK_WIDHT = 16;
	static constexpr int CHUNK_HEIGHT = 16;
	static constexpr int CHUNK_DEPTH = 16;

	static constexpr int NUMBER_OF_CHUNKS_X = 4;
	static constexpr int NUMBER_OF_CHUNKS_Y = 4;
	static constexpr int NUMBER_OF_CHUNKS_Z = 4;

}

namespace cube_data
{

	static constexpr char AIR = 255;

	static constexpr char GRASS = 0;
	static constexpr char DIRT = 1;
	static constexpr char ROCK = 2;
	static constexpr char BRICK_WALL = 2;

	static constexpr int SIDE_TEXTURE = 0;
	static constexpr int TOP_TEXTURE =1;
	static constexpr int BOTTOM_TEXTURE = 2;

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

	static int action1 = GLFW_MOUSE_BUTTON_1;
	static int action2 = GLFW_MOUSE_BUTTON_2;

}

#endif /* SRC_CONFIG_DATA_H_ */
