#include "meshElement.h"

#include <iostream>

namespace mesh {

// TODO This class should be cleaned up, and the code duplication should be
// possible to avoid by using templates?

MeshElement::MeshElement(
    const std::vector<float> &vbd1, int vbd1Format,
    const std::vector<short> &elementData)
{

    m_numberOfElements = elementData.size();
    const static int FLOAT_SIZE = (sizeof(float));

    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    GLuint vbo1;
    glGenBuffers(1, &vbo1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo1);
    glBufferData(GL_ARRAY_BUFFER, vbd1.size() * FLOAT_SIZE, &vbd1[0], GL_STATIC_DRAW);
    m_vboHandels.push_back(vbo1);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, vbd1Format, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    glDisableVertexAttribArray(0);

    glGenBuffers(1, &m_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementData.size() * 2, &elementData[0], GL_STATIC_DRAW);

    glBindVertexArray(0);
}

MeshElement::MeshElement(
    const std::vector<float> &vbd1, int vbd1Format,
    const std::vector<float> &vbd2, int vbd2Format,
    const std::vector<short> &elementData)
{

    m_numberOfElements = elementData.size();
    const static int FLOAT_SIZE = (sizeof(float));

    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    GLuint vbo1;
    glGenBuffers(1, &vbo1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo1);
    glBufferData(GL_ARRAY_BUFFER, vbd1.size() * FLOAT_SIZE, &vbd1[0], GL_STATIC_DRAW);
    m_vboHandels.push_back(vbo1);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, vbd1Format, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    glDisableVertexAttribArray(0);

    GLuint vbo2;
    glGenBuffers(1, &vbo2);
    glBindBuffer(GL_ARRAY_BUFFER, vbo2);
    glBufferData(GL_ARRAY_BUFFER, vbd2.size() * FLOAT_SIZE, &vbd2[0], GL_STATIC_DRAW);
    m_vboHandels.push_back(vbo2);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, vbd2Format, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    glDisableVertexAttribArray(1);

    glGenBuffers(1, &m_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementData.size() * 2, &elementData[0], GL_STATIC_DRAW);

    glBindVertexArray(0);
}

MeshElement::MeshElement(
    const std::vector<float> &vbd1, int vbd1Format,
    const std::vector<float> &vbd2, int vbd2Format,
    const std::vector<float> &vbd3, int vbd3Format,
    const std::vector<short> &elementData)
{
    m_numberOfElements = elementData.size();
    const static int FLOAT_SIZE = (sizeof(float));

    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    GLuint vbo1;
    glGenBuffers(1, &vbo1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo1);
    glBufferData(GL_ARRAY_BUFFER, vbd1.size() * FLOAT_SIZE, &vbd1[0], GL_STATIC_DRAW);
    m_vboHandels.push_back(vbo1);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, vbd1Format, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    glDisableVertexAttribArray(0);

    GLuint vbo2;
    glGenBuffers(1, &vbo2);
    glBindBuffer(GL_ARRAY_BUFFER, vbo2);
    glBufferData(GL_ARRAY_BUFFER, vbd2.size() * FLOAT_SIZE, &vbd2[0], GL_STATIC_DRAW);
    m_vboHandels.push_back(vbo2);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, vbd2Format, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    glDisableVertexAttribArray(1);

    GLuint vbo3;
    glGenBuffers(1, &vbo3);
    glBindBuffer(GL_ARRAY_BUFFER, vbo3);
    glBufferData(GL_ARRAY_BUFFER, vbd3.size() * FLOAT_SIZE, &vbd3[0], GL_STATIC_DRAW);
    m_vboHandels.push_back(vbo3);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, vbd3Format, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    glDisableVertexAttribArray(2);

    glGenBuffers(1, &m_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementData.size() * 2, &elementData[0], GL_STATIC_DRAW);

    glBindVertexArray(0);
}

MeshElement::MeshElement(
        const std::vector<float> &vbd1, int vbd1Format,
        const std::vector<float> &vbd2, int vbd2Format,
        const std::vector<float> &vbd3, int vbd3Format,
        const std::vector<float> &vbd4, int vbd4Format,
        const std::vector<short> &elementData)
{
    m_numberOfElements = elementData.size();
    const static int FLOAT_SIZE = (sizeof(float));

    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    GLuint vbo1;
    glGenBuffers(1, &vbo1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo1);
    glBufferData(GL_ARRAY_BUFFER, vbd1.size() * FLOAT_SIZE, &vbd1[0], GL_STATIC_DRAW);
    m_vboHandels.push_back(vbo1);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, vbd1Format, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    glDisableVertexAttribArray(0);

    GLuint vbo2;
    glGenBuffers(1, &vbo2);
    glBindBuffer(GL_ARRAY_BUFFER, vbo2);
    glBufferData(GL_ARRAY_BUFFER, vbd2.size() * FLOAT_SIZE, &vbd2[0], GL_STATIC_DRAW);
    m_vboHandels.push_back(vbo2);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, vbd2Format, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    glDisableVertexAttribArray(1);

    GLuint vbo3;
    glGenBuffers(1, &vbo3);
    glBindBuffer(GL_ARRAY_BUFFER, vbo3);
    glBufferData(GL_ARRAY_BUFFER, vbd3.size() * FLOAT_SIZE, &vbd3[0], GL_STATIC_DRAW);
    m_vboHandels.push_back(vbo3);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, vbd3Format, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    glDisableVertexAttribArray(2);

    GLuint vbo4;
    glGenBuffers(1, &vbo4);
    glBindBuffer(GL_ARRAY_BUFFER, vbo4);
    glBufferData(GL_ARRAY_BUFFER, vbd4.size() * FLOAT_SIZE, &vbd4[0], GL_STATIC_DRAW);
    m_vboHandels.push_back(vbo4);

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, vbd4Format, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    glDisableVertexAttribArray(3);

    glGenBuffers(1, &m_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementData.size() * 2, &elementData[0], GL_STATIC_DRAW);

    glBindVertexArray(0);
}

MeshElement::~MeshElement() {
    glDeleteVertexArrays(1, &m_VAO);
    for (GLuint handel : m_vboHandels)
        glDeleteBuffers(1, &handel);

    glDeleteBuffers(1, &m_EBO);
}

void MeshElement::draw() {
    glBindVertexArray(m_VAO);

    for (unsigned i = 0; i < m_vboHandels.size(); ++i)
        glEnableVertexAttribArray(i);

    glDrawElements(GL_TRIANGLES, m_numberOfElements, GL_UNSIGNED_SHORT, 0);

    for (unsigned i = 0; i < m_vboHandels.size(); ++i)
        glDisableVertexAttribArray(i);

    glBindVertexArray(0);
}

} /* namespace mesh */
