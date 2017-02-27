#define GLM_FORCE_RADIANS

#include <vector>

#include "texturedCube.h"
#include "transform.h"

#include "../config/data.h"

using namespace std;
using namespace config::cube_data;

namespace graphics
{

TexturedCube::TexturedCube(float xOffset, float yOffset, float zOffset, int id):
    xLocation{xOffset},
    yLocation{yOffset},
    zLocation{zOffset}
{

    float size = 0.51f;

    vector<GLfloat> vertexData {
        // Front
        -size, -size, size, // 0
        size, -size, size, // 1
        size, size, size, // 2
        -size, size, size, // 3

        // Back
        size, -size, -size, // 0
        -size, -size, -size, // 1
        -size, size, -size, // 2
        size, size, -size, // 3

        // Right
        size, -size, size, // 0
        size, -size, -size, // 1
        size, size, -size, // 2
        size, size, size, // 3

        // Left
        -size, -size, -size, // 0
        -size, -size, size, // 1
        -size, size, size, // 2
        -size, size,-size, // 3

        // Top
        -size, size, size, // 0
        size, size, size, // 1
        size, size, -size, // 2
        -size, size, -size, // 3

        // Bottom
        -size, -size, -size, // 0
        size, -size, -size, // 1
        size, -size, size, // 2
        -size, -size, size, // 3

    };

    vector<GLfloat> normals {
        // Normals
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,

        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,

        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,

        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,

        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
    };

    GLfloat sideTexture = BLOCK_TEXTURES[id][SIDE_TEXTURE];
    GLfloat topTexture = BLOCK_TEXTURES[id][TOP_TEXTURE];
    GLfloat bottomTexture = BLOCK_TEXTURES[id][BOTTOM_TEXTURE];

    vector<GLfloat> UV {
        // Front
        0.0f, 0.0f, sideTexture,
        1.0f, 0.0f, sideTexture,
        1.0f, 1.0f, sideTexture,
        0.0f, 1.0f, sideTexture,

        // Back
        0.0f, 0.0f, sideTexture,
        1.0f, 0.0f, sideTexture,
        1.0f, 1.0f, sideTexture,
        0.0f, 1.0f, sideTexture,

        // Right
        0.0f, 0.0f, sideTexture,
        1.0f, 0.0f, sideTexture,
        1.0f, 1.0f, sideTexture,
        0.0f, 1.0f, sideTexture,

        // Left
        0.0f, 0.0f, sideTexture,
        1.0f, 0.0f, sideTexture,
        1.0f, 1.0f, sideTexture,
        0.0f, 1.0f, sideTexture,

        // Top
        0.0f, 0.0f, topTexture,
        1.0f, 0.0f, topTexture,
        1.0f, 1.0f, topTexture,
        0.0f, 1.0f, topTexture,

        // Bottom
        0.0f, 0.0f, bottomTexture,
        1.0f, 0.0f, bottomTexture,
        1.0f, 1.0f, bottomTexture,
        0.0f, 1.0f, bottomTexture,
    };

    vector<short> elementData{
        0, 1, 2, 0, 2, 3,
        0+4, 1+4, 2+4, 0+4, 2+4, 3+4,
        0+8, 1+8, 2+8, 0+8, 2+8, 3+8,
        0+12, 1+12, 2+12, 0+12, 2+12, 3+12,
        0+16, 1+16, 2+16, 0+16, 2+16, 3+16,
        0+20, 1+20, 2+20, 0+20, 2+20, 3+20,
    };

    mesh.reset(new mesh::MeshElement(vertexData, 3, normals, 3, UV, 3, elementData));

}

void TexturedCube::draw() {
	mesh->draw();
}

}

