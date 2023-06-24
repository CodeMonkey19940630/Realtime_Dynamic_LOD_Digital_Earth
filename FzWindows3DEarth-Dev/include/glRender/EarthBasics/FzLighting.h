#pragma once
#include "../../geomath/GeoMathHeader.h"

class FzLighting
{
public:
	FzLighting() {};
	virtual ~FzLighting() {};
public:
	gVector3d sunLightVecEcr;
};
