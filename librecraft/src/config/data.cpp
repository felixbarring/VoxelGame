
#include "data.h"

namespace config {

std::string dataFolder = "data/";
std::string worldMetaFile = "data/meta";
std::string settingsFile = "data/settings.xml";

std::string pre = "resources/";

namespace chunk_data {

const int GRAPHICAL_CHUNK_WIDTH = 16;
const int GRAPHICAL_CHUNK_HEIGHT = 128;

const int CHUNK_WIDTH_AND_DEPTH = 16;
const int CHUNK_HEIGHT = 128;

int NUMBER_OF_CHUNKS_FROM_MIDDLE_TO_BORDER = 5;
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

const std::vector<std::string> textures{
  pre + "cubes/grass_side.png",
  pre + "cubes/grass_top.png",

  pre + "cubes/dirt.png",
  pre + "cubes/coarse_dirt.png",

  pre + "cubes/stone.png",
  pre + "cubes/stone_andesite.png",

  pre + "cubes/brick.png",
  pre + "cubes/brick_top.png",

  pre + "cubes/sand.png",

  pre + "cubes/cactus_side.png",
  pre + "cubes/cactus_bottom.png",
  pre + "cubes/cactus_top.png",

  pre + "cubes/coal_block.png",
  pre + "cubes/coal_ore.png",

  pre + "cubes/cobblestone.png",
  pre + "cubes/cobblestone_bottom.png",
  pre + "cubes/cobblestone_top.png",

  pre + "cubes/end_bricks.png",

  pre + "cubes/end_stone.png",

  pre + "cubes/gravel.png",

  pre + "cubes/planks_big_oak.png",

  pre + "cubes/stonebrick.png",

  pre + "cubes/log_birch.png",
  pre + "cubes/log_birch_top.png",
  pre + "cubes/leaves_birch.png",

  pre + "cubes/light.png",
  pre + "cubes/water.png",
  pre + "cubes/bedrock.png",
};

// TODO Fix better thumbnails
const std::vector<std::string> thumbnails{
  pre + "cubes/grass_side.png",
  pre + "cubes/dirt.png",
  pre + "cubes/coarse_dirt.png",
  pre + "cubes/stone.png",
  pre + "cubes/stone_andesite.png",
  pre + "cubes/brick.png",
  pre + "cubes/sand.png",
  pre + "cubes/cactus_side.png",
  pre + "cubes/coal_block.png",
  pre + "cubes/coal_ore.png",
  pre + "cubes/cobblestone.png",
  pre + "cubes/end_bricks.png",
  pre + "cubes/end_stone.png",
  pre + "cubes/gravel.png",
  pre + "cubes/planks_big_oak.png",
  pre + "cubes/stonebrick.png",
  pre + "cubes/log_birch.png",
  pre + "cubes/leaves_birch.png",
  pre + "cubes/light.png",
};
}

namespace graphics_data {
int fps = 100;

bool fullScreen = false;

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
const std::string title = pre + "gui/title.png";

const std::string solidBlack = pre + "gui/solid_black.png";
const std::string solidWhite = pre + "gui/solid_white.png";

const std::string transparentGuiBox = pre + "gui/transparent_gui_box.png";
const std::string guiBox = pre + "gui/gui_box.png";
const std::string button = pre + "gui/button.png";
const std::string buttonToggled = pre + "gui/button_toggled.png";
const std::string highlight = pre + "gui/highlight.png";

const std::string checkBox = pre + "gui/check_box.png";
const std::string checkBoxChecked = pre + "gui/check_box_checked.png";

const std::string radioButton = pre + "gui/radio_button.png";
const std::string radioButtonChecked = pre + "gui/radio_button_checked.png";

const std::string slider = pre + "gui/slider_body.png";
const std::string sliderKnob = pre + "gui/slider_knob.png";

const std::string crossHair = pre + "gui/crosshair.png";
const std::string mouse = pre + "gui/mouse.png";
}

namespace font_data {
const int charsInFont = 256;
const int fontAtlasWidth = 1024;
const int fontAtlasHeight = 1024;

std::string font = pre + "font/font1.png";
std::string fontLayout = pre + "font/font1.txt";
}

namespace cube_map_data {
std::vector<std::string> cubeMap1{pre + "skybox/right.png",
                                  pre + "skybox/left.png",
                                  pre + "skybox/top.png",
                                  pre + "skybox/bottom.png",
                                  pre + "skybox/back.png",
                                  pre + "skybox/front.png"};

const int cubeMap1Width = 2048;
const int cubeMap1Height = 2048;
}

namespace audio {
const std::string buttonPressed = pre + "sound/button.wav";
const std::string cubeAdded = pre + "sound/cube_place.wav";
const std::string cubeRemoved = pre + "sound/cube_remove.wav";

const std::vector<std::string> footStepSounds{
  pre + "sound/steps/step1.ogg",
  pre + "sound/steps/step2.ogg",
  pre + "sound/steps/step3.ogg",
  pre + "sound/steps/step4.ogg",
  pre + "sound/steps/step5.ogg",
  pre + "sound/steps/step6.ogg",
  pre + "sound/steps/step7.ogg",
  pre + "sound/steps/step8.ogg",
  pre + "sound/steps/step9.ogg",
  pre + "sound/steps/step10.ogg"};

const std::string menuMusic = pre + "music/Continue Life.ogg";

double maserVolume = 1.0;
double musicVolume = 100.0;
double soundVolume = 100.0;
}
}
