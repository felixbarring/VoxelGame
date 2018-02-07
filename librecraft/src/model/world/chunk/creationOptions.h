#ifndef SRC_MODEL_WORLD_CHUNK_CREATIONOPTIONS_H_
#define SRC_MODEL_WORLD_CHUNK_CREATIONOPTIONS_H_

#include <string>

namespace chunk {

/**
 *
 */
class CreationOptions {
public:
  /**
   * @brief Loads the creation options from xml file corresponding to the name.
   * @param name
   */
  CreationOptions(std::string name);

  /**
   * @brief Creates a new creation options according to the arguments.
   *
   * These arguments will be saved to xml file.
   *
   * @param name
   * @param seed
   * @param flat
   * @param different
   */
  CreationOptions(std::string name, int seed, bool flat, bool different);

  std::string& getName();

  int getSeed() const;

  bool getFlat() const;

  bool differentCubesForEachChunk() const;

private:
  std::string m_name{"Error Name"};
  int m_seed{};
  bool m_flat{false};
  bool m_differentCubeForEachChunk{true};
};
}

#endif /* SRC_MODEL_WORLD_CHUNK_CREATIONOPTIONS_H_ */
