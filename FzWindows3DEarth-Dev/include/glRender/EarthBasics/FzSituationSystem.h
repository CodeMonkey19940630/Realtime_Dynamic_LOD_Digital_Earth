#pragma once
#include "FzCamera.h"
#include "FzLighting.h"
class FzSituationSystem
{
public:
	FzSituationSystem() 
	{
	}

	virtual ~FzSituationSystem() {};

	virtual FzCamera* Camera() = 0;

	virtual FzLighting* Lighting() = 0;

	virtual MapProjectedEnum getMapProjected() = 0;

	virtual gMatrix4d getProjectionMatrix4() = 0;

	virtual bool getWidgetPosFromWorldPos(const gVector3d& _pos, gVector2f& _out) = 0;
};