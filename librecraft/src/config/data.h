#ifndef SRC_CONFIG_DATA_H_
#define SRC_CONFIG_DATA_H_

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <string>
#include <vector>

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

// TODO Remove the extern crap.
// TODO The settings should be read from one file and not be global vars.
// TODO Instead a game class should be responsible to set the values in places
// that need them.

namespace config {

extern std::string dataFolder;
extern std::string worldMetaFile;
extern std::string settingsFile;

namespace chunk_data {
extern const int GRAPHICAL_CHUNK_WIDTH;
extern const int GRAPHICAL_CHUNK_HEIGHT;

extern const int CHUNK_WIDTH_AND_DEPTH;
extern const int CHUNK_HEIGHT;

extern int NUMBER_OF_CHUNKS_FROM_MIDDLE_TO_BORDER;
extern const int NUMBER_OF_CHUNKS_Y;
}

namespace cube_data {
extern const char AIR;

extern const char GRASS;
extern const char DIRT;
extern const char COARSE_DIRT;
extern const char STONE;
extern const char STONE_ANDESITE;
extern const char BRICK_WALL;
extern const char SAND;
extern const char CACTUS;
extern const char COAL;
extern const char COAL_ORE;
extern const char COBBLESTONE;
extern const char END_BRICKS;
extern const char END_STONE;
extern const char GRAVEL;
extern const char PLANKS;
extern const char STONE_BRICK;
extern const char LOG_BIRCH;
extern const char LEAVES_BIRCH;
extern const char LIGHT;

extern const char LAST_CUBE_USED_FOR_GENERATION;
extern const char LAST_CUBE_USED_FOR_BUILDING;

extern const char WATER;
extern const char BED_ROCK;
extern const char SELECTED;
extern const char LIGHT;

extern const char LOG_OAK;

// Used for looping over all types, except for air.
extern const char LAST_CUBE;

extern const int SIDE_TEXTURE;
extern const int TOP_TEXTURE;
extern const int BOTTOM_TEXTURE;

// 0 = Side, 1 = Top, 2 = Bottom
extern const char BLOCK_TEXTURES[21][3];

extern const unsigned TEXTURE_WIDTH;
extern const unsigned TEXTURE_HEIGHT;

extern const std::vector<std::string> textures;

// TODO Fix better thumbnails
extern const std::vector<std::string> thumbnails;
}

namespace graphics_data {
extern int fps;

extern bool fullScreen;

extern int windowWidth;
extern int windowHeight;

extern float fov;

extern float virtualWidth;
extern float virtualHeight;

extern char directSunlightValue;

extern glm::vec3 skyColor;
}

namespace input_data {
// Keyboard #####################################################

extern sf::Keyboard::Key moveForwardButton;
extern sf::Keyboard::Key moveLeftButton;
extern sf::Keyboard::Key moveRightButton;
extern sf::Keyboard::Key moveBackwardButton;

extern sf::Keyboard::Key jumpButton;
extern sf::Keyboard::Key goDownButton;

extern sf::Keyboard::Key eraseTextButton;

extern sf::Keyboard::Key increaseBuildingCube;
extern sf::Keyboard::Key decreaseBuildingCube;

extern sf::Keyboard::Key openTerminal;
extern sf::Keyboard::Key tab;
extern sf::Keyboard::Key enter;
extern sf::Keyboard::Key up;
extern sf::Keyboard::Key down;

extern sf::Keyboard::Key escape;

// Mouse ########################################################
extern sf::Mouse::Button action1Button;
extern sf::Mouse::Button action2Button;

extern float mouseSensitivityX;
extern float mouseSensitivityY;
}

namespace gui_data {
extern const std::string title;

extern const std::string solidBlack;
extern const std::string solidWhite;

extern const std::string transparentGuiBox;
extern const std::string guiBox;
extern const std::string button;
extern const std::string buttonToggled;
extern const std::string highlight;

extern const std::string checkBox;
extern const std::string checkBoxChecked;

extern const std::string radioButton;
extern const std::string radioButtonChecked;

extern const std::string slider;
extern const std::string sliderKnob;

extern const std::string crossHair;
extern const std::string mouse;
}

namespace font_data {
extern const int charsInFont;
extern const int fontAtlasWidth;
extern const int fontAtlasHeight;

extern std::string font;
extern std::string fontLayout;
}

namespace cube_map_data {
extern std::vector<std::string> cubeMap1;

extern const int cubeMap1Width;
extern const int cubeMap1Height;
}

namespace audio {
extern const std::string buttonPressed;
extern const std::string cubeAdded;
extern const std::string cubeRemoved;

extern const std::vector<std::string> footStepSounds;

extern const std::string menuMusic;

extern double maserVolume;
extern double musicVolume;
extern double soundVolume;
}
}

#endif /* SRC_CONFIG_DATA_H_ */
