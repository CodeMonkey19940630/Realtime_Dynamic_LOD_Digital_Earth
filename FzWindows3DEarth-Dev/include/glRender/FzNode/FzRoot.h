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
	 * \brief	��ӿɻ��ƵĽڵ�
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
	 * \brief	ɾ���ɻ��ƵĽڵ�
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
	/*ʹ�û���������,������������ڴ�*/
	std::map<std::string, IRenderer*> map_renderer_;
};
