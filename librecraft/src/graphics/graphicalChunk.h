#ifndef SRC_GRAPHICS_GRAPHICALCHUNK_H_
#define SRC_GRAPHICS_GRAPHICALCHUNK_H_

#include <memory>
#include <vector>

#include "mesh/meshElement.h"
#include "transform.h"

#include "../config/data.h"
#include "../util/voxel.h"

namespace graphics {

class GraphicalChunk {
public:

// ########################################################
// Constructor/Destructor #################################
// ########################################################

	GraphicalChunk(float x, float y, float z,
			std::vector<std::vector<std::vector<Voxel>>>&data,
			std::vector<std::vector<std::vector<Voxel>>> *right,
			std::vector<std::vector<std::vector<Voxel>>> *left,
			std::vector<std::vector<std::vector<Voxel>>> *back,
			std::vector<std::vector<std::vector<Voxel>>> *front);

	virtual ~GraphicalChunk() {};

// ########################################################
// Member Functions########################################
// ########################################################

	void draw();

	Transform& getTransform();

	float getxLocation();

	float getyLocation();

// ########################################################
// Implementation #########################################
// ########################################################

private:

	struct CubeFaceData {
		int id;
		bool vissible, front, back, left, right, top, bottom;
		char lightValue;

		// lv means lightValue
		float lvFront_BottomLeft, lvFront_BottomRight,
				lvFront_TopRight, lvFront_TopLeft;

		float lvBack_BottomLeft, lvBack_BottomRight,
				lvBack_TopRight, lvBack_TopLeft;

		float lvLeft_BottomLeft, lvLeft_BottomRight,
				lvLeft_TopRight, lvLeft_TopLeft;

		float lvRight_BottomLeft, lvRight_BottomRight,
				lvRight_TopRight, lvRight_TopLeft;

		float lvTop_BottomLeft, lvTop_BottomRight,
				lvTop_TopRight, lvTop_TopLeft;

		float lvBottom_BottomLeft, lvBottom_BottomRight,
				lvBottom_TopRight, lvBottom_TopLeft;
	};

	Voxel* getVoxel(int x, int y, int z,
		std::vector<std::vector<std::vector<CubeFaceData>>> &faceData);

	void doAORight(CubeFaceData &cf, int x, int y, int z,
		std::vector<std::vector<std::vector<CubeFaceData>>> &faceData);

	void doAOLeft(CubeFaceData &cf, int x, int y, int z,
		std::vector<std::vector<std::vector<CubeFaceData>>> &faceData);

	void doAOBack(CubeFaceData &cf, int x, int y, int z,
		std::vector<std::vector<std::vector<CubeFaceData>>> &faceData);

	void doAOFront(CubeFaceData &cf, int x, int y, int z,
		std::vector<std::vector<std::vector<CubeFaceData>>> &faceData);

	void doAOTop(CubeFaceData &cf, int x, int y, int z,
		std::vector<std::vector<std::vector<CubeFaceData>>> &faceData);

	void doAOBottom(CubeFaceData &cf, int x, int y, int z,
		std::vector<std::vector<std::vector<CubeFaceData>>> &faceData);


	Voxel* getVoxel(int x, int y, int z,
		std::vector<std::vector<std::vector<Voxel>>> &data,
		std::vector<std::vector<std::vector<Voxel>>> *right,
		std::vector<std::vector<std::vector<Voxel>>> *left,
		std::vector<std::vector<std::vector<Voxel>>> *back,
		std::vector<std::vector<std::vector<Voxel>>> *front);

//	void doAORight(CubeFaceData &cf, int x, int y, int z,
//		std::vector<std::vector<std::vector<Voxel>>> &data,
//		std::vector<std::vector<std::vector<Voxel>>> *right,
//		std::vector<std::vector<std::vector<Voxel>>> *left,
//		std::vector<std::vector<std::vector<Voxel>>> *back,
//		std::vector<std::vector<std::vector<Voxel>>> *front);
//
//	void doAOLeft(CubeFaceData &cf, int x, int y, int z,
//		std::vector<std::vector<std::vector<Voxel>>> &data,
//		std::vector<std::vector<std::vector<Voxel>>> *right,
//		std::vector<std::vector<std::vector<Voxel>>> *left,
//		std::vector<std::vector<std::vector<Voxel>>> *back,
//		std::vector<std::vector<std::vector<Voxel>>> *front);
//
//	void doAOBack(CubeFaceData &cf, int x, int y, int z,
//		std::vector<std::vector<std::vector<Voxel>>> &data,
//		std::vector<std::vector<std::vector<Voxel>>> *right,
//		std::vector<std::vector<std::vector<Voxel>>> *left,
//		std::vector<std::vector<std::vector<Voxel>>> *back,
//		std::vector<std::vector<std::vector<Voxel>>> *front);
//
//	void doAOFront(CubeFaceData &cf, int x, int y, int z,
//		std::vector<std::vector<std::vector<Voxel>>> &data,
//		std::vector<std::vector<std::vector<Voxel>>> *right,
//		std::vector<std::vector<std::vector<Voxel>>> *left,
//		std::vector<std::vector<std::vector<Voxel>>> *back,
//		std::vector<std::vector<std::vector<Voxel>>> *front);
//
//	void doAOTop(CubeFaceData &cf, int x, int y, int z,
//		std::vector<std::vector<std::vector<Voxel>>> &data,
//		std::vector<std::vector<std::vector<Voxel>>> *right,
//		std::vector<std::vector<std::vector<Voxel>>> *left,
//		std::vector<std::vector<std::vector<Voxel>>> *back,
//		std::vector<std::vector<std::vector<Voxel>>> *front);
//
//	void doAOBottom(CubeFaceData &cf, int x, int y, int z,
//		std::vector<std::vector<std::vector<Voxel>>> &data,
//		std::vector<std::vector<std::vector<Voxel>>> *right,
//		std::vector<std::vector<std::vector<Voxel>>> *left,
//		std::vector<std::vector<std::vector<Voxel>>> *back,
//		std::vector<std::vector<std::vector<Voxel>>> *front);


	void computeAverageRight(int lightValue, int x, int y, int z, float &bottomLeft,
			float &bottomRight, float &topRight, float &topLeft,
			std::vector<std::vector<std::vector<CubeFaceData>>> &faceData);

	void computeAverageLeft(int x, int y, int z,float &bottomLeft,
		float &bottomRight, float &topRight, float &topLeft,
		std::vector<std::vector<std::vector<Voxel>>> &data,
		std::vector<std::vector<std::vector<Voxel>>> *right,
		std::vector<std::vector<std::vector<Voxel>>> *left,
		std::vector<std::vector<std::vector<Voxel>>> *back,
		std::vector<std::vector<std::vector<Voxel>>> *front,
		std::vector<std::vector<std::vector<CubeFaceData>>> &faceData);

	void computeAverageTop(int x, int y, int z,float &bottomLeft,
		float &bottomRight, float &topRight, float &topLeft,
		std::vector<std::vector<std::vector<Voxel>>> &data,
		std::vector<std::vector<std::vector<Voxel>>> *right,
		std::vector<std::vector<std::vector<Voxel>>> *left,
		std::vector<std::vector<std::vector<Voxel>>> *back,
		std::vector<std::vector<std::vector<Voxel>>> *front,
		std::vector<std::vector<std::vector<CubeFaceData>>> &faceData);

	void computeAverageBottom(int x, int y, int z,float &bottomLeft,
		float &bottomRight, float &topRight, float &topLeft,
		std::vector<std::vector<std::vector<Voxel>>> &data,
		std::vector<std::vector<std::vector<Voxel>>> *right,
		std::vector<std::vector<std::vector<Voxel>>> *left,
		std::vector<std::vector<std::vector<Voxel>>> *back,
		std::vector<std::vector<std::vector<Voxel>>> *front,
		std::vector<std::vector<std::vector<CubeFaceData>>> &faceData);

	void computeAverageBack(int x, int y, int z,float &bottomLeft,
		float &bottomRight, float &topRight, float &topLeft,
		std::vector<std::vector<std::vector<Voxel>>> &data,
		std::vector<std::vector<std::vector<Voxel>>> *right,
		std::vector<std::vector<std::vector<Voxel>>> *left,
		std::vector<std::vector<std::vector<Voxel>>> *back,
		std::vector<std::vector<std::vector<Voxel>>> *front,
		std::vector<std::vector<std::vector<CubeFaceData>>> &faceData);

	void computeAverageFront(int x, int y, int z,float &bottomLeft,
		float &bottomRight, float &topRight, float &topLeft,
		std::vector<std::vector<std::vector<Voxel>>> &data,
		std::vector<std::vector<std::vector<Voxel>>> *right,
		std::vector<std::vector<std::vector<Voxel>>> *left,
		std::vector<std::vector<std::vector<Voxel>>> *back,
		std::vector<std::vector<std::vector<Voxel>>> *front,
		std::vector<std::vector<std::vector<CubeFaceData>>> &faceData);

	std::unique_ptr<mesh::MeshElement> mesh;
	float xLocation;
	float yLocation;
	float zLocation;

	int width = config::chunk_data::GRAPHICAL_CHUNK_WIDTH;
	int height = config::chunk_data::GRAPHICAL_CHUNK_HEIGHT;
	int depth = config::chunk_data::GRAPHICAL_CHUNK_DEPTH;

	Transform transform;
};

} /* graphics namespace */

#endif /* SRC_GRAPHICS_GRAPHICALCHUNK_H_ */
