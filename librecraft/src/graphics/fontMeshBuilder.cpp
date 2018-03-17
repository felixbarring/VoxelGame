#include "fontMeshBuilder.h"

#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

using namespace std;

namespace graphics {

// TODO Remove all the 256 magic numbers!

FontMeshBuilder::FontMeshBuilder(string& pathToLayout,
                                 int atlasWidth,
                                 int atlasHeight)
  : m_atlasWidth(atlasWidth)
  , m_atlasHeight(atlasHeight) {

  vector<string> list;
  ifstream in_stream;
  string line;
  in_stream.open(pathToLayout.c_str());

  // Add all lines to the vector
  while (getline(in_stream, line)) {
    list.push_back(line);
  }
  in_stream.close();

  // The first line in the file is just a comment, hence the + 1
  for (int i = 0 + 1; i < 256 + 1; ++i) {
    string line = list[i];

    CharData charD;
    int counter = 0;
    string str = "";
    for (unsigned j = 0; j < line.size(); ++j) {
      char ch = line[j];
      if (ch != ' ') {
        str.push_back(ch);
      }
      if (ch == ' ' || j == line.size() - 1) {
        switch (counter) {
          case 0:
            charD.width = stoi(str);
            break;
          case 1:
            charD.height = stoi(str);
            break;
          case 2:
            charD.xPosition = stoi(str);
            break;
          case 3:
            charD.yPosition = stoi(str);
            break;
          default:
            break; // ERRORS :s
        }
        counter++;
        str = "";
        charData[i - 1] = charD;
      }
    }
  }
}

unique_ptr<mesh::MeshElement>
FontMeshBuilder::buldMeshForString(const string& str, float height) {

  vector<float> vertices;
  vector<float> uvCoordinates;
  vector<short> elements;

  float width = height;

  float xOrigin = 0;
  float yOrigin = 0;

  int loopCounter = 0;
  float xOffset = 0;
  for (char c : str) {
    CharData cd = charData[c];

    width = height * (cd.width / static_cast<float>(cd.height));

    vertices.push_back(xOrigin + xOffset);
    vertices.push_back(yOrigin);
    vertices.push_back(0);

    vertices.push_back(xOrigin + width + xOffset);
    vertices.push_back(yOrigin);
    vertices.push_back(0);

    vertices.push_back(xOrigin + width + xOffset);
    vertices.push_back(yOrigin + height);
    vertices.push_back(0);

    vertices.push_back(xOrigin + xOffset);
    vertices.push_back(yOrigin + height);
    vertices.push_back(0);

    uvCoordinates.push_back(cd.xPosition / static_cast<float>(m_atlasWidth));
    uvCoordinates.push_back(((m_atlasHeight - cd.yPosition) - cd.height) /
                            static_cast<float>(m_atlasHeight));

    uvCoordinates.push_back((cd.xPosition + cd.width) /
                            static_cast<float>(m_atlasWidth));
    uvCoordinates.push_back(((m_atlasHeight - cd.yPosition) - cd.height) /
                            static_cast<float>(m_atlasHeight));

    uvCoordinates.push_back((cd.xPosition + cd.width) /
                            static_cast<float>(m_atlasWidth));
    uvCoordinates.push_back((m_atlasHeight - cd.yPosition) /
                            static_cast<float>(m_atlasHeight));

    uvCoordinates.push_back(cd.xPosition / static_cast<float>(m_atlasWidth));
    uvCoordinates.push_back((m_atlasHeight - cd.yPosition) /
                            static_cast<float>(m_atlasHeight));

    elements.push_back((short)(0 + loopCounter * 4));
    elements.push_back((short)(1 + loopCounter * 4));
    elements.push_back((short)(2 + loopCounter * 4));
    elements.push_back((short)(0 + loopCounter * 4));
    elements.push_back((short)(2 + loopCounter * 4));
    elements.push_back((short)(3 + loopCounter * 4));

    ++loopCounter;
    xOffset += width;
  }

  vector<pair<vector<float>, int>> vobs{{vertices, 3}, {uvCoordinates, 2}};
  auto mesh = make_unique<mesh::MeshElement>(move(vobs), elements);

  return mesh;
}

float
FontMeshBuilder::lenghtOfString(const std::string& str, float height) {
  return lenghtOfStringAtChar(str, height, str.length());
}

float
FontMeshBuilder::lenghtOfStringAtChar(const std::string& str,
                                      float height,
                                      int num) {
  float stringSize = 0;
  for (int i = 0; i < num; ++i) {
    const CharData cd = charData[str[i]];
    const float width = height * (cd.width / static_cast<float>(cd.height));
    stringSize += width;
  }
  return stringSize;
}

int
FontMeshBuilder::splitStringAt(const std::string& str,
                               int height,
                               float maxLength) {
  if (lenghtOfString(str, height) < maxLength)
    return -1;

  int index = 0;
  float accumulatedLenght = 0;
  while (accumulatedLenght < maxLength) {
    const CharData cd = charData[str[index]];
    const float width = height * (cd.width / static_cast<float>(cd.height));
    accumulatedLenght += width;
    index++;
  }
  return index - 1;
}

void
FontMeshBuilder::printCharData() {
  for (CharData cd : charData) {
    std::cout << "Width: " << cd.width << ", Height: " << cd.height
              << ", xPosition: " << cd.xPosition
              << ", yPosition: " << cd.yPosition << " \n";
  }
}

} /* namespace graphics */
