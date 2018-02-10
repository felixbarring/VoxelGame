#define GLM_FORCE_RADIANS

#include <exception>
#include <iostream>
#include <string>

#include "game.h"
#include "util/settingsPersistence.h"

int
main() {
  try {
    SettingsPersistence settingsPersistence{};
    settingsPersistence.loadSettings();

    Game game;
    game.run();

    settingsPersistence.storeSettings();
  } catch (std::exception& e) {
    std::cout << "An error occurred \n" << e.what() << "\n";
  } catch (...) {
    std::cout << "An error that is not derived from std::exception occured \n";
  }
}
