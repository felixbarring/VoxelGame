
#include "creationOptions.h"

#include <iostream>
#include "../../../dependencies/tinyXml2/tinyxml2.h"
#include "../../../config/data.h"

namespace chunk {

std::string createFileName(std::string& name) {
  return config::dataFolder + name + ".xml";
}

CreationOptions::CreationOptions(std::string name)
  : m_name{std::move(name)} {

  tinyxml2::XMLDocument doc;
  tinyxml2::XMLError errorCode{doc.LoadFile(createFileName(m_name).c_str())};

  // If there is an error, it is probably because the file dose not exist.
  // There will be a new one generated when closing the game.
  if (errorCode != 0)
    return;

  tinyxml2::XMLNode* root{doc.FirstChild()};

  {
    {
      tinyxml2::XMLElement* flat{root->FirstChildElement("flat")};
      std::string value(flat->GetText());
      m_flat = value == "true";
    }

    {
      tinyxml2::XMLElement* different{root->FirstChildElement("different_cube_for_each_chunk")};
      std::string value(different->GetText());
      m_differentCubeForEachChunk = value == "true";
    }
  }
}

CreationOptions::CreationOptions(std::string name, bool flat, bool different)
  : m_name{std::move(name)}
  , m_flat{flat}
  , m_differentCubeForEachChunk{different} {

  tinyxml2::XMLDocument doc;
  tinyxml2::XMLNode* root{doc.NewElement("xml")};
  doc.InsertFirstChild(root);

  {
    {
      tinyxml2::XMLElement* name{doc.NewElement("name")};
      name->SetText(m_name.c_str());
      root->InsertEndChild(name);
    }

    {
      tinyxml2::XMLElement* flat{doc.NewElement("flat")};
      flat->SetText(m_flat);
      root->InsertEndChild(flat);
    }

    {
      tinyxml2::XMLElement* different{doc.NewElement("different_cube_for_each_chunk")};
      different->SetText(config::graphics_data::fullScreen);
      root->InsertEndChild(different);
    }
  }

  tinyxml2::XMLError errorCode{doc.SaveFile(createFileName(m_name).c_str())};

  if (errorCode) {
    std::cout << "Error while creating xml file for world options. \n";
  }
}

std::string&
CreationOptions::getName() {
  return m_name;
}

bool
CreationOptions::getFlat() const {
  return m_flat;
}

bool
CreationOptions::differentCubesForEachChunk() {
  return m_differentCubeForEachChunk;
}

}

