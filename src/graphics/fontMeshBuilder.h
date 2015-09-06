
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

	FontMeshBuilder(std::string &pathToLayout);

	virtual ~FontMeshBuilder();

// ########################################################
// Member Functions########################################
// ########################################################

	mesh::MeshElement buldMeshForString(const std::string &str, float height);

	float lenghtOfString(const std::string &str, int height);

	int splitStringAt(const  std::string &str, int height, float maxLength);

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

};

} /* namespace graphics */

#endif /* SRC_GRAPHICS_FONTMESHBUILDER_H_ */
