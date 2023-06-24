//
// BoundingBox.h
//

#pragma once

#include "CustomMath.h"

class CBoundingSphere
{
public:

	CBoundingSphere();

	CBoundingSphere(glm::vec3 center, float radius){
		Center = center;
		Radius = radius;
	}

	CBoundingSphere( float south, float north, float west, float east, float radius1, float radius2){
		// Compute the points in world coordinates
		const int CornerCount = 8;
		glm::vec3 corners[CornerCount];
	
		float scale = radius2 / radius1;
		corners[0] = SphericalToCartesian((float)DEG_TO_RAD(south), (float)DEG_TO_RAD(west), (float)radius1);
		D3DXVec3Scale(&corners[1], &corners[0], scale);
		corners[2] = SphericalToCartesian((float)DEG_TO_RAD(south), (float)DEG_TO_RAD(east), (float)radius1);
		D3DXVec3Scale(&corners[3], &corners[2], scale);
		corners[4] = SphericalToCartesian((float)DEG_TO_RAD(north), (float)DEG_TO_RAD(west), (float)radius1);
		D3DXVec3Scale(&corners[5], &corners[4], scale);
		corners[6] = SphericalToCartesian((float)DEG_TO_RAD(north), (float)DEG_TO_RAD(east), (float)radius1);
		D3DXVec3Scale(&corners[7], &corners[6], scale);
	
		//Find the center.  In this case, we'll simply average the coordinates. 
		for (int i=0; i<CornerCount; i++)
		{
			Center += corners[i];
		}
		
		D3DXVec3Scale(&Center, &Center, (float)(1/CornerCount));
	
		//Loop through the coordinates and find the maximum distance from the center.  This is the radius.		
		for (int i=0; i<CornerCount; i++)
		{
			glm::vec3 vec;
			vec = corners[i] - Center;
			float dist = glm::length(vec);
			if (dist > Radius)
				Radius = dist;
		}
		
	}		

	~CBoundingSphere();

	glm::vec3 Center;
	float Radius;
	
};
