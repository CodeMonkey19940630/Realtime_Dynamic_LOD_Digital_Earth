//
// frustum.h
//

#pragma once

#include "BoundingSphere.hpp"
#include "BoundingBox.hpp"

class CFrustum
{
public:
	CFrustum() {};
	~CFrustum() {};

	D3DXPLANE planes[6];

	void Update(glm::mat4 m){
		//bottom (down) plane
		planes[0].a = m[0][3] + m[0][1]; //a
		planes[0].b = m[1][3] + m[1][1]; //b
		planes[0].c = m[2][3] + m[2][1]; //c
		planes[0].d = m[3][3] + m[3][1]; //d

										 //far plane
		planes[1].a = m[0][3] - m[0][2];
		planes[1].b = m[1][3] - m[1][2];
		planes[1].c = m[2][3] - m[2][2];
		planes[1].d = m[3][3] - m[3][2];

		//right side plane
		planes[2].a = m[0][3] - m[0][0]; //a
		planes[2].b = m[1][3] - m[1][0]; //b
		planes[2].c = m[2][3] - m[2][0]; //c
		planes[2].d = m[3][3] - m[3][0]; //d

										 //left side plane
		planes[3].a = m[0][3] + m[0][0]; //a
		planes[3].b = m[1][3] + m[1][0]; //b
		planes[3].c = m[2][3] + m[2][0]; //c
		planes[3].d = m[3][3] + m[3][0]; //d

										 //near plane
		planes[4].a = m[0][2];
		planes[4].b = m[1][2];
		planes[4].c = m[2][2];
		planes[4].d = m[3][2];

		//top (up) plane
		planes[5].a = m[0][3] - m[0][1]; //a
		planes[5].b = m[1][3] - m[1][1]; //b
		planes[5].c = m[2][3] - m[2][1]; //c
		planes[5].d = m[3][3] - m[3][1]; //d

		for (int i = 0; i < 6; i++)
		{
			D3DXPlaneNormalize(&planes[i], &planes[i]);
		}

	}

	bool Intersects(CBoundingSphere c)
	{
		for (int i = 0; i < 6; i++)
		{
			float distancePlaneToPoint = planes[i].a * c.Center.x + planes[i].b * c.Center.y + planes[i].c * c.Center.z + planes[i].d;
			if (distancePlaneToPoint < -c.Radius)
				// More than 1 radius outside the plane = outside
				return false;
		}

		//else it's in view
		return true;
	}

	bool ContainsPoint(glm::vec3 v)
	{
		for (int i = 0; i < 6; i++)
		{
			glm::vec3 vec = glm::vec3(planes[i].a, planes[i].b, planes[i].c);
			if (glm::dot(vec, v) + planes[i].d < 0)
				return false;
		}

		return true;
	}

	bool Contains(CBoundingBox bb)
	{
		//
		int iTotalIn = 0;
		for (int i = 0; i < 6; i++)
		{
			int iInCount = 8;
			int iPtIn = 1;
			// TODO: Modify bounding box and only check 2 corners.
			for (int i = 0; i < 8; i++)
			{
				glm::vec3 vec = glm::vec3(planes[i].a, planes[i].b, planes[i].c);
				if (glm::dot(vec, bb.corners[i]) + planes[i].d < 0)
				{
					iPtIn = 0;
					--iInCount;
				}
			}

			if (iInCount == 0)
				return false;

			iTotalIn += iPtIn;
		}

		if (iTotalIn == 6)
			return true;

		return false;
	}

	bool Intersects(CBoundingBox bb)
	{
		for (int i = 0; i < 6; i++)
		{
			glm::vec3 v(planes[i].a, planes[i].b, planes[i].c);
			bool isInside = false;
			// TODO: Modify bounding box and only check 2 corners.
			for (int j = 0; j < 8; j++)
			{
				if (glm::dot(v, bb.corners[j]) + planes[i].d >= 0)
				{
					isInside = true;
					break;
				}
			}

			if (!isInside)
				return false;
		}

		return true;
	}

};
