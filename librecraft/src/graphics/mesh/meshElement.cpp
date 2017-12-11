#include "meshElement.h"

#include <iostream>

namespace mesh {

// TODO Maybe this class should have a function that returns an attribute map.

MeshElement::MeshElement(std::vector<std::pair<std::vector<float>, int>> vbos,
                         const std::vector<short>& elementData)
{
  m_numberOfElements = elementData.size();
  const static int FLOAT_SIZE = (sizeof(float));
  const static int SHORT_SIZE = (sizeof(short));

  glGenVertexArrays(1, &m_VAO);
  glBindVertexArray(m_VAO);

  int i{ 0 };
  for (auto vbo : vbos) {
    GLuint vboHandle;
    glGenBuffers(1, &vboHandle);
    glBindBuffer(GL_ARRAY_BUFFER, vboHandle);

    std::vector<float> data = std::get<0>(vbo);

    glBufferData(
      GL_ARRAY_BUFFER, data.size() * FLOAT_SIZE, &data[0], GL_STATIC_DRAW);

    m_vboHandels.push_back(vboHandle);

    int vbd1Format = std::get<1>(vbo);

    glEnableVertexAttribArray(i);
    glVertexAttribPointer(i, vbd1Format, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glDisableVertexAttribArray(i);

    ++i;

    glGenBuffers(1, &m_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 elementData.size() * SHORT_SIZE,
                 &elementData[0],
                 GL_STATIC_DRAW);
  }

  glBindVertexArray(0);
}

MeshElement::~MeshElement()
{
  glDeleteVertexArrays(1, &m_VAO);
  for (GLuint handel : m_vboHandels)
    glDeleteBuffers(1, &handel);

  glDeleteBuffers(1, &m_EBO);
}

void
MeshElement::draw()
{
  glBindVertexArray(m_VAO);

  for (unsigned i{ 0 }; i < m_vboHandels.size(); ++i)
    glEnableVertexAttribArray(i);

  glDrawElements(GL_TRIANGLES, m_numberOfElements, GL_UNSIGNED_SHORT, 0);

  for (unsigned i = 0; i < m_vboHandels.size(); ++i)
    glDisableVertexAttribArray(i);

  glBindVertexArray(0);
}

} /* namespace mesh */
