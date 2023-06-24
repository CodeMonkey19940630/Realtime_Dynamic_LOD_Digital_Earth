#ifndef GEOCOORDINATESTRUCT_H
#define GEOCOORDINATESTRUCT_H
#include "GeoMathematics.h"

//角度制,经纬
struct dLB
{
	dLB() :L(0), B(0)
	{}
	dLB(double _l, double _b)
		: L(_l), B(_b)
	{}
	//大地经度(°)
	double L;
	//大地纬度(°)
	double B;

	bool operator==(const dLB& _data) const
	{
		return L == _data.L&&B == _data.B;
	}
	bool operator!=(const dLB& _data) const
	{
		return L != _data.L||B != _data.B;
	}
};

//弧度制,经纬,可通过dLB赋值
struct rLB
{
	rLB() :L(0), B(0)
	{}
	rLB(double _l, double _b)
		: L(_l), B(_b)
	{}
	rLB(const dLB& _d)
		:L(D2R(_d.L)), B(D2R(_d.B))
	{}
	dLB toDegree() const
	{
		return dLB(R2D(L), R2D(B));
	}
	//大地经度(弧度)
	double L;
	//大地纬度(弧度)
	double B;

	bool operator==(const rLB& _data) const
	{
		return L == _data.L&&B == _data.B;
	}
	bool operator!=(const rLB& _data) const
	{
		return L != _data.L || B != _data.B;
	}
};

//角度制，米制大地经纬高
struct dLBH
{
	dLBH() :L(0), B(0), H(0)
	{}
	dLBH(double _l, double _b, double _h = 0)
		:L(_l), B(_b), H(_h)
	{}
	dLBH(const dLB& _lb, double _h = 0)
		:L(_lb.L), B(_lb.B), H(_h)
	{}
	dLB getdLB() const
	{
		return dLB(L, B);
	}
	//大地纬度(°)
	double L;
	//大地经度(°)
	double B;
	//大地高度(m)
	double H;
	bool operator==(const dLBH& _data) const
	{
		return L == _data.L&&B == _data.B&&H == _data.H;
	}
	bool operator!=(const dLBH& _data) const
	{
		return L != _data.L||B != _data.B||H != _data.H;
	}
};

//弧度制，米制大地经纬高,可通过dLBH赋值
struct rLBH
{
	rLBH() :L(0), B(0), H(0)
	{}
	rLBH(double _l, double _b, double _h = 0)
		:L(_l), B(_b), H(_h)
	{}
	rLBH(const rLB& _lb, double _h = 0)
		:L(_lb.L), B(_lb.B), H(_h)
	{}
	rLBH(const dLBH& _d)
		:L(D2R(_d.L)), B(D2R(_d.B)), H(_d.H)
	{}
	dLBH toDegree() const
	{
		return dLBH(R2D(L), R2D(B), H);
	}
	rLB getrLB() const
	{
		return rLB(L, B);
	}
	//大地经度(弧度)(longitude)
	double L;
	//大地纬度(弧度)(latitude)
	double B;
	//大地高度(m)
	double H;

	bool operator==(const rLB& _data) const
	{
		return L == _data.L&&B == _data.B;
	}
	bool operator!=(const rLB& _data) const
	{
		return L != _data.L || B != _data.B;
	}
};

/*!
 * \fn	inline bool IsLBHApproximate(rLBH _pos_a, rLBH _pos_b, const double _tolerance_m=0.1)
 *
 * \brief	估算地理坐标是否近似重合.
 *
 * \author	向旭杰
 * \date	2022/2/7
 *
 * \param	_pos_a			a点坐标.
 * \param	_pos_b			b点坐标.
 * \param	_tolerance_m	米制误差.
 *
 * \return	如果重合返回true,不重合返回false.
 */

inline bool IsLBHApproximate(rLBH _pos_a, rLBH _pos_b, const double _tolerance_m=0.1)
{
	if (fabs(_pos_a.H - _pos_b.H) > _tolerance_m)
		return false;
	if (fabs(_pos_a.B - _pos_b.B)* GMath::earth_radius_ > _tolerance_m)
		return false;
	double _deta_l = _pos_a.L - _pos_b.L;
	_deta_l = _deta_l - round(_deta_l / (2 * M_PI))*(2 * M_PI);
	if (fabs(_pos_a.L*cos(_pos_a.B) - _pos_b.L*cos(_pos_b.B))*GMath::earth_radius_ > _tolerance_m)
		return false;

	return true;
}

//米制，角度制球坐标
struct dSphereCoord
{
	dSphereCoord() :r(0), azimuth(0), elevation(0)
	{}
	dSphereCoord(double _az, double _tilt, double _r)
		:azimuth(_az), elevation(_tilt), r(_r)
	{}
	//方位角(°)
	double azimuth;
	//倾斜角(°)
	double elevation;
	//半径(m)
	double r;
	
	bool operator==(const dSphereCoord& _data) const
	{
		return azimuth==_data.azimuth&&elevation==_data.elevation&&r==_data.r;
	}
	bool operator!=(const dSphereCoord& _data) const
	{
		return azimuth!=_data.azimuth||elevation!=_data.elevation||r!=_data.r;
	}
};

//米制，弧度制球坐标,可通过dSphereCoord赋值
struct rSphereCoord
{
	rSphereCoord() :r(0), azimuth(0), elevation(0)
	{}
	rSphereCoord(double _az, double _tilt, double _r)
		:azimuth(_az), elevation(_tilt), r(_r)
	{}
	rSphereCoord(const dSphereCoord& _d)
		:azimuth(D2R(_d.azimuth)), elevation(D2R(_d.elevation)), r(_d.r)
	{}
	dSphereCoord toDegree() const
	{
		return dSphereCoord(R2D(azimuth), R2D(elevation), r);
	}
	//方位角(弧度)
	double azimuth;
	//倾斜角(弧度)
	double elevation;
	//半径(m)
	double r;
	
	bool operator==(const rSphereCoord& _data) const
	{
		return azimuth==_data.azimuth&&elevation==_data.elevation&&r==_data.r;
	}
	bool operator!=(const rSphereCoord& _data) const
	{
		return azimuth!=_data.azimuth||elevation!=_data.elevation||r!=_data.r;
	}
};


//角度制 方位俯仰,定向角
struct dOrientation
{
	dOrientation() : yaw(0), pitch(0)
	{}
	dOrientation(double _yaw, double _pitch)
		: yaw(_yaw), pitch(_pitch)
	{}
	//方位角(°)
	double yaw;
	//倾斜角(°)
	double pitch;
	
	bool operator==(const dOrientation& _data) const
	{
		return yaw==_data.yaw&&pitch==_data.pitch;
	}
	bool operator!=(const dOrientation& _data) const
	{
		return yaw!=_data.yaw||pitch!=_data.pitch;
	}
};

//弧度制 方位俯仰,可通过dOrientation赋值
struct rOrientation
{
	rOrientation() : yaw(0), pitch(0)
	{}
	rOrientation(double _yaw, double _pitch)
		: yaw(_yaw), pitch(_pitch)
	{}
	rOrientation(const dOrientation& _d)
		: yaw(D2R(_d.yaw)), pitch(D2R(_d.pitch))
	{}
	dOrientation toDegree() const
	{
		return dOrientation(R2D(yaw), R2D(pitch));
	}
	//方位角(弧度)
	double yaw;
	//倾斜角(弧度)
	double pitch;
	
	bool operator==(const rOrientation& _data) const
	{
		return yaw==_data.yaw&&pitch==_data.pitch;
	}
	bool operator!=(const rOrientation& _data) const
	{
		return yaw!=_data.yaw||pitch!=_data.pitch;
	}
};

//角度制偏航，俯仰，滚转，姿态角
struct dAttitude
{
	dAttitude() :yaw(0), pitch(0), roll(0)
	{}
	dAttitude(double _yaw, double _pitch, double _roll)
		:yaw(_yaw), pitch(_pitch), roll(_roll)
	{}
	dAttitude(const dOrientation& _rori, double _roll = 0)
		:yaw(_rori.yaw), pitch(_rori.pitch), roll(_roll)
	{}
	dOrientation getdOrientation() const
	{
		return dOrientation(yaw,pitch);
	}
	//偏航角(°)
	double yaw;
	//俯仰角(°)
	double pitch;
	//滚转角(°)
	double roll;
	
	bool operator==(const dAttitude& _data) const
	{
		return yaw==_data.yaw&&pitch==_data.pitch&&roll==_data.roll;
	}
	bool operator!=(const dAttitude& _data) const
	{
		return yaw!=_data.yaw||pitch!=_data.pitch||roll!=_data.roll;
	}
};

//弧度制偏航，俯仰，滚转
struct rAttitude
{
	rAttitude() :yaw(0), pitch(0), roll(0)
	{}
	rAttitude(double _yaw, double _pitch, double _roll)
		:yaw(_yaw), pitch(_pitch), roll(_roll)
	{}
	rAttitude(const dAttitude& _d)
		: yaw(D2R(_d.yaw)), pitch(D2R(_d.pitch)), roll(D2R(_d.roll))
	{}
	rAttitude(const rOrientation& _rori, double _roll = 0)
		:yaw(_rori.yaw), pitch(_rori.pitch), roll(_roll)
	{}
	dAttitude toDegree() const
	{
		return dAttitude(R2D(yaw), R2D(pitch), R2D(roll));
	}
	rOrientation getrOrientation() const
	{
		return rOrientation(yaw, pitch);
	}
	//偏航角(弧度)
	double yaw;
	//俯仰角(弧度)
	double pitch;
	//滚转角(弧度)
	double roll;
	
	bool operator==(const rAttitude& _data) const
	{
		return yaw==_data.yaw&&pitch==_data.pitch&&roll==_data.roll;
	}
	bool operator!=(const rAttitude& _data) const
	{
		return yaw!=_data.yaw||pitch!=_data.pitch||roll!=_data.roll;
	}
};

//角度制欧拉角 
struct dEuler
{
	dEuler() :psi(0), theta(0), phi(0)
	{}
	dEuler(double _psi, double _theta, double _phi)
		:psi(_psi), theta(_theta), phi(_phi)
	{}
	//内旋角一(角度)
	double psi;
	//内旋角二(角度)
	double theta;
	//内旋角三(角度)
	double phi;
	
	bool operator==(const dEuler& _data) const
	{
		return psi==_data.psi&&theta==_data.theta&&phi==_data.phi;
	}
	bool operator!=(const dEuler& _data) const
	{
		return psi!=_data.psi||theta!=_data.theta||phi!=_data.phi;
	}
};

//弧度制欧拉角 
struct rEuler
{
	rEuler() :psi(0), theta(0), phi(0)
	{}
	rEuler(double _psi, double _theta, double _phi)
		:psi(_psi), theta(_theta), phi(_phi)
	{}
	rEuler(const dEuler& _d)
		:psi(D2R(_d.psi)), theta(D2R(_d.theta)), phi(D2R(_d.phi))
	{}
	dEuler toDegree() const
	{
		return dEuler(R2D(psi), R2D(theta), R2D(phi));
	}
	//内旋角一(弧度)
	double psi;
	//内旋角二(弧度)
	double theta;
	//内旋角三(弧度)
	double phi;
	
	bool operator==(const rEuler& _data) const
	{
		return psi==_data.psi&&theta==_data.theta&&phi==_data.phi;
	}
	bool operator!=(const rEuler& _data) const
	{
		return psi!=_data.psi||theta!=_data.theta||phi!=_data.phi;
	}
};


//椭球规格
struct ellipsoid_standard
{
	/*! \brief 长轴 */
	double a = 6371000;
	/*! \brief 短轴 */
	double b = 6371000;
	/*! \brief 扁率 alpha=(a-b)/a; */
	double f = 0;
	/*! \brief 极曲率半径 c = (a ^ 2) / b; */
	double c = 1;
	/*! \brief 第一偏心率 e = sqrt(a ^ 2 - b ^ 2) / a; */
	double e1 = 0;
	/*! \brief ee = (a ^ 2 - b ^ 2) / a ^ 2; */
	double e1_sqaure = 0;
	/*! \brief 第二偏心率 e = sqrt(a ^ 2 - b ^ 2) / b; */
	double e2 = 0;
	/*! \brief ee = (a ^ 2 - b ^ 2) / b ^ 2; */
	double e2_sqaure = 0;

	ellipsoid_standard()
	{}
	ellipsoid_standard(double _radius)
	{
		a = _radius;
		b = _radius;
	}
	ellipsoid_standard(double _a, double _b)
	{
		a = _a;
		b = _b;
		f = (a - b) / a;
		c = a*a / b;
		e1_sqaure = (a *a - b*b) / (a*a);
		e1 = sqrt(e1_sqaure);
		e2_sqaure = (a *a - b*b) / (b*b);
		e2 = sqrt(e2_sqaure);
	}
	~ellipsoid_standard()
	{}
};

/*! \brief	wgs84 标准椭球参数 */
const ellipsoid_standard ellipsoid_wgs84_(6378137.0, 6356752.314251795);
/*! \brief	rew库中 IAG75 椭球参数 */
const ellipsoid_standard ellipsoid_iag75_(6378140, 6356755);
/*! \brief	圆球 椭球参数 */
const ellipsoid_standard ellipsoid_sphere_(6378137);

#endif // GEOVECTOR