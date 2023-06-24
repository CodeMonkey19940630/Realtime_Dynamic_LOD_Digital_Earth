#include "EnityNode.h"

QNodeTimer::QNodeTimer(QWidget *parent /*= nullptr*/) :QWidget(parent)
{

}

QNodeTimer::~QNodeTimer()
{

}

void QNodeTimer::connectNode(EntityNode* _node)
{
	QTimer* timer = new QTimer();
	timer->start(100);
	connect(timer, &QTimer::timeout, this, [=]()
	{
		dLBH pos = _node->GetGroundPos().toDegree();
		pos.B += 0.01;
		pos.H = 6000;
		_node->SetGroundPos(pos);
	});
}
