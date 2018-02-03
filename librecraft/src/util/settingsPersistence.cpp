
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
  doc.LoadFile(settingsFile.c_str());
  XMLNode* root{doc.FirstChild()};

  {
    XMLElement* graphicsData{root->FirstChildElement("graphics_data")};

    XMLElement* fps{graphicsData->FirstChildElement("fps")};
    graphics_data::fps = std::strtol(fps->GetText(), 0, 100);

    XMLElement* fov{graphicsData->FirstChildElement("fov")};
    graphics_data::fov = std::stod(fov->GetText());
  }

  {
    XMLElement* inputData{root->FirstChildElement("input_data")};

    XMLElement* mouseSenseX{inputData->FirstChildElement("mouseSensitivityX")};
    input_data::mouseSensitivityX = std::stod(mouseSenseX->GetText());

    XMLElement* mouseSenseY{inputData->FirstChildElement("mouseSensitivityY")};
    input_data::mouseSensitivityY = std::stod(mouseSenseY->GetText());
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

  doc.SaveFile(settingsFile.c_str());
}
