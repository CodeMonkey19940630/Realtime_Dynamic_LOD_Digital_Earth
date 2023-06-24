#pragma once
#include "GeoVector.h"
#include "GeoMatrix.h"
#include "GeoCoordinateStruct.h"

#define RADIAN_TO_METER	(6378137.0)
#define METER_TO_RADIAN	(1.0f/(6378137.0))
#define EARTH_MEANRADIUS	6378137.0	//地球半径

template <class T>
void clamp(T& _value, T _min, T _max)
{
	if (_value < _min)
		_value = _min;
	else if(_max<_value)
		_value = _max;
}

//取余时使余数大于等于0
inline double fmodPostitive(double _divisor, double _dividend, int* _quotient = nullptr)
{
	double tquo = _divisor / _dividend;
	double mod_;
	if (_dividend > 0)
		tquo = floor(tquo);
	else
		tquo = ceil(tquo);
	if (_quotient)
		*_quotient = tquo;
	return _divisor - _dividend*tquo;
}

inline gVector3d LBHToEcr(rLBH _r_lbh)
{
	double latRadians = _r_lbh.B;
	double lonRadians = _r_lbh.L;
	double radius = _r_lbh.H + EARTH_MEANRADIUS;
	float radCosLat = radius * cos(latRadians);

	return gVector3d(radCosLat * cos(lonRadians),
		radCosLat * sin(lonRadians),
		radius * sin(latRadians));
}

inline gVector3d LBHToMercator(rLBH _r_lbh)
{
	gVector3d _mercator;
	_mercator.x = _r_lbh.H;
	_mercator.y = fmodPostitive(_r_lbh.L, 2 * M_PI)*RADIAN_TO_METER;
	_mercator.z = log(tan(M_PI_4 + _r_lbh.B / 2.0))*RADIAN_TO_METER;
	return _mercator;
}

inline gVector2d LBToMercator(rLB _r_lb)
{
	gVector2d _mercator;
	_mercator.x = fmodPostitive(_r_lb.L, 2 * M_PI)*RADIAN_TO_METER;
	_mercator.y = log(tan(M_PI_4 + _r_lb.B / 2.0))*RADIAN_TO_METER;
	return _mercator;
}

inline rLBH EcrToLBH(gVector3d _ecr_pos)
{
	rLBH _r_lbh;
	_r_lbh.H = _ecr_pos.Length();
	if (_ecr_pos.y || _ecr_pos.x)
		_r_lbh.L = atan2(_ecr_pos.y, _ecr_pos.x);
	else
		_r_lbh.L = 0;
	_r_lbh.B = asin(_ecr_pos.z / _r_lbh.H);
	_r_lbh.H -= EARTH_MEANRADIUS;
	return _r_lbh;
}

inline rLB MercatorToLB(gVector2d _mercator)
{
	rLB _lb;
	_lb.L = fmod(_mercator.x*METER_TO_RADIAN, 2 * M_PI);
	_lb.B = 2.0*atan(exp(_mercator.y * METER_TO_RADIAN)) - M_PI_2;
	return _lb;
}

inline rLBH MercatorToLB(gVector3d _mercator)
{
	rLBH _lbh;
	_lbh.H = _mercator.x;
	_lbh.L = fmod(_mercator.y*METER_TO_RADIAN, 2 * M_PI);
	_lbh.B = 2.0*atan(exp(_mercator.z * METER_TO_RADIAN)) - M_PI_2;
	return _lbh;
}

//经度范围为[-180,180)
//纬度范围为[-85.01,85.01]
//Meter(0,0)对应LB(0,85.01)
//tile(0,0)对应LB(180,85.01)；相差M_PI

inline rLB MeterToLB(gVector2d _meter)
{
	rLB _lb;
	double _radian_lon = _meter.x * METER_TO_RADIAN;
	double n = _meter.y * METER_TO_RADIAN;

	//为防止经度越界，做取余判断
	_lb.L = fmod(_radian_lon, 2 * M_PI);
	_lb.B = M_PI_2 - 2.0*atan(exp(-n));
}

inline gVector2d LBToMeter(rLB _lb)
{
	gVector2d _meter;
	//为防止X轴坐标越界，做取余判断
	_lb.L = fmod(_lb.L, 2 * M_PI);
	_meter.x = _lb.L*RADIAN_TO_METER;
	double sinLat = sin(_lb.B);
	//φ∈[-85.01°~85.01°] ln(tan(45°+φ/2))∈[-PI,PI]
	_meter.y = 0.5*log((1.0 + sinLat) / (1.0 - sinLat))*(double)EARTH_MEANRADIUS;
	return _meter;
}

//////////////////////////////////////////////////////////////////////////
// 投影坐标变换
// Y轴原点在左上角，向下为正
inline double dlon2tilex(double d_lon, int z)
{
	double normalize_mod = fmod(d_lon + 180, 360) / 360.0;
	if (normalize_mod < 0)
		normalize_mod += 1;
	return normalize_mod * (1 << z);
}

inline double dlat2tiley(double d_lat, int z)
{
	double latrad = d_lat * M_PI / 180.0;
	return (1.0 - asinh(tan(latrad)) / M_PI) / 2.0 * (1 << z);
}

inline double tilex2dlon(double x, int z)
{
	double normalize_mod = x / (double)(1 << z);
	if (normalize_mod >= 0.5)
		normalize_mod -= 1;
	return  normalize_mod* 360.0 + 180.0;
}

inline double tiley2dlat(int y, int z)
{
	double n = ((double)M_PI) - 2.0 * ((double)M_PI) * y / (double)(1 << z);
	return 180.0 / ((double)M_PI) * atan(0.5 * (exp(n) - exp(-n)));
}

inline gVector2d LB2tile(rLB _lb, int z)
{
	gVector2d tile;
	_lb.L = fmod(_lb.L, 2 * M_PI);
	if (_lb.L < 0)
		_lb.L += 2 * M_PI;
	tile.x = (_lb.L + M_PI) / (2 * M_PI) * (1 << z);
	tile.y = (1.0 - asinh(tan(_lb.B)) / M_PI) / 2.0 * (1 << z);
	return tile;
}

inline rLB tile2LB(gVector2d _tile, int z)
{
	rLB _lb;
	_lb.L = _tile.x / (double)(1 << z) * 2 * M_PI - M_PI;
	double n = ((double)M_PI) - 2.0 * ((double)M_PI) * _tile.y / (double)(1 << z);
	_lb.B = atan(0.5 * (exp(n) - exp(-n)));
	return _lb;
}

//Y轴坐标符号，与经纬度符号一致
inline double YTorLat(double y)
{
	return ((double)M_PI) / 2.0 - 2.0 * atan(exp(-y / (double)EARTH_MEANRADIUS));
}

inline double rLatToY(double lat)
{
	double sinLat = sin(lat);
	return 0.5*log((1.0 + sinLat) / (1.0 - sinLat))*(double)EARTH_MEANRADIUS;

}
inline double rLonToX(double lon)
{
	return lon * (double)EARTH_MEANRADIUS;
}
inline double XTorLon(double x)
{
	return x / (double)EARTH_MEANRADIUS;
}
