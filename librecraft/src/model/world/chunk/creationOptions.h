#ifndef SRC_MODEL_WORLD_CHUNK_CREATIONOPTIONS_H_
#define SRC_MODEL_WORLD_CHUNK_CREATIONOPTIONS_H_

#include <string>

namespace chunk {

/**
 *
 */
class CreationOptions {
public:

  CreationOptions(std::string name);

  CreationOptions(std::string name, bool flat, bool different);

  std::string& getName();

  bool getFlat() const;

  bool differentCubesForEachChunk();

private:

  std::string m_name{"Error Name"};
  bool m_flat{false};
  bool m_differentCubeForEachChunk{true};
};
}

#endif /* SRC_MODEL_WORLD_CHUNK_CREATIONOPTIONS_H_ */
