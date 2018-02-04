
#include "settingsPersistence.h"
#include "../dependencies/tinyXml2/tinyxml2.h"

#include <cstdlib>
#include <string>

#include "../config/data.h"

using namespace tinyxml2;
using namespace config;
using namespace std;

void
SettingsPersistence::loadSettings() {
  XMLDocument doc;
  XMLError errorCode{doc.LoadFile(settingsFile.c_str())};

  // If there is an error, it is probably because the file dose not exist.
  // There will be a new one generated when closing the game.
  if (errorCode != 0)
    return;

  XMLNode* root{doc.FirstChild()};

  {
    XMLElement* graphicsData{root->FirstChildElement("graphics_data")};

    {
      XMLElement* fps{graphicsData->FirstChildElement("fps")};
      graphics_data::fps = std::stoi(fps->GetText());
    }

    {
      XMLElement* fullScreen{graphicsData->FirstChildElement("fullScreen")};
      string value{fullScreen->GetText()};
      graphics_data::fps = value == "true";
    }

    {
      XMLElement* fov{graphicsData->FirstChildElement("fov")};
      graphics_data::fov = std::stod(fov->GetText());
    }
  }

  {
    XMLElement* inputData{root->FirstChildElement("input_data")};

    XMLElement* mouseSenseX{inputData->FirstChildElement("mouseSensitivityX")};
    input_data::mouseSensitivityX = std::stod(mouseSenseX->GetText());

    XMLElement* mouseSenseY{inputData->FirstChildElement("mouseSensitivityY")};
    input_data::mouseSensitivityY = std::stod(mouseSenseY->GetText());
  }

  {
    XMLElement* audio{root->FirstChildElement("audio")};

    XMLElement* maserVolume{audio->FirstChildElement("master_volume")};
    audio::maserVolume = std::stod(maserVolume->GetText());

    XMLElement* soundVolume{audio->FirstChildElement("sound_volume")};
    audio::soundVolume = std::stod(soundVolume->GetText());

    XMLElement* musicVolume{audio->FirstChildElement("music_volume")};
    audio::musicVolume = std::stod(musicVolume->GetText());
  }
}

void
SettingsPersistence::storeSettings() {
  XMLDocument doc;
  XMLNode* root{doc.NewElement("xml")};
  doc.InsertFirstChild(root);

  {
    XMLElement* graphicsData{doc.NewElement("graphics_data")};
    root->InsertEndChild(graphicsData);

    {
      XMLElement* fps{doc.NewElement("fps")};
      fps->SetText(graphics_data::fps);
      graphicsData->InsertEndChild(fps);
    }

    {
      XMLElement* fullscreen{doc.NewElement("fullScreen")};
      fullscreen->SetText(graphics_data::fullScreen);
      graphicsData->InsertEndChild(fullscreen);
    }

    {
      XMLElement* fov{doc.NewElement("fov")};
      fov->SetText(graphics_data::fov);
      graphicsData->InsertEndChild(fov);
    }
  }

  {
    XMLElement* inputData{doc.NewElement("input_data")};
    root->InsertEndChild(inputData);

    {
      XMLElement* mouseSenseX{doc.NewElement("mouseSensitivityX")};
      mouseSenseX->SetText(input_data::mouseSensitivityX);
      inputData->InsertEndChild(mouseSenseX);
    }

    {
      XMLElement* mouseSenseY{doc.NewElement("mouseSensitivityY")};
      mouseSenseY->SetText(input_data::mouseSensitivityY);
      inputData->InsertEndChild(mouseSenseY);
    }
  }

  {
    XMLElement* audio{doc.NewElement("audio")};
    root->InsertEndChild(audio);

    {
      XMLElement* maserVolume{doc.NewElement("master_volume")};
      maserVolume->SetText(audio::maserVolume);
      audio->InsertEndChild(maserVolume);
    }

    {
      XMLElement* soundVolume{doc.NewElement("sound_volume")};
      soundVolume->SetText(audio::soundVolume);
      audio->InsertEndChild(soundVolume);
    }

    {
      XMLElement* musicVolume{doc.NewElement("music_volume")};
      musicVolume->SetText(audio::musicVolume);
      audio->InsertEndChild(musicVolume);
    }
  }

  doc.SaveFile(settingsFile.c_str());
}
