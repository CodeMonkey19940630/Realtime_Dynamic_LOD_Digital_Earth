#include "../../../include/glRender/FzNode/FzRoot.h"


FzRoot::FzRoot()
{

}

FzRoot::~FzRoot()
{
	for (auto& irenderer : map_renderer_)
	{
		irenderer.second->ClearAllNodes();
	}
	map_renderer_.clear();
	return;
}

bool FzRoot::AddRenderNode(FzNode* _node)
{
	bool _return = true;
	{
		_node->root_ = this;
		auto& map_iter = map_renderer_.find(_node->GetNodeObjectName());
		if (map_iter == map_renderer_.end())
		{
			if (strcmp(_node->GetNodeObjectName(),"")!=0){
				printf_s("%s������������,�����ͽڵ����ʧ��", _node->GetNodeObjectName());
			}
			_return= false;
		}
		else
		{
			map_iter->second->AddNode(_node);
		}
	}
	for (auto& iter : _node->children_)
	{
		AddRenderNode(iter);
	}
	return _return;
}

bool FzRoot::EraseRenderNode(FzNode* _node)
{
	bool _return = true;
	{
		_node->root_ = nullptr;
		auto& map_iter = map_renderer_.find(_node->GetNodeObjectName());
		if (map_iter == map_renderer_.end())
		{
			if (strcmp(_node->GetNodeObjectName(), "") != 0){
				printf_s("%s������������,�����ͽڵ�ɾ��ʧ��", _node->GetNodeObjectName());
			}
			_return=false;
		}
		else
		{
			map_iter->second->EraseNode(_node);
		}
	}
	for (auto& iter : _node->children_)
	{
		EraseRenderNode(iter);
	}
	return _return;
}