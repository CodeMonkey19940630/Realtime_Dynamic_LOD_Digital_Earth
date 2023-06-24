#pragma once
#if 0
#include "FzNode.h"
#else
#include "../../../include/glRender/FzNode/FzNode.h"
#endif
#define  XxxNodeObject "object.xxx"
#ifdef XXXRENDERER_LIB
# define XXXRENDERER_EXPORT __declspec(dllexport)
#else
# define XXXRENDERER_EXPORT __declspec(dllexport)
#endif

class XxxRenderer;
class XXXRENDERER_EXPORT FzXxxNode :public FzNode
{
public:
	FzXxxNode(const std::string& _node_id = "");
	~FzXxxNode();

protected:
	friend class XxxRenderer;

};

