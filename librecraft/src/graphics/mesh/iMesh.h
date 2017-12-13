
#ifndef SRC_GRAPHICS_MESH_IMESH_H_
#define SRC_GRAPHICS_MESH_IMESH_H_

namespace mesh {

class IMesh {
public:
  virtual ~IMesh() {
  }

  virtual void draw() = 0;
};
}

#endif /* SRC_GRAPHICS_MESH_IMESH_H_ */
