#pragma once
#include "../../geomath/GeoMathHeader.h"

//地图投影方式：正球——ECR坐标系；立体墨卡托——原点：经纬高(0,0,0)、X：经度增加方向、Y：纬度增加方向、Z：高度增加方向。
enum MapProjectedEnum :unsigned char
{
	UnKnowPrject = 0,
	Sphere = 1,
	Mercator = 2,
};

/*		三维常见坐标系		*/
//本体坐标系(Body)：							只是为了方便人为规定的某一坐标系，不同对象原点和坐标系会有不同的定义，一般XYZ为右手系
//全局笛卡尔坐标系(GlobalCartesian,缩写gc)：	投影为Sphere时,全局笛卡尔坐标系使用ECR坐标系;投影为Mercator时,全局笛卡尔坐标系使用立体墨卡托坐标系;
//世界坐标系(World)：							以镜头位置为坐标原点，XYZ轴向与全局笛卡尔坐标系一致。
//观察坐标系(View)：							以镜头位置为坐标原点，镜头右侧为X轴，镜头上方为Y轴，镜头后方为Z轴。
//投影坐标系(Project)：							将观察坐标进行投影变换后得到的坐标，如果是正交投影、则相当于对XY方向做拉伸或压缩，如果是透视投影、则是对xyz非线性变化,深度规范化后XY方向近大远小。
//齐次裁剪空间(NDC)：							将投影坐标规范化深度值后得到的坐标，以画布中心为坐标原点，画布右侧为X轴∈[-1,1]，画布上方为Y轴∈[-1,1]，Z值越大深度越大∈[0,1]。

//相机的基类
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
