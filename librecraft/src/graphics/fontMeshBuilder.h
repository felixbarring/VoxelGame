
#ifndef SRC_GRAPHICS_FONTMESHBUILDER_H_
#define SRC_GRAPHICS_FONTMESHBUILDER_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "graphics/mesh/meshElement.h"

#include "config/data.h"

namespace graphics {

/**
 * @brief FontMeshBuilder used to build meshes that can be used for rendering
 *        text.
 */
class FontMeshBuilder {
public:
  FontMeshBuilder(std::string& pathToLayout, int atlasWidth, int atlasHeight);

  /**
   *
   */
  std::unique_ptr<mesh::MeshElement> buldMeshForString(const std::string& str,
                                                       float height);

  /**
   *
   */
  float lenghtOfString(const std::string& str, float height);

  // Returns -1 if num is greater than the string's length
  /**
   *
   */
  float lenghtOfStringAtChar(const std::string& str, float height, int num);

  /**
   * @brief
   *
   * @param str The string that will be checked onW
   * @param height The height of the string's char's. Used to compute the
   *        lenght
   * @param maxLength The lenght that the string should not be bigger than
   * @return The position in the string at where the string should be
   * 	       split in order to not go outside maxLength. Returns -1 if
   * 	       no split is necessary
   */
  int splitStringAt(const std::string& str, int height, float maxLength);

private:
  void printCharData();

  struct CharData {
    int width;
    int height;

    int xPosition;
    int yPosition;
  };

  std::vector<CharData> charData =
    std::vector<CharData>(config::font_data::charsInFont);
  const int m_atlasWidth;
  const int m_atlasHeight;
};

} /* namespace graphics */

#endif /* SRC_GRAPHICS_FONTMESHBUILDER_H_ */
