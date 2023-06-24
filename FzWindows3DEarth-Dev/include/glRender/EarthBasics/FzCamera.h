#pragma once
#include "../../geomath/GeoMathHeader.h"

//��ͼͶӰ��ʽ�����򡪡�ECR����ϵ������ī���С���ԭ�㣺��γ��(0,0,0)��X���������ӷ���Y��γ�����ӷ���Z���߶����ӷ���
enum MapProjectedEnum :unsigned char
{
	UnKnowPrject = 0,
	Sphere = 1,
	Mercator = 2,
};

/*		��ά��������ϵ		*/
//��������ϵ(Body)��							ֻ��Ϊ�˷�����Ϊ�涨��ĳһ����ϵ����ͬ����ԭ�������ϵ���в�ͬ�Ķ��壬һ��XYZΪ����ϵ
//ȫ�ֵѿ�������ϵ(GlobalCartesian,��дgc)��	ͶӰΪSphereʱ,ȫ�ֵѿ�������ϵʹ��ECR����ϵ;ͶӰΪMercatorʱ,ȫ�ֵѿ�������ϵʹ������ī��������ϵ;
//��������ϵ(World)��							�Ծ�ͷλ��Ϊ����ԭ�㣬XYZ������ȫ�ֵѿ�������ϵһ�¡�
//�۲�����ϵ(View)��							�Ծ�ͷλ��Ϊ����ԭ�㣬��ͷ�Ҳ�ΪX�ᣬ��ͷ�Ϸ�ΪY�ᣬ��ͷ��ΪZ�ᡣ
//ͶӰ����ϵ(Project)��							���۲��������ͶӰ�任��õ������꣬���������ͶӰ�����൱�ڶ�XY�����������ѹ���������͸��ͶӰ�����Ƕ�xyz�����Ա仯,��ȹ淶����XY�������ԶС��
//��βü��ռ�(NDC)��							��ͶӰ����淶�����ֵ��õ������꣬�Ի�������Ϊ����ԭ�㣬�����Ҳ�ΪX���[-1,1]�������Ϸ�ΪY���[-1,1]��ZֵԽ�����Խ���[0,1]��

//����Ļ���
class FzCamera
{
public:
	FzCamera() {};
	virtual ~FzCamera() {};

public:
	virtual gVector3d getCameraPosition() const
	{
		return cameraPosEcr;
	}
	virtual gMatrix3d getViewMatrix3() const
	{
		return world2View;
	}
	virtual gMatrix4d getViewMatrix4() const
	{
		return MatrixUpOrder4(world2View);
	}
	virtual gMatrix4d getGC2View() const
	{
		return MatrixUpOrder4(world2View, -cameraPosEcr);
	}
protected:
	gVector3d cameraPosEcr;
	gMatrix3d world2View;
};
