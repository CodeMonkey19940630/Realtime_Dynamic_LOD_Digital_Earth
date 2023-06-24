//
// BoundingBox.h
//

#pragma once

#include "CustomMath.h"
#include "uniHead.h"

class CBoundingBox
{
public:
	CBoundingBox();

	CBoundingBox(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 v4, glm::vec3 v5, glm::vec3 v6, glm::vec3 v7){
		corners[0] = v0;
		corners[1] = v1;
		corners[2] = v2;
		corners[3] = v3;
		corners[4] = v4;
		corners[5] = v5;
		corners[6] = v6;
		corners[7] = v7;
	}

	CBoundingBox( float south, float north, float west, float east, float radius1, float radius2){
		float scale = radius2 / radius1;
		corners[0] = SphericalToCartesian(south, west, radius1);
		D3DXVec3Scale(&corners[1], &corners[0], scale);
		corners[2] = SphericalToCartesian(south, east, radius1);
		D3DXVec3Scale(&corners[3], &corners[2], scale);
		corners[4] = SphericalToCartesian(north, west, radius1);
		D3DXVec3Scale(&corners[5], &corners[4], scale);
		corners[6] = SphericalToCartesian(north, east, radius1);
		D3DXVec3Scale(&corners[7], &corners[6], scale);
	}

	~CBoundingBox(){};

	glm::vec3 corners[8];

	glm::vec3 CalculateCenter(){
		glm::vec3 res(0.0f, 0.0f, 0.0f);
		for (int i=0; i<8; i++)
		{
			res += corners[i];
		}

		D3DXVec3Scale(&res, &res, (float)(1.0f / 8.0));

		return res;
	}

	void Get3DBox(float south, float north, float west, float east, float radius1, float radius2) {
		float scale = radius2 / radius1;
		corners[0] = SphericalToCartesian(south, west, radius1);
		D3DXVec3Scale(&corners[1], &corners[0], scale);
		corners[2] = SphericalToCartesian(south, east, radius1);
		D3DXVec3Scale(&corners[3], &corners[2], scale);
		corners[4] = SphericalToCartesian(north, west, radius1);
		D3DXVec3Scale(&corners[5], &corners[4], scale);
		corners[6] = SphericalToCartesian(north, east, radius1);
		D3DXVec3Scale(&corners[7], &corners[6], scale);
	}

	void Get2DBox(float south, float north, float west, float east, float radius1, float radius2) {
		float scale = radius2 / radius1;
		corners[0] = SphericalToMercator(south, west, radius1);
		D3DXVec3Scale(&corners[1], &corners[0], scale);
		corners[2] = SphericalToMercator(south, east, radius1);
		D3DXVec3Scale(&corners[3], &corners[2], scale);
		corners[4] = SphericalToMercator(north, west, radius1);
		D3DXVec3Scale(&corners[5], &corners[4], scale);
		corners[6] = SphericalToMercator(north, east, radius1);
		D3DXVec3Scale(&corners[7], &corners[6], scale);
	}
};
