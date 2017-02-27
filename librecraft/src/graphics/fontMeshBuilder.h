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
#include <vector>

#include "mesh/meshElement.h"

#include "../config/data.h"

namespace graphics {

/**
 * \class FontMeshBuilder
 */
class FontMeshBuilder {
public:

    FontMeshBuilder(std::string &pathToLayout, int atlasWidth, int atlasHeight);

    /**
     *
     */
    std::shared_ptr<mesh::MeshElement> buldMeshForString(const std::string &str,
            float height);

    /**
     *
     */
    float lenghtOfString(const std::string &str, float height);

    // Returns -1 if num is greater than the string's length
    /**
     *
     */
    float lenghtOfStringAtChar(const std::string &str, float height, int num);

    /**
     * \brief
     *
     * @param str The string that will be checked onW
     * @param height The height of the string's char's. Used to compute the
     *        lenght
     * @param maxLength The lenght that the string should not be bigger than
     * @return The position in the string at where the string should be
     * 	       split in order to not go outside maxLength. Returns -1 if
     * 	       no split is necessary
     */
    int splitStringAt(const std::string &str, int height, float maxLength);

private:

    void printCharData();

    struct CharData {
        int width;
        int height;

        int xPosition;
        int yPosition;
    };

    std::vector<CharData> charData  = std::vector<CharData>(config::font_data::charsInFont);
    const int ATLAS_WIDTH;
    const int ATLAS_HEIGHT;

};

} /* namespace graphics */

#endif /* SRC_GRAPHICS_FONTMESHBUILDER_H_ */
