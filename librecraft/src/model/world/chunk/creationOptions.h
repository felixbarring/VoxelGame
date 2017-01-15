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

    CreationOptions(std::string name, bool flat)
        : m_name{name},
          m_flat{flat}
    {
    }

    std::string& getName() { return m_name; }

    bool getFlat() const { return m_flat; }

private:

    std::string m_name;
    bool m_flat;

};

}


#endif /* SRC_MODEL_WORLD_CHUNK_CREATIONOPTIONS_H_ */
