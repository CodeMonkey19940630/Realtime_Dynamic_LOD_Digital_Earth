#include "situationwidget.h"
#include <QDebug>
#include <QCoreApplication>
#include <QEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPainter>
#include "../../../include/GeoMath/CustomMath.h"
using namespace std;
#pragma comment(lib,"FzNode.lib")
#pragma comment(lib,"TerrainRenderer.lib")

SituationWidget::SituationWidget(QWidget *parent)
	: QOpenGLWidget(parent)
	, clear_color(Qt::yellow)
{
	QSurfaceFormat surfaceFormat;
	surfaceFormat.setDepthBufferSize(24);
	surfaceFormat.setStencilBufferSize(8);
	surfaceFormat.setSamples(4);//多重采样
	setFormat(surfaceFormat); //setFormat是QOpenGLWidget的函数
	setFocusPolicy(Qt::ClickFocus);
	setMinimumSize(400, 400);

	renderer_mgr_ = RendererMgr::getInstance();

	timer_.start(40);
	bool _is_update = true;
	connect(&timer_, &QTimer::timeout, this, [=]() {
		if (!isHidden())
			QWidget::update();});

	//一个窗口相当于一个根节点,由它创建相应根管理指针
	root_ = new FzRoot();
	/*只是为了管层级需要才增加,*/
	//root_->AddRenderNode(this);
	
	//先创建对象,向根管理内添加绘制功能
	vec_renderer_ = renderer_mgr_->CreateLoadedRenderer();
	for (size_t i = 0; i < vec_renderer_.size(); i++){
		root_->map_renderer_[vec_renderer_[i]->RenderObjectName()] = vec_renderer_[i];
	}
	terrain_single_ = nullptr;
}

SituationWidget::~SituationWidget()
{
	//绘制器管理指针置空,内存别动
	renderer_mgr_ = nullptr;
	//释放子节点内存
	DestroyChildren();

	//根管理析构并置空,以免继承基类FzNode的析构调用野指针
	delete root_, root_ = nullptr;

	//绘制器清空,内存释放完毕
	for (size_t i = 0; i < vec_renderer_.size(); i++)
	{
		delete vec_renderer_[i], vec_renderer_[i] = nullptr;
	}
	vec_renderer_.clear();
}

void SituationWidget::initializeGL()
{
	initializeOpenGLFunctions();
	for (size_t i = 0; i < vec_renderer_.size(); i++){
		vec_renderer_[i]->Init(this,this);
	}
}

void SituationWidget::paintGL()
{
	gVector2f _centpos;
	system_.camera_.setCameraPosition(LBHToEcr(system_.surface_point_),D2R(system_.d_heading_) , D2R(system_.d_tilt_) , system_.m_distance_);
	system_.projected_ = MapProjectedEnum::Sphere;
	map<string, vector<FzNode*>> depthsort_vec_node_;
	double near_dis = 1000;
	double far_dis = EARTH_MEANRADIUS * 4;
	for (size_t i = 0; i < vec_renderer_.size(); i++)
	{
		vector<FzNode*> temp_vec_node = vec_renderer_[i]->GetAllNodes();
		vector<pair<double, FzNode*>> temp_vec_dis_node;
		for (size_t j = 0; j < temp_vec_node.size(); j++)
		{
			//double temp_near = near_dis;
			double temp_far = far_dis;
			//投影切换后设置图元投影
			if (temp_vec_node[j]->GetProjected() != system_.projected_)
				temp_vec_node[j]->SetProject(system_.projected_);
			//做Z平面远近排序和可见性判断；
			if (temp_vec_node[j]->GetVisible() && temp_vec_node[j]->ViewRangeTest(system_.Camera(), nullptr, &temp_far))
			{
				//near_dis = min(near_dis, temp_near);
				far_dis = max(temp_far, far_dis);
				temp_vec_dis_node.push_back({ temp_far, temp_vec_node[j] });
			}
		}
		//排序先比距离再比指针大小
		sort(temp_vec_dis_node.begin(), temp_vec_dis_node.end());
		temp_vec_node.clear();
		for (size_t j = 0; j < temp_vec_dis_node.size(); j++)
			temp_vec_node.push_back(temp_vec_dis_node[j].second);
		depthsort_vec_node_[vec_renderer_[i]->RenderObjectName()] = temp_vec_node;
	}

	system_.projection_ = MatrixPerspectiveFovRH(system_.fovy_, (double)width() / height(), near_dis, far_dis);

	glClearColor(clear_color.redF(), clear_color.greenF(), clear_color.blueF(), clear_color.alphaF());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (size_t i = 0; i < vec_renderer_.size(); i++)
	{
		vec_renderer_[i]->Render(&system_, depthsort_vec_node_[vec_renderer_[i]->RenderObjectName()]);
	}
	for (size_t i = 0; i < vec_renderer_.size(); i++)
	{
		vec_renderer_[i]->Paint();
	}

}

void SituationWidget::resizeGL(int width, int height)
{
	glViewport(0, 0, width, height);
	system_.widget_size_ = gVector2f(width, height);
}

void SituationWidget::mousePressEvent(QMouseEvent *event)
{
	switch (event->button())
	{
	case Qt::LeftButton:
		left_down_ = true;
		break;
	case Qt::MiddleButton:
		middle_down_ = true;
		break;
	case Qt::RightButton:
		right_down_ = true;
		break;
	default:
		break;
	}
	last_mouse_ = { event->x() ,event->y() };
	QOpenGLWidget::mousePressEvent(event);
}

void SituationWidget::mouseMoveEvent(QMouseEvent *event)
{
	gVector2i delta = { event->x(),event->y() };
	delta -=last_mouse_;

	if (right_down_)
	{
		system_.d_heading_ += delta.x;
		system_.d_tilt_ += delta.y;
		clamp(system_.d_tilt_, -90.0, 90.0);
	}

	if (left_down_)
	{
		gVector2d ll_delta = gVector2d(delta) * 0.2 * system_.m_distance_ / EARTH_MEANRADIUS;
		ll_delta = ll_delta* Matrix2Rotate(D2R(system_.d_heading_));
		system_.surface_point_.L -= ll_delta.x;
		system_.surface_point_.B += ll_delta.y;
		clamp(system_.surface_point_.B, -90.0, 90.0);
	}
	last_mouse_ = { event->x() ,event->y() };
	QOpenGLWidget::mouseMoveEvent(event);
}

void SituationWidget::mouseReleaseEvent(QMouseEvent *event)
{
	switch (event->button())
	{
	case Qt::LeftButton:
		left_down_ = false;
		break;
	case Qt::MiddleButton:
		middle_down_ = false;
		break;
	case Qt::RightButton:
		right_down_ = false;
		break;
	default:
		break;
	}
	QOpenGLWidget::mouseReleaseEvent(event);
}

void SituationWidget::wheelEvent(QWheelEvent * event)
{
	double scale = 0;
	if (event->delta() < 0)
		scale = M_SQRT2;
	else
		scale = M_SQRT1_2;
	if (middle_down_)
	{
		system_.fovy_ *= scale;
		clamp(system_.fovy_, M_PI_4 / 4.0, M_PI*M_SQRT1_2);
	}
	else
	{
		system_.m_distance_ *= scale;
		clamp(system_.m_distance_, (double)(1 << 12), (double)(1<<24));
	}
	QOpenGLWidget::wheelEvent(event);
}

void SituationWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
	QOpenGLWidget::mouseDoubleClickEvent(event);
}

void SituationWidget::paintEvent(QPaintEvent *event)
{
	QOpenGLWidget::paintEvent(event);
}

void SituationWidget::moveEvent(QMoveEvent *event)
{
	QOpenGLWidget::moveEvent(event);
}

void SituationWidget::InsertChild(FzNode* _child)
{
	if (strcmp(_child->GetNodeObjectName(), TerrainNodeObject))
	{
		if (terrain_single_ == nullptr){
			terrain_single_ = static_cast<FzTerrainNode*>(_child);
		}
		else
			return;
	}
	FzNode::InsertChild(_child);
}
