#include "SituationSystem.h"



SituationSystem::SituationSystem()
{
	fovy_ = M_PI_4;
	surface_point_= dLBH(0, 0, 0);
	d_heading_ = 0;
	d_tilt_ = 0;
	m_distance_ = 10000000;
	
}


SituationSystem::~SituationSystem()
{
}

FzCamera* SituationSystem::Camera()
{
	return &camera_;
}

FzLighting* SituationSystem::Lighting()
{
	return &lighting_;
}

MapProjectedEnum SituationSystem::getMapProjected()
{
	return projected_;
}

gMatrix4d SituationSystem::getProjectionMatrix4()
{
	return projection_;
}

bool SituationSystem::getWidgetPosFromWorldPos(const gVector3d& _pos, gVector2f& _out)
{
	gVector4d out_p((_pos - camera_.getCameraPosition())*camera_.getViewMatrix3(),1);
	//µÈ¼ÛÓÚgVector4d out_p = gVector4d(_pos,1)*camera_.getGC2View();
	out_p = out_p * projection_;
	out_p /= out_p.w;
	if (abs(out_p.x)>1 || abs(out_p.y)>1 || abs(out_p.z - 0.5)>0.5)
		return false;
	else
	{
		_out.x = widget_size_.x*(out_p.x / 2 + 0.5);
		_out.y = widget_size_.y*(0.5 - out_p.y / 2);
		return true;
	}
}
