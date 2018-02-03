
#include "data.h"

namespace config {

std::string dataFolder = "data/";
std::string worldMetaFile = "data/meta";
std::string settingsFile = "data/settings.xml";

namespace chunk_data {

const int GRAPHICAL_CHUNK_WIDTH = 16;
const int GRAPHICAL_CHUNK_HEIGHT = 128;

const int CHUNK_WIDTH_AND_DEPTH = 16;
const int CHUNK_HEIGHT = 128;

const int NUMBER_OF_CHUNKS_FROM_MIDDLE_TO_BORDER = 8;
const int NUMBER_OF_CHUNKS_Y = 1;
}

namespace cube_data {

const char AIR = 255;

const char GRASS = 0;
const char DIRT = 1;
const char COARSE_DIRT = 2;
const char STONE = 3;
const char STONE_ANDESITE = 4;
const char BRICK_WALL = 5;
const char SAND = 6;
const char CACTUS = 7;
const char COAL = 8;
const char COAL_ORE = 9;
const char COBBLESTONE = 10;
const char END_BRICKS = 11;
const char END_STONE = 12;
const char GRAVEL = 13;
const char PLANKS = 14;
const char STONE_BRICK = 15;
const char LOG_BIRCH = 16;
const char LEAVES_BIRCH = 17;
const char LIGHT = 18;

const char LAST_CUBE_USED_FOR_GENERATION = LOG_BIRCH;
const char LAST_CUBE_USED_FOR_BUILDING = LIGHT; // Better name needed

const char WATER = 19;
const char BED_ROCK = 20;

// Used for looping over all types, except for air. water, bedrock
const char LAST_CUBE = BED_ROCK;

const int SIDE_TEXTURE = 0;
const int TOP_TEXTURE = 1;
const int BOTTOM_TEXTURE = 2;

// 0 = Side, 1 = Top, 2 = Bottom
const char BLOCK_TEXTURES[21][3]{
  {0, 1, 2},    // GRASS
  {2, 2, 2},    // DIRT
  {3, 3, 3},    // COARSE_DIRT
  {4, 4, 4},    // STONE
  {5, 5, 5},    // STONE_ANDESITE
  {6, 7, 7},    // BRICK_WALL
  {8, 8, 8},    // SAND
  {9, 11, 10},  // CACTUS
  {12, 12, 12}, // COAL
  {13, 13, 13}, // COAL_ORE
  {14, 16, 15}, // COBBLESTONE
  {17, 17, 17}, // END_BRICKS
  {18, 18, 18}, // END_STONE
  {19, 19, 19}, // GRAVEL
  {20, 20, 20}, // PLANKS
  {21, 21, 21}, // STONE_BRICK
  {22, 23, 23}, // LOG_BIRCH
  {1, 1, 1},    // LEAVES_BIRCH
  {25, 25, 25}, // LIGHT
  {26, 26, 26}, // WATER
  {27, 27, 27}, // BEDROCK

};

const unsigned TEXTURE_WIDTH = 32;
const unsigned TEXTURE_HEIGHT = 32;

std::string pre = "../resources/cubes/";

const std::vector<std::string> textures{
  pre + "grass_side.png",
  pre + "grass_top.png",

  pre + "dirt.png",
  pre + "coarse_dirt.png",

  pre + "stone.png",
  pre + "stone_andesite.png",

  pre + "brick.png",
  pre + "brick_top.png",

  pre + "sand.png",

  pre + "cactus_side.png",
  pre + "cactus_bottom.png",
  pre + "cactus_top.png",

  pre + "coal_block.png",
  pre + "coal_ore.png",

  pre + "cobblestone.png",
  pre + "cobblestone_bottom.png",
  pre + "cobblestone_top.png",

  pre + "end_bricks.png",

  pre + "end_stone.png",

  pre + "gravel.png",

  pre + "planks_big_oak.png",

  pre + "stonebrick.png",

  pre + "log_birch.png",
  pre + "log_birch_top.png",
  pre + "leaves_birch.png",

  pre + "light.png",
  pre + "water.png",
  pre + "bedrock.png",
};

// TODO Fix better thumbnails
const std::vector<std::string> thumbnails{
  "../resources/cubes/grass_side.png",
  "../resources/cubes/dirt.png",
  "../resources/cubes/coarse_dirt.png",
  "../resources/cubes/stone.png",
  "../resources/cubes/stone_andesite.png",
  "../resources/cubes/brick.png",
  "../resources/cubes/sand.png",
  "../resources/cubes/cactus_side.png",
  "../resources/cubes/coal_block.png",
  "../resources/cubes/coal_ore.png",
  "../resources/cubes/cobblestone.png",
  "../resources/cubes/end_bricks.png",
  "../resources/cubes/end_stone.png",
  "../resources/cubes/gravel.png",
  "../resources/cubes/planks_big_oak.png",
  "../resources/cubes/stonebrick.png",
  "../resources/cubes/log_birch.png",
  "../resources/cubes/leaves_birch.png",
  "../resources/cubes/light.png",
};
}

namespace graphics_data {
int fps = 100;

int windowWidth = 1500;
int windowHeight = 800;

float fov = 80; // degrees

float virtualWidth{800};
float virtualHeight{600};

int directSunlightValue{15};

glm::vec3 skyColor(0.47f, 0.76f, 0.93f);
}

namespace input_data {
// Keyboard #####################################################

sf::Keyboard::Key moveForwardButton = sf::Keyboard::W;
sf::Keyboard::Key moveLeftButton = sf::Keyboard::A;
sf::Keyboard::Key moveRightButton = sf::Keyboard::D;
sf::Keyboard::Key moveBackwardButton = sf::Keyboard::S;

sf::Keyboard::Key jumpButton = sf::Keyboard::Space;
sf::Keyboard::Key goDownButton = sf::Keyboard::LShift;

sf::Keyboard::Key eraseTextButton = sf::Keyboard::BackSpace;

sf::Keyboard::Key increaseBuildingCube = sf::Keyboard::E;
sf::Keyboard::Key decreaseBuildingCube = sf::Keyboard::Q;

sf::Keyboard::Key openTerminal = sf::Keyboard::T;
sf::Keyboard::Key tab = sf::Keyboard::Tab;
sf::Keyboard::Key enter = sf::Keyboard::Return;
sf::Keyboard::Key up = sf::Keyboard::Up;
sf::Keyboard::Key down = sf::Keyboard::Down;

sf::Keyboard::Key escape = sf::Keyboard::Escape;

// Mouse ########################################################
sf::Mouse::Button action1Button = sf::Mouse::Left;
sf::Mouse::Button action2Button = sf::Mouse::Right;

float mouseSensitivityX{0.001};
float mouseSensitivityY{0.001};
}

namespace gui_data {
const std::string title = "../resources/gui/title.png";

const std::string solidBlack = "../resources/gui/solid_black.png";
const std::string solidWhite = "../resources/gui/solid_white.png";

const std::string transparentGuiBox =
  "../resources/gui/transparent_gui_box.png";
const std::string guiBox = "../resources/gui/gui_box.png";
const std::string button = "../resources/gui/button.png";
const std::string buttonToggled = "../resources/gui/button_toggled.png";
const std::string highlight = "../resources/gui/highlight.png";

const std::string checkBox = "../resources/gui/check_box.png";
const std::string checkBoxChecked = "../resources/gui/check_box_checked.png";

const std::string radioButton = "../resources/gui/radio_button.png";
const std::string radioButtonChecked =
  "../resources/gui/radio_button_checked.png";

const std::string slider = "../resources/gui/slider_body.png";
const std::string sliderKnob = "../resources/gui/slider_knob.png";

const std::string crossHair = "../resources/gui/crosshair.png";
const std::string mouse = "../resources/gui/mouse.png";
}

namespace font_data {
const int charsInFont = 256;
const int fontAtlasWidth = 1024;
const int fontAtlasHeight = 1024;

std::string font = "../resources/font/font1.png";
std::string fontLayout = "../resources/font/font1.txt";
}

namespace cube_map_data {
std::vector<std::string> cubeMap1{"../resources/skybox/right.png",
                                  "../resources/skybox/left.png",
                                  "../resources/skybox/top.png",
                                  "../resources/skybox/bottom.png",
                                  "../resources/skybox/back.png",
                                  "../resources/skybox/front.png"};

const int cubeMap1Width = 2048;
const int cubeMap1Height = 2048;
}

namespace souds {
const std::string buttonPressed = "../resources/sound/button.wav";
const std::string cubeAdded = "../resources/sound/cube_place.wav";
const std::string cubeRemoved = "../resources/sound/cube_remove.wav";

const std::vector<std::string> footStepSounds{
  "../resources/sound/steps/step1.ogg",
  "../resources/sound/steps/step2.ogg",
  "../resources/sound/steps/step3.ogg",
  "../resources/sound/steps/step4.ogg",
  "../resources/sound/steps/step5.ogg",
  "../resources/sound/steps/step6.ogg",
  "../resources/sound/steps/step7.ogg",
  "../resources/sound/steps/step8.ogg",
  "../resources/sound/steps/step9.ogg",
  "../resources/sound/steps/step10.ogg"};
}

namespace music {
const std::string menuMusic = "../resources/music/Continue Life.ogg";
}
}
