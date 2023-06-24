#ifndef SITUATIONWIDGET_H
#define SITUATIONWIDGET_H

#include "situationwidget_global.h"
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLBuffer>
#include <QTimer>
#include "../../../include/glRender/FzNode/FzNode.h"
#include "../../../include/glRender/FzNode/FzRoot.h"
#include "../../../include/glRender/FzNode/FzTerrainNode.h"
#include "../../../include/glRender/FzRenderProgramme/IRenderer.h"
#include "SituationSystem.h"


#include "RendererMgr.h"
class SITUATIONWIDGET_EXPORT SituationWidget : public QOpenGLWidget,
	protected QOpenGLFunctions_3_3_Core,
	public FzNode
{
	Q_OBJECT

public:
	SituationWidget(QWidget *parent = 0);
	~SituationWidget();

signals:
	void clicked();
protected:
	void initializeGL() override;
	void paintGL() override;
	void resizeGL(int width, int height) override;

	void mousePressEvent(QMouseEvent *event) override;

	void mouseMoveEvent(QMouseEvent *event) override;

	void mouseReleaseEvent(QMouseEvent *event) override;

	void wheelEvent(QWheelEvent *event) override;

	void mouseDoubleClickEvent(QMouseEvent *event) override;

	void paintEvent(QPaintEvent *event) override;

	void moveEvent(QMoveEvent *event) override;

	virtual void InsertChild(FzNode* _child) override;

private:
	//清屏颜色
	QColor clear_color;
	//绘制刷新定时器
	QTimer timer_;
	//绘制器管理类
	RendererMgr*					renderer_mgr_;
	//态势全局系统
	SituationSystem					system_;
	//按加载顺序排序
	std::vector<IRenderer*>			vec_renderer_;
	//与界面交互相关的节点
	FzTerrainNode*					terrain_single_;

	//鼠标左键是否按下
	bool left_down_ = false;
	//鼠标右键是否按下
	bool right_down_ = false;
	//鼠标中键是否按下
	bool middle_down_ = false;

	//鼠标上一次点选对象
	gVector2i last_mouse_;
};

#endif // SITUATIONWIDGET_H
