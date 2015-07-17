
#ifndef SRC_CONFIG_CUBEDATA_H_
#define SRC_CONFIG_CUBEDATA_H_

#include <vector>
#include <string>

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

#endif /* SRC_CONFIG_CUBEDATA_H_ */
