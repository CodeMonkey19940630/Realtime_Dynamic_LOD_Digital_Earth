#include "RoamCamera.h"



RoamCamera::RoamCamera()
{
}


RoamCamera::~RoamCamera()
{
}

void RoamCamera::setCameraPosition(const gVector3d& _view_point, const double& _r_heading, const double& _r_tilt, const double& _m_dis)
{
	gMatrix3d enu2ecr;
	enu2ecr[2] = _view_point.GetNormalize();
	if (enu2ecr[2].x || enu2ecr[2].y)//�Ǽ��������õ����ض�����ECR��λ����
		enu2ecr[0] = gVector3d(-enu2ecr[2].y, enu2ecr[2].x, 0).GetNormalize();
	else
	{
		enu2ecr[0] = gVector3d(0, 1, 0);
	}
	enu2ecr[1] = enu2ecr[2].Cross(enu2ecr[0]);

	//��������ת���۲�����ϵ����ͱ��ض�����һ��
	gMatrix3d view2enu = {
		gVector3d(1,0,0),
		gVector3d(0,1,0),
		gVector3d(0,0,1)
	};
	//����ǣ��ƹ۲�ϵX�ᣬ�ٺ���ǣ��ƹ۲�ϵ-Z�ᡣ
	view2enu = Matrix3RotateX(_r_tilt)*Matrix3RotateZ(-_r_heading)*view2enu;

	gMatrix3d view2ecr= view2enu*enu2ecr;
	world2View = view2ecr.GetTranspose();

	cameraPosEcr = _view_point + gVector3d(0, 0, _m_dis)*view2ecr;
}

