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

	virtual ~GraphicalChunk() { };

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
		float lvFront_BottomLeft, lvFront_BottomRight, lvFront_TopRight, lvFront_TopLeft;

		float lvBack_BottomLeft, lvBack_BottomRight, lvBack_TopRight, lvBack_TopLeft;

		float lvLeft_BottomLeft, lvLeft_BottomRight, lvLeft_TopRight, lvLeft_TopLeft;

		float lvRight_BottomLeft, lvRight_BottomRight, lvRight_TopRight, lvRight_TopLeft;

		float lvTop_BottomLeft, lvTop_BottomRight, lvTop_TopRight, lvTop_TopLeft;

		float lvBottom_BottomLeft, lvBottom_BottomRight, lvBottom_TopRight, lvBottom_TopLeft;
	};

	Voxel* getVoxel(int x, int y, int z,
		std::vector<std::vector<std::vector<CubeFaceData>>> &faceData);

	void createMeshData(
			bool transparent,
			const std::vector<std::vector<std::vector<CubeFaceData>>> &faceData,
			std::vector<GLfloat> &vertexData,
			std::vector<GLfloat> &normals,
			std::vector<GLfloat> &UV,
			std::vector<short> &elementData);

	void doAORight(CubeFaceData &cf, int x, int y, int z,
		std::vector<std::vector<std::vector<CubeFaceData>>> &faceData);

	void doAOLeft(CubeFaceData &cf, int x, int y, int z,
		std::vector<std::vector<std::vector<CubeFaceData>>> &faceData);

	void doAOFront(CubeFaceData &cf, int x, int y, int z,
		std::vector<std::vector<std::vector<CubeFaceData>>> &faceData);

	void doAOBack(CubeFaceData &cf, int x, int y, int z,
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

	void computeAverageRight(int lightValue, int x, int y, int z, float &bottomLeft, float &bottomRight,
			float &topRight, float &topLeft, std::vector<std::vector<std::vector<CubeFaceData>>> &faceData);

	void computeAverageLeft(int lightValue, int x, int y, int z, float &bottomLeft, float &bottomRight,
			float &topRight, float &topLeft, std::vector<std::vector<std::vector<CubeFaceData>>> &faceData);

	void computeAverageTop(int lightValue, int x, int y, int z, float &bottomLeft, float &bottomRight,
			float &topRight, float &topLeft, std::vector<std::vector<std::vector<CubeFaceData>>> &faceData);

	void computeAverageBottom(int lightValue, int x, int y, int z, float &bottomLeft, float &bottomRight,
			float &topRight, float &topLeft, std::vector<std::vector<std::vector<CubeFaceData>>> &faceData);

	void computeAverageBack(int lightValue, int x, int y, int z, float &bottomLeft, float &bottomRight,
			float &topRight, float &topLeft, std::vector<std::vector<std::vector<CubeFaceData>>> &faceData);

	void computeAverageFront(int lightValue, int x, int y, int z,
			float &bottomLeft, float &bottomRight,
			float &topRight, float &topLeft,
			std::vector<std::vector<std::vector<CubeFaceData>>> &faceData);

	void computeAverageHelper(int lightValue,
			const CubeFaceData& cLeftLeft,
			const CubeFaceData& cLeftLeft_Right,
			const CubeFaceData&	cBottomLeft,
			const CubeFaceData& cBottomLeft_Right,
			const CubeFaceData& cBottomMiddle,
			const CubeFaceData&	cBottomMiddle_Right,
			const CubeFaceData& cBottomRight,
			const CubeFaceData& cBottomRight_Right,
			const CubeFaceData& cRightRight,
			const CubeFaceData& cRightRight_Right,
			const CubeFaceData& cTopRight,
			const CubeFaceData& cTopRight_Right,
			const CubeFaceData& cTopMiddle,
			const CubeFaceData& cTopMiddle_Right,
			const CubeFaceData& cTopLeft,
			const CubeFaceData& cTopLeft_Right,
			float& bottomLeft,
			float& bottomRight,
			float& topRight,
			float& topLeft);

	std::vector<std::vector<std::vector<CubeFaceData>>> m_faceData;

	std::unique_ptr<mesh::MeshElement> m_mesh;
	std::unique_ptr<mesh::MeshElement> m_waterMesh;

	float m_xLocation;
	float m_yLocation;
	float m_zLocation;

	int m_width = config::chunk_data::GRAPHICAL_CHUNK_WIDTH;
	int m_height = config::chunk_data::GRAPHICAL_CHUNK_HEIGHT;
	int m_depth = config::chunk_data::GRAPHICAL_CHUNK_WIDTH;

	Transform transform;
};

} /* graphics namespace */

#endif /* SRC_GRAPHICS_GRAPHICALCHUNK_H_ */
