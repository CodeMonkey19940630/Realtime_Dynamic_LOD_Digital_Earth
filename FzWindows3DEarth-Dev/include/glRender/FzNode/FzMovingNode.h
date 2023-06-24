#pragma once
#include "FzNode.h"
#include "../../GeoMath/GeoCoordinateStruct.h"
#include "../../GeoMath/GeoMathematics.h"

class FZNODE_EXPORT FzMovingNode :public FzNode
{
public:
	FzMovingNode(const std::string& _node_id = "");

	virtual ~FzMovingNode();

	virtual void SetProject(MapProjectedEnum _project);

	virtual void SetGroundPos(const rLBH _position);

	const rLBH GetGroundPos() const { return center_ground_pos_; }

	virtual void SetAttitude(const rAttitude _att);

	const rAttitude GetAttitude() const { return att_; }

	const gVector3d GetCartesianPos() const{ return center_world_pos_; }

	const gMatrix3d GetBody2World() const{ return body2enu_*enu2world_; }
	
private:
	//中心在笛卡尔坐标系的位置
	gVector3d	center_world_pos_;

	//中心在大地坐标系的位置
	rLBH		center_ground_pos_;

	//本体相对当地北天东的姿态角
	rAttitude	att_;

	//模型本体到东北天的旋转矩阵
	gMatrix3d	body2enu_;

	//东北天到世界的旋转矩阵
	gMatrix3d	enu2world_;
};
