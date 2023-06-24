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
	//������ɫ
	QColor clear_color;
	//����ˢ�¶�ʱ��
	QTimer timer_;
	//������������
	RendererMgr*					renderer_mgr_;
	//̬��ȫ��ϵͳ
	SituationSystem					system_;
	//������˳������
	std::vector<IRenderer*>			vec_renderer_;
	//����潻����صĽڵ�
	FzTerrainNode*					terrain_single_;

	//�������Ƿ���
	bool left_down_ = false;
	//����Ҽ��Ƿ���
	bool right_down_ = false;
	//����м��Ƿ���
	bool middle_down_ = false;

	//�����һ�ε�ѡ����
	gVector2i last_mouse_;
};

#endif // SITUATIONWIDGET_H
