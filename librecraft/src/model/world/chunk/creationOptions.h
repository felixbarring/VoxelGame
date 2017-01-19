/*
 * creationOptions.h
 *
 *  Created on: Jan 15, 2017
 *      Author: felix
 */

#ifndef SRC_MODEL_WORLD_CHUNK_CREATIONOPTIONS_H_
#define SRC_MODEL_WORLD_CHUNK_CREATIONOPTIONS_H_

#include <string>

namespace chunk {

/**
 *
 */
class CreationOptions {
public:

    CreationOptions()
    {
    }

    void setName(std::string value) { m_name = std::move(value); }

    std::string& getName() { return m_name; }

    void setFlat(bool value) { m_flat = value; }

    bool getFlat() const { return m_flat; }

    void setDifferencBubesForEachChunk(bool value) { m_differentCubeForEachChunk = value; }

    bool differentCubesForEachChunk() { return m_differentCubeForEachChunk; }

private:

    std::string m_name{"Error Name"};
    bool m_flat{false};
    bool m_differentCubeForEachChunk{false};

};

}


#endif /* SRC_MODEL_WORLD_CHUNK_CREATIONOPTIONS_H_ */
