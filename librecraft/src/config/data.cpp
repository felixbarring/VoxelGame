
#include "data.h"

namespace config
{

    std::string dataFolder = "data/";
    std::string worldMetaFile = "data/meta";

    namespace chunk_data {
         const int GRAPHICAL_CHUNK_WIDTH = 16;
         const int GRAPHICAL_CHUNK_HEIGHT = 64;

         const int CHUNK_WIDTH_AND_DEPTH = 16;
         const int CHUNK_HEIGHT = 128;

         const int NUMBER_OF_CHUNKS_FROM_MIDDLE_TO_BORDER = 5;
         const int NUMBER_OF_CHUNKS_Y = 1;
    }

    namespace cube_data {
         const char AIR = 255;

         const char GRASS = 0;
         const char DIRT = 1;
         const char ROCK = 2;
         const char MORE_ROCK = 3;
         const char BRICK_WALL = 4;
         const char SAND = 5;

         const char LAST_CUBE_USED_FOR_BUILDING = SAND; // Better name needed

         const char WATER = 6;
         const char BED_ROCK = 7;
         const char SELECTED = 8;

        // Used for looping over all types, except for air.
         const char LAST_CUBE = SELECTED;

         const int SIDE_TEXTURE = 0;
         const int TOP_TEXTURE = 1;
         const int BOTTOM_TEXTURE = 2;

        // 0 = Side, 1 = Top, 2 = Bottom
         const char BLOCK_TEXTURES[9][3] {
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

         const int TEXTURE_WIDTH = 32;
         const int TEXTURE_HEIGHT = 32;

         const std::vector<std::string> textures {
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
         const std::vector<std::string> thumbnails {
            "../resources/cubes/grass_side.png",
            "../resources/cubes/dirt.png",
            "../resources/cubes/rock.png",
            "../resources/cubes/more_rock.png",
            "../resources/cubes/brick.png",
            "../resources/cubes/sand.png",
        };

    }

    namespace graphics_data {
        int fps = 100;

        int windowWidth = 1500;
        int windowHeight = 800;

        float fov = 80 * (3.14 / 180); // degree to radians

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

         sf::Keyboard::Key switchBuildingCube = sf::Keyboard::E;

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

    namespace texture_paths {
        // Remove THESE ?
         std::string scout = "../resources/cubes/grass_side.png";
         std::string scout2 = "../resources/cubes/grass_top.png";
    }

    namespace gui_data {
         const std::string title = "../resources/gui/title.png";

         const std::string solidBlack = "../resources/gui/solid_black.png";
         const std::string solidWhite = "../resources/gui/solid_white.png";

         const std::string transparentGuiBox = "../resources/gui/transparent_gui_box.png";
         const std::string guiBox = "../resources/gui/gui_box.png";
         const std::string button = "../resources/gui/button.png";
         const std::string buttonToggled = "../resources/gui/button_toggled.png";
         const std::string highlight = "../resources/gui/highlight.png";

         const std::string checkBox = "../resources/gui/check_box.png";
         const std::string checkBoxChecked = "../resources/gui/check_box_checked.png";

         const std::string radioButton = "../resources/gui/radio_button.png";
         const std::string radioButtonChecked = "../resources/gui/radio_button_checked.png";

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
         std::vector<std::string> cubeMap1 {
            "../resources/skybox/right.png",
            "../resources/skybox/left.png",
            "../resources/skybox/top.png",
            "../resources/skybox/bottom.png",
            "../resources/skybox/back.png",
            "../resources/skybox/front.png"
        };

         const int cubeMap1Width = 2048;
         const int cubeMap1Height = 2048;
    }

    namespace souds {
         const std::string buttonPressed = "../resources/sound/button.wav";
         const std::string cubeAdded = "../resources/sound/cube_place.wav";
         const std::string cubeRemoved =  "../resources/sound/cube_remove.wav";
    }

    namespace music {
         const std::string menuMusic = "../resources/music/Continue Life.ogg";
    }

}

