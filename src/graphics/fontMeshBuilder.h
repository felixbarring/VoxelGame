
#ifndef SRC_GRAPHICS_FONTMESHBUILDER_H_
#define SRC_GRAPHICS_FONTMESHBUILDER_H_

#include <string>

#include "mesh/meshElement.h"

#include "../config/data.h"

namespace graphics {

class FontMeshBuilder {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	FontMeshBuilder(std::string &pathToLayout, int atlasWidth, int atlasHeight);

	virtual ~FontMeshBuilder();

// ########################################################
// Member Functions########################################
// ########################################################

	mesh::MeshElement buldMeshForString(const std::string &str, float height);

	float lenghtOfString(const std::string &str, int height);

	int splitStringAt(const  std::string &str, int height, float maxLength);

private:

	void printCharData();

// ########################################################
// Instance Variables #####################################
// ########################################################

	struct CharData {
		int width;
		int height;

		int xPosition;
		int yPosition;
	};

	CharData charData[config::font_data::charsInFont];
	const int ATLAS_WIDTH;
	const int ATLAS_HEIGHT;

};

} /* namespace graphics */

#endif /* SRC_GRAPHICS_FONTMESHBUILDER_H_ */
