#pragma once
#include "../../../include/glRender/EarthBasics/FzSituationSystem.h"
#include "RoamCamera.h"
class SituationSystem :
	public FzSituationSystem
{
public:
	SituationSystem();
	~SituationSystem();

	virtual FzCamera* Camera() override;


	virtual FzLighting* Lighting() override;


	virtual MapProjectedEnum getMapProjected() override;


	virtual gMatrix4d getProjectionMatrix4() override;


	virtual bool getWidgetPosFromWorldPos(const gVector3d& _pos, gVector2f& _out);
public:
	RoamCamera	camera_;
	FzLighting	lighting_;
	MapProjectedEnum projected_;
	gMatrix4d	projection_;
	gVector2f	widget_size_;

	double fovy_;
	dLBH surface_point_;
	double d_heading_, d_tilt_ , m_distance_;
};

