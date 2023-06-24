#pragma once
#include "QWidget.h"
#include "QTimer.h"
#include "../../../include/glRender/FzNode/FzModelNode.h"
#include "../../../include/glRender/FzNode/FzLabelNode.h"
class  EntityNode :public FzMovingNode
{
public:
	EntityNode()
	{
		label_ = new FzLabelNode("标牌");
		label_->SetParent(this);
		label_->SetText("测试数据\n飞机1号");
		label_->SetFrameColor({ 127, 127, 127, 255 });
		label_->SetFillColor({ 255, 255, 255, 127 });
		label_->SetLinkLineColor({ 64, 64, 64, 255 });
		label_->SetTextColor({ 0, 0, 0, 255 });

		flying_model_ = new FzModelNode("模型");
		flying_model_->SetParent(this);
		flying_model_->SetScale(1000);
		flying_model_->SetModelingName("J-20");
	};

	~EntityNode()
	{
		/*delete label_,*/ label_=nullptr;
		/*delete flying_model_,*/ flying_model_=nullptr;
	}
	FzLabelNode*					label_;

	FzModelNode*					flying_model_;

	virtual void SetProject(MapProjectedEnum _project) override
	{
		label_->SetProject(_project);
		flying_model_->SetProject(_project);

		FzMovingNode::SetProject(_project);
	}

	virtual void SetGroundPos(const rLBH _position) override
	{
		label_->SetGroundPos(_position);
		flying_model_->SetGroundPos(_position);

		FzMovingNode::SetGroundPos(_position);
	}

	virtual void SetAttitude(const rAttitude _att) override
	{
		flying_model_->SetAttitude(_att);

		FzMovingNode::SetAttitude(_att);
	}

};

class QNodeTimer :QWidget
{
	Q_OBJECT
public:
	QNodeTimer(QWidget *parent = nullptr);
	~QNodeTimer();
public:
	void connectNode(EntityNode* _node);
};