#include "../../../include/glRender/FzRenderProgramme/IRenderer.h"

void IRenderer::AddNode(FzNode* _node)
{
	auto iter = std::lower_bound(nodes_.begin(), nodes_.end(), _node);
	nodes_.insert(iter, _node);
}

void IRenderer::EraseNode(FzNode* _node)
{
	auto iter = std::lower_bound(nodes_.begin(), nodes_.end(), _node);
	if (iter != nodes_.end() && *iter == _node){
		nodes_.erase(iter);
	}
}

void IRenderer::ClearAllNodes()
{
	nodes_.clear();
}

std::vector<FzNode*> IRenderer::GetAllNodes()
{
	return nodes_;
}
