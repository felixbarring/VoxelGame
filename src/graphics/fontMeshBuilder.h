
/**
 *  \file [fontMeshBuilder.h]
 *  \brief used to build meshed that can be used to render text
 *  \author Felix Bärring
 *
 *  Copyright (c) 2015, Felix Bärring. All rights reserved.
 */

#ifndef SRC_GRAPHICS_FONTMESHBUILDER_H_
#define SRC_GRAPHICS_FONTMESHBUILDER_H_

#include <string>
#include <memory>
#include <map>

#include "mesh/meshElement.h"

#include "../config/data.h"

namespace graphics {


/**
 * \class FontMeshBuilder
 *
 *
 *
 */
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

	/*
static std::map<std::string, FontMeshBuilder> fontBuilders;

public:

	static FontMeshBuilder& getFontMeshBuilder(std::string &pathToLayout, int atlasWidth, int atlasHeight)
	{
		std::map<std::string, FontMeshBuilder>::iterator it = fontBuilders.find(pathToLayout);

		if (it == fontBuilders.end()) {
			fontBuilders.insert(std::make_pair(pathToLayout, FontMeshBuilder{pathToLayout, atlasWidth, atlasHeight}));
		}

		return fontBuilders.at(pathToLayout);
	}
*/

	/**
	 *
	 */
	std::shared_ptr<mesh::MeshElement> buldMeshForString(const std::string &str, float height);

	/**
	 *
	 */
	float lenghtOfString(const std::string &str, int height);

	/**
	 *
	 */
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
