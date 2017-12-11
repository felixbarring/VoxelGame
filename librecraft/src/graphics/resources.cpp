#include "resources.h"

#include <iostream>

using namespace std;
using namespace texture;

namespace graphics {

Texture&
Resources::getTexture(const string& path)
{
  auto it = textures.find(path);

  if (it == textures.end())
    textures.insert(make_pair(path, Texture{ path.c_str() }));

  return textures.at(path);
}

TextureArray&
Resources::getTextureArray(const vector<string>& textures,
                           unsigned width,
                           unsigned height)
{
  auto it = textureArraysMap.find(textures.at(0));

  if (it == textureArraysMap.end())
    textureArraysMap.insert(
      make_pair(textures.at(0), TextureArray{ textures, width, height }));

  return textureArraysMap.at(textures.at(0));
}

TextureCubeMap&
Resources::getTextureCubeMap(string& right,
                             string& left,
                             string& top,
                             string& bottom,
                             string& back,
                             string& front)
{
  auto it = textureCubeMaps.find(right);

  if (it == textureCubeMaps.end())
    textureCubeMaps.insert(make_pair(right,
                                     TextureCubeMap{ right.c_str(),
                                                     left.c_str(),
                                                     top.c_str(),
                                                     bottom.c_str(),
                                                     back.c_str(),
                                                     front.c_str() }));

  return textureCubeMaps.at(right);
}

FontMeshBuilder&
Resources::getFontMeshBuilder(string& pathToLayout,
                              int atlasWidth,
                              int atlasHeight)
{
  auto it = fontMeshBuilders.find(pathToLayout);

  if (it == fontMeshBuilders.end())
    fontMeshBuilders.insert(make_pair(
      pathToLayout, FontMeshBuilder{ pathToLayout, atlasWidth, atlasHeight }));

  return fontMeshBuilders.at(pathToLayout);
}

} /* namespace graphics */
