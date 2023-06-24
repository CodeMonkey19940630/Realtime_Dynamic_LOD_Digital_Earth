#include "LabelRenderer.h"
#include "../../../include/glRender/FzNode/FzLabelNode.h"
#include <QPainter>

LabelRenderer::LabelRenderer()
{
	widget_ = nullptr;
	f_ = nullptr;
	vert_memery_ = nullptr;
	frag_memery_ = nullptr;
}

LabelRenderer::~LabelRenderer()
{
	if (vert_memery_)
		delete[] vert_memery_, vert_memery_=nullptr;
	if (frag_memery_)
		delete[] frag_memery_, frag_memery_ = nullptr;
	widget_ = nullptr;
	f_ = nullptr;
}

const char* LabelRenderer::RenderObjectName() const
{
	return LabelNodeObject;
}

bool LabelRenderer::Init(QOpenGLWidget* _widget, QOpenGLFunctions_3_3_Core* _function)
{
	widget_ = _widget;
	f_ = _function;
	return true;
}

void LabelRenderer::Render(FzSituationSystem* _system, std::vector<FzNode*> _node_vector)
{
	system_ = _system;
	nodes_ = _node_vector;
}

void LabelRenderer::Uninit()
{

}

void LabelRenderer::Paint()
{
	QPainter painter(widget_);
	QFont font;
	QPen pen;
	QBrush brush;

	double label_width_ = 160, label_height_ = 240;
	for (const auto&var : nodes_)
	{
		FzLabelNode* label = static_cast<FzLabelNode*>(var);
		gVector2f screen_p;
		auto color_index = label->color_;
		QColor color_frame_(color_index[0].r(), color_index[0].g(), color_index[0].b(), color_index[0].a());
		QColor color_fill_(color_index[1].r(), color_index[1].g(), color_index[1].b(), color_index[1].a());
		QColor color_linkline_(color_index[2].r(), color_index[2].g(), color_index[2].b(), color_index[2].a());
		QColor color_text_(color_index[3].r(), color_index[3].g(), color_index[3].b(), color_index[3].a());
		if (system_->getWidgetPosFromWorldPos(label->GetCartesianPos(), screen_p))
		{
			if (screen_p != label->anchor_point_){
				label->ResetAnchorPoint(screen_p);
			}
			font.setPixelSize(label->font_size_);
			painter.setFont(font);
			brush.setColor(color_fill_);
			brush.setStyle(Qt::SolidPattern);
			painter.setRenderHint((QPainter::Antialiasing));
			painter.setBrush(brush);
			pen.setColor(color_linkline_);
			pen.setWidth(1);
			painter.setPen(pen);
			painter.drawLine(QPoint(label->anchor_point_.x, label->anchor_point_.y), QPoint(label->anchor_point_.x + label->relative_point_.x, label->anchor_point_.y + label->relative_point_.y));

			QRectF rect_org(label->anchor_point_.x + label->relative_point_.x, label->anchor_point_.y + label->relative_point_.y, label_width_, label_height_);
			QString _qstring_text;
			if (!label->is_show_node_id_){
				_qstring_text = QString::fromLocal8Bit(label->GetText());
			}
			else{
				_qstring_text = QString::fromLocal8Bit(label->GetNodeID().c_str());
			}
			QRectF rect_current = painter.boundingRect(rect_org, _qstring_text);
			pen.setColor(color_frame_);
			pen.setWidth(1);
			painter.setPen(pen);
			painter.drawRect(rect_current);

			pen.setColor(color_text_);
			painter.setPen(pen);
			QPainterPath myPath;
			QStringList text_vec = _qstring_text.split('\n');
			for (int i = 0; i < text_vec.size(); i++)
			{
				QPointF _text_bottomleft = rect_current.topLeft() + QPointF(0, (i + 1)*rect_current.height() / text_vec.size() - 2);
				myPath.addText(_text_bottomleft, font, text_vec[i]);
			}

			painter.setBrush(color_text_);
			painter.drawPath(myPath);
		}
	}
}
