#define GLM_FORCE_RADIANS

#include <exception>
#include <iostream>
#include <string>

#include "game.h"

int
main() {
  try {
    Game game;
    game.run();

  } catch (std::exception& e) {
    std::cout << "An error occurred \n" << e.what() << "\n";
  } catch (...) {
    std::cout << "An error that is not derived from std::exception occured \n";
  }
}
