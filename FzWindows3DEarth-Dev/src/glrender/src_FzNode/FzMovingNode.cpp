#include "../../../include/glRender/FzNode/FzMovingNode.h"
#include "../../../include/GeoMath/CustomMath.h"

const gMatrix3d eus2ned = { 0, 1, 0, 0, 0, -1, -1, 0, 0 };
const gMatrix3d ned2enu = { 0, 1, 0, 1, 0, 0, 0, 0, -1 };
const gMatrix3d enu2ned = { 0, 1, 0, 1, 0, 0, 0, 0, -1 };

FzMovingNode::FzMovingNode(const std::string& _node_id /*= ""*/)
	: FzNode(_node_id)
{
	body2enu_ = eus2ned*ned2enu;
}

FzMovingNode::~FzMovingNode()
{

}

void FzMovingNode::SetProject(MapProjectedEnum _project)
{
	gMatrix3d enu2world;
	if (_project!=projected_)
	{
		switch (_project)
		{
		case UnKnowPrject:
			break;
		case Sphere:
			center_world_pos_ = LBHToEcr(center_ground_pos_);
			enu2world[2] = center_world_pos_.GetNormalize();
			enu2world[0] = gVector3d(0, 0, 1).Cross(enu2world[2]).GetNormalize();
			enu2world[1] = enu2world[2].Cross(enu2world[0]);
			break;
		case Mercator:
			center_world_pos_ = LBHToMercator(center_ground_pos_);
			enu2world[0] = gVector3d(0, 1, 0);
			enu2world[1] = gVector3d(0, 0, 1);
			enu2world[2] = gVector3d(1, 0, 0);
			break;
		default:
			break;
		}
	}
	enu2world_ = enu2world;
	FzNode::SetProject(_project);
}

void FzMovingNode::SetGroundPos(const rLBH _position)
{
	gMatrix3d enu2world;
	center_ground_pos_ = _position;
	switch (projected_)
	{
	case UnKnowPrject:
		break;
	case Sphere:
		center_world_pos_ = LBHToEcr(center_ground_pos_);
		enu2world[2] = center_world_pos_.GetNormalize();
		enu2world[0] = gVector3d(0, 0, 1).Cross(enu2world[2]).GetNormalize();
		enu2world[1] = enu2world[2].Cross(enu2world[0]);
		break;
	case Mercator:
		center_world_pos_ = LBHToMercator(center_ground_pos_);
		enu2world[0] = gVector3d(0, 1, 0);
		enu2world[1] = gVector3d(0, 0, 1);
		enu2world[2] = gVector3d(1, 0, 0);
		break;
	default:
		break;
	}
	enu2world_ = enu2world;
}

void FzMovingNode::SetAttitude(const rAttitude _att)
{
	att_ = _att;
	body2enu_ = eus2ned*Matrix3RotateX(att_.roll)*Matrix3RotateY(att_.pitch)*Matrix3RotateZ(att_.yaw)* ned2enu;
}
