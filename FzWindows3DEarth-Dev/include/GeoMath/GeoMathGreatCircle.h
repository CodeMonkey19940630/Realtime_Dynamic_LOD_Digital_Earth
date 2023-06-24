#ifndef GEOMATHGREATCIRCLE_H
#define GEOMATHGREATCIRCLE_H
#include "GeoMathematics.h"
#include "GeoCoordinateStruct.h"
#include <vector>
#include <utility>

// 球面正弦:sin(A)/sin(a)=sin(B)/sin(b)=sin(C)/sin(c)
// 球面余弦:cos(a)=cos(b)*cos(c)+sin(b)*sin(c)*Cos(C)

/*大圆（正圆）航线相关计算*/

/*!
 * \fn	inline double ApproxAngularRadian(rLB _begin, rLB _end)
 *
 * \brief	根据起始点经纬度计算大圆圆心角弧度.
 *
 * \author	向旭杰
 * \date	2022/1/27
 *
 * \param	_begin	起点经纬度.
 * \param	_end  	终点经纬度.
 *
 * \return	返回圆心角.
 */

inline double ApproxAngularRadian(rLB _begin, rLB _end)
{
	double dlon = _end.L - _begin.L;
	double dlat = _end.B - _begin.B;
	double k = sin(dlat * 0.5);
	double l = sin(dlon * 0.5);
	double a = k * k + cos(_begin.B) * cos(_end.B) * l * l;
	a = a > 1 ? 1 : a;
	double c = 2 * asin(sqrt(a));
	return c;
}

/*!
 * \fn	inline double ApproxHeadingRadian(rLB _begin, rLB _end)
 *
 * \brief	根据起始点经纬度计算初始航向弧度.
 *
 * \author	向旭杰
 * \date	2022/1/28
 *
 * \param	_begin	起点经纬度.
 * \param	_end  	终点经纬度.
 *
 * \return	返回航向角.
 */

inline double ApproxHeadingRadian(rLB _begin, rLB _end)
{
	double dlon = _end.L - _begin.L;
	double dlat = _end.B - _begin.B;
	double k = sin(dlat * 0.5);
	double l = sin(dlon * 0.5);
	double cosB_begin = cos(_begin.B), cosB_end = cos(_end.B);
	double a = k * k + cosB_begin *cosB_end * l * l;
	a = a > 1 ? 1 : a;
	
	//  a==sin(theta/2)^2;
	//	double angular_radian = 2 * asin(sqrt(a));
	//	sin(angular_radian)==2*sqrt(a*(1-a));
	//	cos(angular_radian)==1-2*a;
	//	double sin_heading = sin(dlon) * cosB_end / sin(angular_radian);
	
	//球面正弦定理
	double sin_heading = sin(dlon) * cosB_end / (2*sqrt(a*(1-a)));
	if (sin_heading>1){
		sin_heading = 1;
	}
	else if (sin_heading<-1){
		sin_heading = -1;
	}
	double heading_radian = asin(sin_heading);
	//根据球面余弦定理,cos(a)>cos(b)*cos(c)时航向角为锐角。
	if (sin(_end.B) < sin(_begin.B)*(1 - 2 * a))
	{
		if (heading_radian >= 0)
			heading_radian = M_PI - heading_radian;
		else
			heading_radian = -M_PI - heading_radian;
	}
	return heading_radian;
}

/*!
 * \fn	rLB IntermediateLonLat(double _f, rLB _begin, rLB _end, double _angular_radian)
 *
 * \brief	根据首尾经纬度插中间值.
 *
 * \author	向旭杰
 * \date	2022/1/27
 *
 * \param	_f			   	插值占比(0~1).
 * \param	_begin		   	起点经纬度.
 * \param	_end		   	终点经纬度.
 * \param	_angular_radian	对应圆心角.
 *
 * \return	插值点经纬度.
 */

inline rLB IntermediateLonLat(double _f, rLB _begin, rLB _end, double _angular_radian)
{
	double sind = sin(_angular_radian);
	double cosLat1 = cos(_begin.B);
	double cosLat2 = cos(_end.B);
	double sinLat1 = sin(_begin.B);
	double sinLat2 = sin(_end.B);

	double cosLon1 = cos(_begin.L);
	double cosLon2 = cos(_end.L);
	double sinLon1 = sin(_begin.L);
	double sinLon2 = sin(_end.L);
	double A = sin((1 - _f) * _angular_radian) / sind;
	double B = sin(_f * _angular_radian) / sind;
	double x = A * cosLat1 * cosLon1 + B * cosLat2 * cosLon2;
	double y = A * cosLat1 * sinLon1 + B * cosLat2 * sinLon2;
	double z = A * sinLat1 + B * sinLat2;
	return rLB(atan2(y, x), atan2(z, sqrt(x * x + y * y)));
}

/*!
 * \fn	rLB GetLBFromHeadingAngularRadian(rLB _pos, double _heading_radian, double _angular_radian)
 *
 * \brief	根据经纬，射向弧度和圆心角弧度求目标经纬度.
 *
 * \author	向旭杰
 * \date	2022/1/27
 *
 * \param	_pos		   	起点经纬度.
 * \param	_heading_radian	射向弧度.
 * \param	_angular_radian	圆心角弧度.
 *
 * \return	目标经纬度.
 */

inline rLB GetLBFromHeadingAngularRadian(rLB _pos, double _heading_radian, double _angular_radian)
{
	rLB _tgt;
	//已知SAS,求对边弧度和临边角度。

	//对边弧度，即目标纬度;
	double ds = (M_PI_2 - _pos.B) - _angular_radian;
	double k = sin(ds * 0.5);
	double l = sin(_heading_radian * 0.5);
	double a = k * k + cos(_pos.B) * sin(_angular_radian) * l * l;
	a = a > 1 ? 1 : a;
	double side_arc = 2 * asin(sqrt(a));
	_tgt.B = M_PI_2 - side_arc;

	//根据球面正弦定理求经度角
	double sin_angle = sin(_angular_radian) * sin(_heading_radian) / sin(side_arc);
	if (sin_angle > 1){
		sin_angle = 1;
	}
	else if (sin_angle < -1){
		sin_angle = -1;
	}
	//暂时存放经度差值。
	_tgt.L = asin(sin_angle);
	//根据球面余弦定理,cos(a)>cos(b)*cos(c)时经度差值为锐角。
	if (cos(_angular_radian)<sin(_pos.B)*sin(_tgt.B))
	{
		if (_tgt.L >= 0)
			_tgt.L = M_PI - _tgt.L;
		else
			_tgt.L = -M_PI - _tgt.L;
	}
	_tgt.L = _pos.L + _tgt.L;
	_tgt.L = _tgt.L - round(_tgt.L / (2 * M_PI)) * (2 * M_PI);
	return _tgt;
}

//根据连线向量计算转动轴向和转角
template<class T>
void BezierControlPairFunc(const std::vector<T>& _line_vec, std::vector<std::pair<double, T>>& _rota_axis_vec)
{
	int count_ = _line_vec.size();
	_rota_axis_vec.resize(2 * count_);
	for (int i = 0; i < count_ - 1; i++)
	{
		double length_i0, length_i1;
		length_i0 = _line_vec[i].Length();
		length_i1 = _line_vec[i + 1].Length();
		double rote = length_i0 / (length_i0 + length_i1);
		double coangle = acos(_line_vec[i].Dot(_line_vec[i + 1]) / (_line_vec[i].Length() * _line_vec[i + 1].Length()));

		gVector3f normal_axis = _line_vec[i + 1].Cross(_line_vec[i]);
		_rota_axis_vec[2 * i + 1] = make_pair(M_PI - rote*coangle, normal_axis);
		_rota_axis_vec[2 * i + 2] = make_pair((1 - rote)*coangle, normal_axis);
	}
	if (count_ <= 2){
		for (size_t i = 0; i < count_; i++)
		{
			gVector3f normal_axis = gVector3f(1, 0, 0).Cross(_line_vec[i]);
			_rota_axis_vec[2 * i] = make_pair(0, normal_axis);
			_rota_axis_vec[2 * i + 1] = make_pair(M_PI, normal_axis);
		}
	}
	else{
		_rota_axis_vec[0] = _rota_axis_vec[1];
		_rota_axis_vec[0].first = M_PI - _rota_axis_vec[0].first;
		_rota_axis_vec[2 * count_ - 1] = _rota_axis_vec[2 * count_ - 2];
		_rota_axis_vec[2 * count_ - 1].first = M_PI - _rota_axis_vec[2 * count_ - 1].first;
	}
}



//根据起止点和两控制点位置进行贝塞尔曲线插值
template<class T>
void BezierFunc(const T vec[4], double _t, T& _out)
{
	_out = vec[0] + _t*(-3 * vec[0] + 3 * vec[1] + _t*(3 * vec[0] - 6 * vec[1] + 3 * vec[2] + _t*(-vec[0] + 3 * vec[1] - 3 * vec[2] + vec[3])));
}

#endif // !GEOMATHGREATCIRCLE_H
