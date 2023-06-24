#include "RendererMgr.h"
#include <qlibrary>
#include <QDebug>

RendererMgr* RendererMgr::render_mgr_ = nullptr;

typedef const char* (*func_GetObjectName)();
bool cmpRenderer(const IRenderer* a, const IRenderer* b)
{
	return strcmp(a->RenderObjectName(),b->RenderObjectName());
}

RendererMgr::RendererMgr()
{
	vec_renderer_func_.clear();
}

RendererMgr::~RendererMgr()
{
	render_mgr_ = nullptr;
}

void RendererMgr::LoadRenderProgramme(const std::string & _file_name)
{
	QString qstr_filename = QString::fromLocal8Bit(_file_name.c_str());
	QLibrary *library = new QLibrary(qstr_filename);
	if (!library->load()) {
		qDebug() << qstr_filename << QString::fromLocal8Bit("加载失败");
	}
	else
	{
		func_CreateRenderer renderer_instance = (func_CreateRenderer)library->resolve("CreateRenderer");
		if (renderer_instance)
		{
			vec_renderer_func_.push_back(renderer_instance);
		}
		else
		{
			qDebug() << qstr_filename << QString::fromLocal8Bit("创建实例对象失败");
		}
	}

}

std::vector<IRenderer*> RendererMgr::CreateLoadedRenderer()
{
	std::vector<IRenderer*> vec_render;
	for (size_t i = 0; i < vec_renderer_func_.size(); i++)
	{
		IRenderer* _render = vec_renderer_func_[i]();
		qDebug() << QString::fromLocal8Bit("绘制器:") << _render->RenderObjectName() << QString::fromLocal8Bit("已加载");
		if (_render)
			vec_render.push_back(_render);
	}
	return vec_render;
}
