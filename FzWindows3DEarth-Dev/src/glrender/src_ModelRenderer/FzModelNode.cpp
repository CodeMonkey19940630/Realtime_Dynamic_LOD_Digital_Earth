#include "modelrenderer_global.h"
#include "../../../include/glRender/FzNode/FzModelNode.h"
#include "ModelingManager.h"
ModelingManager* model_manager_ = nullptr;

FzModelNode::FzModelNode(const std::string& _node_id /*= ""*/)
{
	object_name_ = ModelNodeObject;
	model_scale_ = 1;
}

FzModelNode::~FzModelNode()
{
}

bool FzModelNode::SetModelingName(const std::string& _modeling_key_name)
{
	modeling_key_name_ = _modeling_key_name;
	return true;
}

const std::string FzModelNode::GetModelingName() const
{
	return modeling_key_name_;
}

void FzModelNode::SetScale(double _model_scale)
{
	model_scale_ = _model_scale;
}

double FzModelNode::GetScale() const
{
	return model_scale_;
}

bool FzModelNode::LoadModeling(const ModelingExplain& _explain)
{
	return model_manager_->LoadModelingByExplain(_explain);
}

bool FzModelNode::UnLoadModeling(const std::string& _modeling_key_name)
{
	if (model_manager_)
	{
		model_manager_->UnloadModelingByName(_modeling_key_name);
		return true;
	}
	else
	{
		return false;
	}
}

bool FzModelNode::UnLoadAllModelings()
{
	if (model_manager_)
	{
		model_manager_->UnloadAllModeling();
	}
	return true;
}

