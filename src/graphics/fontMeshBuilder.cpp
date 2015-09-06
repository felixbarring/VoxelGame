
#include "fontMeshBuilder.h"

#include <iostream>
#include <fstream>
#include <vector>

namespace graphics {

// TODO Remove all the 256 magic numbers!


// ########################################################
// Constructor/Destructor #################################
// ########################################################

FontMeshBuilder::FontMeshBuilder(std::string &pathToLayout, int atlasWidth, int atlasHeight) :
	ATLAS_WIDTH(atlasWidth),
	ATLAS_HEIGHT(atlasHeight)
{

	std::vector<std::string> list;
	std::ifstream in_stream;
	std::string line;
	in_stream.open(pathToLayout.c_str());

	// Add all lines to the vector
	while ( getline( in_stream, line) ) {
		list.push_back(line);
	}
	in_stream.close();

	// The first line in the file is just a comment, hence the + 1
	for (int i = 0 + 1; i < 256 + 1; i++) {
		std::string line = list.at(i);

		CharData charD;
		int counter = 0;
		std::string str = "";
		for (int j = 0; j < line.size(); j++) {

			char ch = line[j];
			if (ch != ' ') {
				str.push_back(ch);
			}
			if (ch == ' ' || j == line.size() - 1) {
				switch (counter) {
					case 0: charD.width = std::stoi(str); break;
					case 1: charD.height = std::stoi(str); break;
					case 2: charD.xPosition = std::stoi(str); break;
					case 3: charD.yPosition = std::stoi(str); break;
					default: break; // ERRORS :s
				}
				counter++;
				str = "";
				charData[i-1] = charD;
			}
		}
	}

}

FontMeshBuilder::~FontMeshBuilder()
{

}

// ########################################################
// Member Functions########################################
// ########################################################

mesh::MeshElement FontMeshBuilder::buldMeshForString(const std::string &str, float height)
{


	const int NUMBER_OF_CHARS = str.length();
	const int VS = 12;
	const int TS = 8;
	const int ES = 6;

	std::vector<float> vertices; //[NUMBER_OF_CHARS*VS]; // = new float[NUMBER_OF_CHARS*VS];
	std::vector<float> uvCoordinates; //[NUMBER_OF_CHARS*TS]; // = new float[NUMBER_OF_CHARS*TS];
	std::vector<short> elements; //[NUMBER_OF_CHARS*ES]; // = new short[NUMBER_OF_CHARS*ES];

	float width = height;

	int vertexCounter = 0;
	int uvCounter = 0;
	int elementsCounter = 0;

	float xOrigin = 0;
	float yOrigin = 0;

	int loopCounter = 0;
	float xOffset = 0;
	for(char c : str){

		//if(c >= 256 || c < 0){

			// Handle  error

			//try {
			//	throw new Exception("Illegal char value, must be in the range of [0, 256)");
			//} catch (Exception e) {
			//	e.printStackTrace();
			//}

			//c = ERROR_CHARACTHER;
		//}

		CharData cd = charData[c];

		width = height*(cd.width / (float)cd.height);

		/*
		vertices[0 + vertexCounter] = xOrigin + xOffset;
		vertices[1 + vertexCounter] = yOrigin;
		vertices[2 + vertexCounter] = 0;

		vertices[3 + vertexCounter] = xOrigin + width + xOffset;
		vertices[4 + vertexCounter] = yOrigin;
		vertices[5 + vertexCounter] = 0;

		vertices[6 + vertexCounter] = xOrigin + width + xOffset;
		vertices[7 + vertexCounter] = yOrigin + height;
		vertices[8 + vertexCounter] = 0;

		vertices[9 + vertexCounter] = xOrigin + xOffset;
		vertices[10 + vertexCounter] = yOrigin + height;
		vertices[11 + vertexCounter] = 0;

		uvCoordinates[0 + uvCounter] = cd.xPosition / static_cast<float>(ATLAS_WIDTH);
		uvCoordinates[1 + uvCounter] = (cd.yPosition + cd.height) / static_cast<float>(ATLAS_HEIGHT);

		uvCoordinates[2 + uvCounter] = (cd.xPosition + cd.width) / static_cast<float>(ATLAS_WIDTH);
		uvCoordinates[3 + uvCounter] = (cd.yPosition + cd.height) / static_cast<float>(ATLAS_HEIGHT);

		uvCoordinates[4 + uvCounter] = (cd.xPosition + cd.width) / static_cast<float>(ATLAS_WIDTH);
		uvCoordinates[5 + uvCounter] = cd.yPosition / static_cast<float>(ATLAS_HEIGHT);

		uvCoordinates[6 + uvCounter] = cd.xPosition / static_cast<float>(ATLAS_WIDTH);
		uvCoordinates[7 + uvCounter] = cd.yPosition / static_cast<float>(ATLAS_HEIGHT);

		elements[0 + elementsCounter] = (short) (0 + loopCounter*4);
		elements[1 + elementsCounter] = (short) (1 + loopCounter*4);
		elements[2 + elementsCounter] = (short) (2 + loopCounter*4);
		elements[3 + elementsCounter] = (short) (0 + loopCounter*4);
		elements[4 + elementsCounter] = (short) (2 + loopCounter*4);
		elements[5 + elementsCounter] = (short) (3 + loopCounter*4);
		*/

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

		uvCoordinates.push_back(cd.xPosition / static_cast<float>(ATLAS_WIDTH));
		uvCoordinates.push_back((cd.yPosition + cd.height) / static_cast<float>(ATLAS_HEIGHT));

		uvCoordinates.push_back((cd.xPosition + cd.width) / static_cast<float>(ATLAS_WIDTH));
		uvCoordinates.push_back((cd.yPosition + cd.height) / static_cast<float>(ATLAS_HEIGHT));

		uvCoordinates.push_back((cd.xPosition + cd.width) / static_cast<float>(ATLAS_WIDTH));
		uvCoordinates.push_back(cd.yPosition / static_cast<float>(ATLAS_HEIGHT));

		uvCoordinates.push_back(cd.xPosition / static_cast<float>(ATLAS_WIDTH));
		uvCoordinates.push_back(cd.yPosition / static_cast<float>(ATLAS_HEIGHT));

		elements.push_back((short) (0 + loopCounter*4));
		elements.push_back((short) (1 + loopCounter*4));
		elements.push_back((short) (2 + loopCounter*4));
		elements.push_back((short) (0 + loopCounter*4));
		elements.push_back((short) (2 + loopCounter*4));
		elements.push_back((short) (3 + loopCounter*4));

		vertexCounter = vertexCounter + VS;
		uvCounter = uvCounter + TS;
		elementsCounter = elementsCounter + ES;

		loopCounter ++;
		xOffset += width;
	}

	// mesh::MeshElement(vertices, 3 uvCoordinates, 2, elements)

	mesh::MeshElement mesh{vertices, 3, uvCoordinates, 2, elements};

	return {vertices, 3, uvCoordinates, 2, elements};
}

float FontMeshBuilder::lenghtOfString(const std::string &str, int height)
{
	//TODO
	return 1.01;
}

int FontMeshBuilder::splitStringAt(const  std::string &str, int height, float maxLength)
{
	// TODO
	return 101;
}

void FontMeshBuilder::printCharData()
{
	for (CharData cd : charData) {
		std::cout << "Width: " << cd.width << ", Height: " << cd.height << ", xPosition: " << cd.xPosition << ", yPosition: " << cd.yPosition << " \n";
	}
}


} /* namespace graphics */
