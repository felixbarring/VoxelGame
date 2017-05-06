#ifndef SRC_MODEL_WORLD_TERRAINGEN_NOISEMIXER_H_
#define SRC_MODEL_WORLD_TERRAINGEN_NOISEMIXER_H_

#include <glm/gtc/noise.hpp>

namespace terrainGen {

/**
 *
 */
class NoiseMixer {
public:

  /**
   * This will be used in the computeNoise function. For each noise added,
   * there parts will be summed together to produce the final result.
   *
   * @param frequency
   * @param amplitude
   */
  void addNoise(float frequency, float amplitude) {
    m_noises.push_back(Noise{frequency, amplitude});
  }

  /**
   * @brief Generates noise
   *
   * @param x
   * @param y
   * @return
   */
  int computeNoise(float x, float z) {
    float value = 10;
    for (auto noise : m_noises) {
      value += noise.m_amplitude * (1.0f + glm::simplex(
          glm::vec3(x / noise.m_frequency, z / noise.m_frequency, 0.5f)));
    }
    return value;
  }

private:

  struct Noise {
    Noise(float freq, float amp)
      : m_frequency(freq), m_amplitude(amp)
    {
    }

    float m_frequency;
    float m_amplitude;
  };

  std::vector<Noise> m_noises;
};

} /* namespace terrainGen */

#endif /* SRC_MODEL_WORLD_TERRAINGEN_NOISEMIXER_H_ */
