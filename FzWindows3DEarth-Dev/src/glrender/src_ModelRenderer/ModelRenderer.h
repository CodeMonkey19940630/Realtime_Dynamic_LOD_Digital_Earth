
#ifndef TERRAINRENDERER_H
#define TERRAINRENDERER_H

#include "modelrenderer_global.h"
#include "../../../include/glRender/FzRenderProgramme/IRenderer.h"
#include <QOpenGLShaderProgram>


class ModelRenderer :public IRenderer
{
public:
	ModelRenderer();
	~ModelRenderer();

	virtual const char* RenderObjectName() const override;

	virtual bool Init(QOpenGLWidget* _widget, QOpenGLFunctions_3_3_Core* _function) override;

	virtual void Render(FzSituationSystem* _system, std::vector<FzNode*> _node_vector) override;

	virtual void Uninit() override;

protected:
	QOpenGLShaderProgram modelProgram;
	char*	vert_memery_;
	char*	frag_memery_;
protected:
	QOpenGLWidget* widget_;
	QOpenGLFunctions_3_3_Core* f_;
};

extern "C" MODELRENDERER_EXPORT IRenderer*	CreateRenderer()
{
	return new ModelRenderer();
}

#endif // TERRAINRENDERER_H