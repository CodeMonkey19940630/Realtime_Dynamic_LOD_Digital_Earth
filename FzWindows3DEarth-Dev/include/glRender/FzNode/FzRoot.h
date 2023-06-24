#pragma once
#include <vector>
#include <map>
#include "FzNode.h"
#include "../FzRenderProgramme/IRenderer.h"

class FZNODE_EXPORT FzRoot
{
public:
	FzRoot();

	~FzRoot();

	/*!
	 * \fn	bool FzRoot::AddRenderNode(FzNode* _node);
	 *
	 * \brief	添加可绘制的节点
	 *
	 * \author	Admin
	 * \date	2022/7/28
	 *
	 * \param [in,out]	_node	If non-null, the node.
	 *
	 * \return	True if it succeeds, false if it fails.
	 */

	bool AddRenderNode(FzNode* _node);

	/*!
	 * \fn	bool FzRoot::EraseRenderNode(FzNode* _node);
	 *
	 * \brief	删除可绘制的节点
	 *
	 * \author	Admin
	 * \date	2022/7/28
	 *
	 * \param [in,out]	_node	If non-null, the node.
	 *
	 * \return	True if it succeeds, false if it fails.
	 */

	bool EraseRenderNode(FzNode* _node);
	
public:
	/*使用绘制器方法,不管理绘制器内存*/
	std::map<std::string, IRenderer*> map_renderer_;
};
