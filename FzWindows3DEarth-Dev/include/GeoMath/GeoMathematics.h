#ifndef GEOMATHEMATICS_H
#define GEOMATHEMATICS_H

#if 0
#ifdef GEO_MATH_LIBRARY
#define GEOMATH_SHARED_EXPORT __declspec(dllexport)
#else
#define GEOMATH_SHARED_EXPORT __declspec(dllimport)
#endif
#else
#define GEOMATH_SHARED_EXPORT  
#endif

#define _USE_MATH_DEFINES
#include <math.h>

#define _USE_MATRX_RIGHT_MULTIPLICATION
namespace GMath
{
	//角度转弧度
	const double d2r_ = M_PI / 180.0;
	//弧度转角度
	const double r2d_ = 180.0 / M_PI;
	//正球半径
	const double earth_radius_ = 6371000;
	enum MatrxMulOrder : char
	{
		LEFT_MUL = 0,
		RIGHT_MUL = 1,
	};
#ifdef _USE_MATRX_RIGHT_MULTIPLICATION
	static MatrxMulOrder mm_order_ = RIGHT_MUL;
#else
	static MatrxMulOrder mm_order_ = LEFT_MUL;
#endif
}

#ifndef D2R
#define D2R(a) (a*GMath::d2r_)
#endif // !D2R
#ifndef R2D
#define R2D(a) (a*GMath::r2d_)
#endif // !R2D

#endif // GEOMATHEMATICS_H