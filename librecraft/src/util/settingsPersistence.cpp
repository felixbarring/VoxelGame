
#include "settingsPersistence.h"
#include "../dependencies/tinyXml2/tinyxml2.h"

#include <cstdlib>
#include <iostream>
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
  if (errorCode != 0) {
    cout << "Could not open settings xml file. \n";
    return;
  }

  XMLNode* root{doc.FirstChild()};

  {
    XMLElement* chunkData{root->FirstChildElement("chunk_data")};

    if (chunkData) {
      XMLElement* numberOfChunks{
        chunkData->FirstChildElement("number_of_chunks_from_middle_to_border")};
      if (numberOfChunks)
        chunk_data::NUMBER_OF_CHUNKS_FROM_MIDDLE_TO_BORDER =
          std::stoi(numberOfChunks->GetText());
    }
  }

  {
    XMLElement* graphicsData{root->FirstChildElement("graphics_data")};

    if (graphicsData) {
      XMLElement* fps{graphicsData->FirstChildElement("fps")};
      if (fps)
        graphics_data::fps = std::stoi(fps->GetText());

      XMLElement* fullScreen{graphicsData->FirstChildElement("fullScreen")};
      if (fullScreen) {
        string value{fullScreen->GetText()};
        graphics_data::fullScreen = value == "true";
      }

      XMLElement* fov{graphicsData->FirstChildElement("fov")};
      if (fov)
        graphics_data::fov = std::stod(fov->GetText());
    }
  }

  {
    XMLElement* inputData{root->FirstChildElement("input_data")};

    if (inputData) {
      XMLElement* mouseSenseX{
        inputData->FirstChildElement("mouseSensitivityX")};
      if (mouseSenseX)
        input_data::mouseSensitivityX = std::stod(mouseSenseX->GetText());

      XMLElement* mouseSenseY{
        inputData->FirstChildElement("mouseSensitivityY")};
      if (mouseSenseY)
        input_data::mouseSensitivityY = std::stod(mouseSenseY->GetText());
    }
  }

  {
    XMLElement* audio{root->FirstChildElement("audio")};

    if (audio) {
      XMLElement* maserVolume{audio->FirstChildElement("master_volume")};
      if (maserVolume)
        audio::maserVolume = std::stod(maserVolume->GetText());

      XMLElement* soundVolume{audio->FirstChildElement("sound_volume")};
      if (soundVolume)
        audio::soundVolume = std::stod(soundVolume->GetText());

      XMLElement* musicVolume{audio->FirstChildElement("music_volume")};
      if (musicVolume)
        audio::musicVolume = std::stod(musicVolume->GetText());
    }
  }
}

void
SettingsPersistence::storeSettings() {
  XMLDocument doc;
  XMLNode* root{doc.NewElement("xml")};
  doc.InsertFirstChild(root);

  {
    XMLElement* chunkData{doc.NewElement("chunk_data")};
    root->InsertEndChild(chunkData);

    {
      XMLElement* numberOfChunks{
        doc.NewElement("number_of_chunks_from_middle_to_border")};
      numberOfChunks->SetText(
        chunk_data::NUMBER_OF_CHUNKS_FROM_MIDDLE_TO_BORDER);
      chunkData->InsertEndChild(numberOfChunks);
    }
  }

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

  XMLError errorCode{doc.SaveFile(settingsFile.c_str())};
  if (errorCode != 0) {
    cout << "Could not save settings xml file. \n";
    return;
  }
}
