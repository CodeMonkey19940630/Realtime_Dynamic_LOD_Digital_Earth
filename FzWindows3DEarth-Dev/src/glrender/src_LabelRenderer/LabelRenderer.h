
#ifndef TERRAINRENDERER_H
#define TERRAINRENDERER_H

#include "labelrenderer_global.h"
#include "../../../include/glRender/FzRenderProgramme/IRenderer.h"
#include <QOpenGLShaderProgram>


class LabelRenderer :public IRenderer
{
public:
	LabelRenderer();
	~LabelRenderer();

	virtual const char* RenderObjectName() const override;

	virtual bool Init(QOpenGLWidget* _widget, QOpenGLFunctions_3_3_Core* _function) override;

	virtual void Render(FzSituationSystem* _system, std::vector<FzNode*> _node_vector) override;

	virtual void Uninit() override;

	virtual void Paint() override;

protected:
	QOpenGLShaderProgram surfProgram;
	char*	vert_memery_;
	char*	frag_memery_;
	FzSituationSystem* system_;
	std::vector<FzNode*> nodes_;

protected:
	QOpenGLWidget* widget_;
	QOpenGLFunctions_3_3_Core* f_;
};

extern "C" LABELRENDERER_EXPORT IRenderer*	CreateRenderer()
{
	return new LabelRenderer();
}

#endif // TERRAINRENDERER_H