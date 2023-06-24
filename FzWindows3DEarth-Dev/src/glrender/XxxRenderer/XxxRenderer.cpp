#include "XxxRenderer.h"
#if 0
#include "../../../include/glRender/FzNode/FzXxxNode.h"
#else
#include "FzXxxNode.h"
#endif

XxxRenderer::XxxRenderer()
{

}

XxxRenderer::~XxxRenderer()
{

}

const char* XxxRenderer::RenderObjectName() const
{
	return XxxNodeObject;
}

bool XxxRenderer::Init(QOpenGLWidget* _widget, QOpenGLFunctions_3_3_Core* _function)
{
	return false;
}

void XxxRenderer::Render(FzSituationSystem* _system, std::vector<FzNode*> _node_vector)
{

}

void XxxRenderer::Uninit()
{

}
