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
	//�����ڵѿ�������ϵ��λ��
	gVector3d	center_world_pos_;

	//�����ڴ������ϵ��λ��
	rLBH		center_ground_pos_;

	//������Ե��ر��춫����̬��
	rAttitude	att_;

	//ģ�ͱ��嵽���������ת����
	gMatrix3d	body2enu_;

	//�����쵽�������ת����
	gMatrix3d	enu2world_;
};
