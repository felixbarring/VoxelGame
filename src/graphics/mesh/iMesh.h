
#ifndef SRC_GRAPHICS_MESH_IMESH_H_
#define SRC_GRAPHICS_MESH_IMESH_H_

class IMesh {
public:
	virtual ~IMesh() {}

	virtual void render() = 0;

};

#endif /* SRC_GRAPHICS_MESH_IMESH_H_ */
