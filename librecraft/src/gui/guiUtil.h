
#ifndef SRC_GUI_GUIUTIL_H_
#define SRC_GUI_GUIUTIL_H_

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace gui
{

static glm::mat4 createVirtualToScreen(int virtualWidth, int virtualHeight, int realWidth, int realHeight)
{
	float matrix[16];

	float vW = virtualWidth;
	float vH = virtualHeight;

	float requiredXScale = realWidth / vW;
	float requiredYScale = realHeight / vH;

	float scale;
	float xOffset;
	float yOffset;

	if(requiredXScale < requiredYScale){
		scale = requiredXScale;
		yOffset = (realHeight - vH * scale) / 2;
		xOffset = 0;
	} else {
		scale = requiredYScale;
		xOffset = (realWidth - vW * scale) / 2;
		yOffset = 0;
	}

	matrix[0] = scale;
	matrix[4] = 0;
	matrix[8] = 0;
	matrix[12] = xOffset;

	matrix[1] = 0;
	matrix[5] = scale;
	matrix[9] = 0;
	matrix[13] = yOffset;

	matrix[2] = 0;
	matrix[6] = 0;
	matrix[10] = 1;
	matrix[14] = 0;

	matrix[3] = 0;
	matrix[7] = 0;
	matrix[11] = 0;
	matrix[15] = 1;

	return glm::make_mat4(matrix);
}

static glm::vec2 adjustMouse(int virtualWidth, int virtualHeight, int realWidth, int realHeight, int mouseX, int mouseY)
{
	float mouseOffX = 0;
	float mouseOffY = 0;
	float mouseScale = 1;

	float vW = virtualWidth;
	float vH = virtualHeight;

	float requiredXScale = realWidth / vW;
	float requiredYScale = realHeight / vH;

	if(requiredXScale < requiredYScale){
		mouseScale = requiredXScale;
		mouseOffY = (realHeight - virtualHeight * mouseScale) / 2.0f;
		mouseOffX = 0;
	} else {
		mouseScale = requiredYScale;
		mouseOffX = (realWidth - virtualWidth * mouseScale) / 2.0f;
		mouseOffY = 0;
	}
	float mouseXAdjusted = (mouseX - mouseOffX) / mouseScale;
	float mouseYAdjusted = (mouseY - mouseOffY) / mouseScale;

	return glm::vec2(mouseXAdjusted, mouseYAdjusted);
}


}

#endif /* SRC_GUI_GUIUTIL_H_ */
