#define GLM_FORCE_RADIANS

#include <string>
#include <exception>
#include <iostream>
#include <experimental/filesystem>

#include "game.h"

int main() {

  std::experimental::filesystem::path();

  try {
    Game game;
    game.run();

  } catch (std::exception &e) {
    std::cout << "An error occured \n" << e.what() << "\n";
  } catch (...) {
    std::cout << "An error that is not derived from std::exception occured \n";
  }

}
