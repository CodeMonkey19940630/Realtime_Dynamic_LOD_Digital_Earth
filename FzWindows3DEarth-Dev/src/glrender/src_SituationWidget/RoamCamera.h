#pragma once
#include "../../../include/glRender/EarthBasics/FzCamera.h"
class RoamCamera :
	public FzCamera
{
public:
	RoamCamera();
	~RoamCamera();
	virtual void setCameraPosition(const gVector3d& _view_point, const double& _r_heading, const double& _r_tilt,const double& _m_dis);
};

