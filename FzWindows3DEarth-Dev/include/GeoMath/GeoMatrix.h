#ifndef GEOMATRIX_H
#define GEOMATRIX_H
#include "GeoMathematics.h"
#include "GeoVector.h"
#pragma warning( disable : 4114 )
//2阶矩阵
template <typename T>
struct  gMatrix2T
{
	gMatrix2T();
	template <typename U>
	gMatrix2T(const gVector2T<U>& _v1, const const gVector2T<U>& _v2);
	template <typename U>
	gMatrix2T(const gMatrix2T<U>& _m);
	gMatrix2T(	T _s11, T _s12,
				T _s21, T _s22);

	//加法
	gMatrix2T<T> operator+(const gMatrix2T& _m2) const;
	//减法
	gMatrix2T<T> operator-(const gMatrix2T& _m2) const;
	//加等于
	void operator+=(const gMatrix2T& _m2);
	//减等于
	void operator-=(const gMatrix2T& _m2);
	//乘等于
	void operator*=(const gMatrix2T& _m2);
	//标量乘法
	gMatrix2T<T> operator*(const T& _s) const;
	template <typename T>
	friend gMatrix2T<T>	operator*(const double& _s, const gMatrix2T<T>& _m2);
	//同向量乘法
	gVector2T<T> operator*(const gVector2T<T>& _v2) const;
	template <typename T>
	friend gVector2T<T>	operator*(const gVector2T<T>& _v2, const gMatrix2T<T>& _m2);
	//矩阵乘法
	gMatrix2T<T> operator*(const gMatrix2T& _m2) const;
	//标量除法
	gMatrix2T<T> operator/(const T& _s) const;
	//乘等于
	void operator*=(const T& _s);
	//求代数余子式
	gMatrix2T<T> GetCofactorMatrix() const;
	//求逆矩阵
	gMatrix2T<T> GetInverse() const;
	//求转置矩阵
	gMatrix2T<T> GetTranspose() const;
	//转为转置矩阵
	void toTranspose();
	//计算行列式
	T	det() const;
	//转为单位矩阵
	void toIdentity();
	//取下标偏移
	gVector2T<T>& operator[](unsigned _i) const;
	//返回第i(0,1)行
	gVector2T<T>	row(unsigned _i) const;
	//返回第j(0,1)列
	gVector2T<T>	column(unsigned _j) const;
	//设置第j(0,1)列
	void			setColumn(unsigned _j,const gVector2T<T>& _v2);
	//比较两个矩阵是否相同
	bool operator ==(const gMatrix2T<T>& _mat);

	T _11;
	T _12;
	T _21;
	T _22;
};


//3阶矩阵
template <typename T>
struct  gMatrix3T
{
	gMatrix3T();
	template <typename U>
	gMatrix3T(const gVector3T<U>& _v1, const gVector3T<U>& _v2, const gVector3T<U>& _v3);
	template <typename U>
	gMatrix3T(const gMatrix3T<U>& _m);
	gMatrix3T(	T _s11, T _s12, T _s13,
				T _s21, T _s22, T _s23,
				T _s31, T _s32, T _s33);

	//加法
	gMatrix3T<T> operator+(const gMatrix3T& _m3) const;
	//减法
	gMatrix3T<T> operator-(const gMatrix3T& _m3) const;
	//加等于
	void operator+=(const gMatrix3T& _m3);
	//减等于
	void operator-=(const gMatrix3T& _m3);
	//乘等于
	void operator*=(const gMatrix3T& _m3);
	//标量乘法
	gMatrix3T<T> operator*(const T& _s) const;
	template <typename T>
	friend gMatrix3T<T>	operator*(const double& _s, const gMatrix3T<T>& _m3);
	//同向量乘法
	gVector3T<T> operator*(const gVector3T<T>& _v3) const;

	template <typename T>
	friend gVector3T<T>	operator*(const gVector3T<T>& _v3, const gMatrix3T<T>& _m3);
	//矩阵乘法
	gMatrix3T<T> operator*(const gMatrix3T& _m3) const;
	//标量除法
	gMatrix3T<T> operator/(const T& _s) const;
	//乘等于
	void operator*=(const T& _s);
	//求代数余子式
	gMatrix3T<T> GetCofactorMatrix() const;
	//求逆矩阵
	gMatrix3T<T> GetInverse() const;
	//求转置矩阵
	gMatrix3T<T> GetTranspose() const;
	//转为转置矩阵
	void toTranspose();
	//计算行列式
	T	det() const;
	//转为单位矩阵
	void toIdentity();
	//取下标偏移
	gVector3T<T>& operator[](unsigned _i) const;
	//返回第i(0,1,2)行
	gVector3T<T>	row(unsigned _i) const;
	//返回第j(0,1,2)列
	gVector3T<T>	column(unsigned _j) const;
	//设置第j(0,1,2)列
	void			setColumn(unsigned _j, const gVector3T<T>& _v3);
	//比较两个矩阵是否相同
	bool operator ==(const gMatrix3T<T>& _mat);

	T _11;
	T _12;
	T _13;
	T _21;
	T _22;
	T _23;
	T _31;
	T _32;
	T _33;
};

//4阶矩阵
template <typename T>
struct  gMatrix4T
{
	gMatrix4T();
	template <typename U>
	gMatrix4T(const gVector4T<U>& _v1, const gVector4T<U>& _v2, const gVector4T<U>& _v3, const gVector4T<U>& _v4);
	template <typename U>
	gMatrix4T(const gMatrix4T<U>& _m);
	gMatrix4T(	T _s11, T _s12, T _s13, T _s14,
				T _s21, T _s22, T _s23, T _s24,
				T _s31, T _s32, T _s33, T _s34,
				T _s41, T _s42, T _s43, T _s44);

	//加法
	gMatrix4T<T> operator+(const gMatrix4T& _m4) const;
	//减法
	gMatrix4T<T> operator-(const gMatrix4T& _m4) const;
	//加等于
	void operator+=(const gMatrix4T& _m4);
	//减等于
	void operator-=(const gMatrix4T& _m4);
	//乘等于
	void operator*=(const gMatrix4T& _m4);
	//标量乘法
	gMatrix4T<T> operator*(const T& _s) const;
	template <typename T>
	friend gMatrix4T<T>	operator*(const double& _s, const gMatrix4T<T>& _m4);
	//同向量乘法
	gVector4T<T> operator*(const gVector4T<T>& _v4) const;
	template <typename T>
	friend gVector4T<T>	operator*(const gVector4T<T>& _v4, const gMatrix4T<T>& _m4);
	//矩阵乘法
	gMatrix4T<T> operator*(const gMatrix4T& _m4) const;
	//标量除法
	gMatrix4T<T> operator/(const T& _s) const;
	//乘等于
	void operator*=(const T& _s);
	//求代数余子式
	//_si,_sj从0开始
	T	ComputerCof(unsigned _si, unsigned _sj) const;
	gMatrix4T<T> GetCofactorMatrix() const;
	//求逆矩阵
	gMatrix4T<T> GetInverse() const;
	//求转置矩阵
	gMatrix4T GetTranspose() const;
	//转为转置矩阵
	void toTranspose();
	//计算行列式
	T	det() const;
	//转为单位矩阵
	void toIdentity();
	//取下标偏移
	gVector4T<T>& operator[](unsigned _i) const;
	//返回第i(0,1,2,3)行
	gVector4T<T>	row(unsigned _i) const;
	//返回第j(0,1,2,3)列
	gVector4T<T>	column(unsigned _j) const;
	//设置第j(0,1,2,3)列
	void			setColumn(unsigned _j, const gVector4T<T>& _v4);
	//比较两个矩阵是否相同
	bool operator == (const gMatrix4T<T>& _mat);

	T _11;
	T _12;
	T _13;
	T _14;
	T _21;
	T _22;
	T _23;
	T _24;
	T _31;
	T _32;
	T _33;
	T _34;
	T _41;
	T _42;
	T _43;
	T _44;
};

template <typename T>
gMatrix2T<T>::gMatrix2T()
	:_11(1), _12(0), _21(0), _22(1)
{

}

template <typename T>
template <typename U>
gMatrix2T<T>::gMatrix2T(const gMatrix2T<U>& _m)
	:_11(_m._11), _12(_m._12), _21(_m._21), _22(_m._22)
{

}


template <typename T>
template <typename U>
gMatrix2T<T>::gMatrix2T(const gVector2T<U>& _v1, const gVector2T<U>& _v2)
	:_11(_v1.x), _12(_v1.y), _21(_v2.x), _22(_v2.y)
{

}

template <typename T>
gMatrix2T<T>::gMatrix2T(T _s11, T _s12, T _s21, T _s22)
	:_11(_s11), _12(_s12), _21(_s21), _22(_s22)
{

}

template <typename T>
gMatrix2T<T> gMatrix2T<T>::operator+(const gMatrix2T& _m2) const
{
	gMatrix2T<T> m;
	m._11 = _11 + _m2._11;
	m._12 = _12 + _m2._12;
	m._21 = _21 + _m2._21;
	m._22 = _22 + _m2._22;
	return m;
}

template <typename T>
gMatrix2T<T> gMatrix2T<T>::operator-(const gMatrix2T& _m2) const
{
	gMatrix2T<T> m;
	m._11 = _11 - _m2._11;
	m._12 = _12 - _m2._12;
	m._21 = _21 - _m2._21;
	m._22 = _22 - _m2._22;
	return m;
}

template <typename T>
void gMatrix2T<T>::operator+=(const gMatrix2T& _m2)
{
	_11 += _m2._11;
	_12 += _m2._12;
	_21 += _m2._21;
	_22 += _m2._22;
}

template <typename T>
void gMatrix2T<T>::operator-=(const gMatrix2T& _m2)
{
	_11 -= _m2._11;
	_12 -= _m2._12;
	_21 -= _m2._21;
	_22 -= _m2._22;
}

template <typename T>
void gMatrix2T<T>::operator*=(const gMatrix2T& _m2)
{
	gMatrix2T<T> m;
	m._11 = _11*_m2._11 + _12*_m2._21;
	m._12 = _11*_m2._12 + _12*_m2._22;

	m._21 = _21*_m2._11 + _22*_m2._21;
	m._22 = _21*_m2._12 + _22*_m2._22;
	*this = m;
}


template <typename T>
gMatrix2T<T> gMatrix2T<T>::operator*(const T& _s) const
{
	gMatrix2T<T> m;
	m._11 *= _s;
	m._12 *= _s;
	m._21 *= _s;
	m._22 *= _s;
	return m;
}

template <typename T>
gVector2T<T> gMatrix2T<T>::operator*(const gVector2T<T>& _v2) const
{
	gVector2T<T> v;
	v.x = _v2.x*_m2._11 + _v2.y*_m2._12;
	v.y = _v2.x*_m2._21 + _v2.y*_m2._22;
	return v;
}

template <typename T>
gVector2T<T> operator*(const gVector2T<T>& _v2, const gMatrix2T<T>& _m2)
{
	gVector2T<T> v;
	v.x = _v2.x*_m2._11 + _v2.y*_m2._21;
	v.y = _v2.x*_m2._12 + _v2.y*_m2._22;
	return v;
}

//标量乘法
template <typename T>
gMatrix2T<T> operator*(const double& _s, const gMatrix2T<T>& _m2)
{
	return gMatrix2T<T>(_m2._11*_s, _m2._12*_s, _m2._21*_s, _m2._22*_s);
}

template <typename T>
gMatrix2T<T> gMatrix2T<T>::operator*(const gMatrix2T& _m2) const
{
	gMatrix2T<T> m;
	m._11 = _11*_m2._11 + _12*_m2._21;
	m._12 = _11*_m2._12 + _12*_m2._22;

	m._21 = _21*_m2._11 + _22*_m2._21;
	m._22 = _21*_m2._12 + _22*_m2._22;
	return m;
}

template <typename T>
gMatrix2T<T> gMatrix2T<T>::operator/(const T& _s) const
{
	gMatrix2T<T> m;
	m._11 = _11 / _s;
	m._12 = _12 / _s;
	m._21 = _21 / _s;
	m._22 = _22 / _s;
	return m;
}

template <typename T>
void gMatrix2T<T>::operator*=(const T& _s)
{
	_11 *= _s;
	_12 *= _s;
	_21 *= _s;
	_22 *= _s;
}

template <typename T>
gMatrix2T<T> gMatrix2T<T>::GetCofactorMatrix() const
{
	return gMatrix2T<T>(_22, -_12, -_21, _11);
}

template <typename T>
gMatrix2T<T> gMatrix2T<T>::GetInverse() const
{
	gMatrix2T<T> inv = GetCofactorMatrix() / det();
	return inv;
}

template <typename T>
gMatrix2T<T> gMatrix2T<T>::GetTranspose() const
{
	return gMatrix2T<T>(_11, _21, _12, _22);
}

template <typename T>
void gMatrix2T<T>::toTranspose()
{
	T swap_data = _12;
	_12 = _21;
	_21 = swap_data;
}

template <typename T>
T gMatrix2T<T>::det() const
{
	return _11*_22 - _12*_21;
}

template <typename T>
void gMatrix2T<T>::toIdentity()
{
	_11 = 1;
	_12 = 0;
	_21 = 0;
	_22 = 1;
}

template <typename T>
gVector2T<T>& gMatrix2T<T>::operator[](unsigned _i)  const
{
	return ((gVector2T<T>*)_11)[_i];
}

template <typename T>
gVector2T<T> gMatrix2T<T>::row(unsigned _i) const
{
	switch (_i)
	{
	case 0:
		return gVector2T<T>(_11, _12);
	case 1:
		return gVector2T<T>(_21, _22);
	default:
		return gVector2T<T>();
	}
}

template <typename T>
gVector2T<T> gMatrix2T<T>::column(unsigned _j) const
{
	switch (_j)
	{
	case 0:
		return gVector2T<T>(_11, _21);
	case 1:
		return gVector2T<T>(_12, _22);
	default:
		return gVector2T<T>();
	}
}

template <typename T>
void gMatrix2T<T>::setColumn(unsigned _j, const gVector2T<T>& _v2)
{
	switch (_j)
	{
	case 0:
		_11 = _v2.x, _21 = _v2.y;
		break;
	case 1:
		_12 = _v2.x, _22 = _v2.y;
		break;
	default:
		break;
	}
}

template <typename T>
bool gMatrix2T<T>::operator ==(const gMatrix2T<T>& _mat)
{
	if (*this[0] == _mat[0] && *this[1] == _mat[1])
		return true;
	else
		return false;
}

template <typename T>
gMatrix3T<T>::gMatrix3T()
	:_11(1), _12(0), _13(0), _21(0), _22(1), _23(0), _31(0), _32(0), _33(1)
{

}

template <typename T>
template <typename U>
gMatrix3T<T>::gMatrix3T(const gVector3T<U>& _v1, const gVector3T<U>& _v2, const gVector3T<U>& _v3)
	:_11(_v1.x), _12(_v1.y), _13(_v1.z), _21(_v2.x), _22(_v2.y), _23(_v2.z), _31(_v3.x), _32(_v3.y), _33(_v3.z)
{

}

template <typename T>
template <typename U>
gMatrix3T<T>::gMatrix3T(const gMatrix3T<U>& _m)
	:_11(_m._11), _12(_m._12), _13(_m._13), _21(_m._21), _22(_m._22), _23(_m._23), _31(_m._31), _32(_m._32), _33(_m._33)
{

}

template <typename T>
gMatrix3T<T>::gMatrix3T(T _s11, T _s12, T _s13, T _s21, T _s22, T _s23, T _s31, T _s32, T _s33)
	:_11(_s11), _12(_s12), _13(_s13), _21(_s21), _22(_s22), _23(_s23), _31(_s31), _32(_s32), _33(_s33)
{

}

template <typename T>
gMatrix3T<T> gMatrix3T<T>::operator+(const gMatrix3T& _m3) const
{
	gMatrix3T<T> m;
	m._11 = _11 + _m3._11;
	m._12 = _12 + _m3._12;
	m._13 = _13 + _m3._13;

	m._21 = _21 + _m3._21;
	m._22 = _22 + _m3._22;
	m._23 = _23 + _m3._23;

	m._31 = _31 + _m3._31;
	m._32 = _32 + _m3._32;
	m._33 = _33 + _m3._33;
	return m;
}

template <typename T>
gMatrix3T<T> gMatrix3T<T>::operator-(const gMatrix3T& _m3) const
{
	gMatrix3T<T> m;
	m._11 = _11 - _m3._11;
	m._12 = _12 - _m3._12;
	m._13 = _13 - _m3._13;

	m._21 = _21 - _m3._21;
	m._22 = _22 - _m3._22;
	m._23 = _23 - _m3._23;

	m._31 = _31 - _m3._31;
	m._32 = _32 - _m3._32;
	m._33 = _33 - _m3._33;
	return m;
}

template <typename T>
void gMatrix3T<T>::operator+=(const gMatrix3T& _m3)
{
	_11 += _m3._11;
	_12 += _m3._12;
	_13 += _m3._13;

	_21 += _m3._21;
	_22 += _m3._22;
	_23 += _m3._23;

	_31 += _m3._31;
	_32 += _m3._32;
	_33 += _m3._33;
}

template <typename T>
void gMatrix3T<T>::operator-=(const gMatrix3T& _m3)
{
	_11 -= _m3._11;
	_12 -= _m3._12;
	_13 -= _m3._13;

	_21 -= _m3._21;
	_22 -= _m3._22;
	_23 -= _m3._23;

	_31 -= _m3._31;
	_32 -= _m3._32;
	_33 -= _m3._33;
}

template <typename T>
void gMatrix3T<T>::operator*=(const gMatrix3T& _m3)
{
	gMatrix3T<T> m;
	m._11 = _11*_m3._11 + _12*_m3._21 + _13*_m3._31;
	m._12 = _11*_m3._12 + _12*_m3._22 + _13*_m3._32;
	m._13 = _11*_m3._13 + _12*_m3._23 + _13*_m3._33;

	m._21 = _21*_m3._11 + _22*_m3._21 + _23*_m3._31;
	m._22 = _21*_m3._12 + _22*_m3._22 + _23*_m3._32;
	m._23 = _21*_m3._13 + _22*_m3._23 + _23*_m3._33;

	m._31 = _31*_m3._11 + _32*_m3._21 + _33*_m3._31;
	m._32 = _31*_m3._12 + _32*_m3._22 + _33*_m3._32;
	m._33 = _31*_m3._13 + _32*_m3._23 + _33*_m3._33;
	*this = m;
}

template <typename T>
gMatrix3T<T> operator*(const double& _s, const gMatrix3T<T>& _m3)
{
	gMatrix3T<T> m;
	m._11 = _s*_m3._11;
	m._12 = _s*_m3._12;
	m._13 = _s*_m3._13;

	m._21 = _s*_m3._21;
	m._22 = _s*_m3._22;
	m._23 = _s*_m3._23;

	m._31 = _s*_m3._31;
	m._32 = _s*_m3._32;
	m._33 = _s*_m3._33;
	return m;
}

template <typename T>
gMatrix3T<T> gMatrix3T<T>::operator*(const T& _s) const
{
	gMatrix3T<T> m;
	m._11 = _s*_11;
	m._12 = _s*_12;
	m._13 = _s*_13;

	m._21 = _s*_21;
	m._22 = _s*_22;
	m._23 = _s*_23;

	m._31 = _s*_31;
	m._32 = _s*_32;
	m._33 = _s*_33;
	return m;
}

template <typename T>
gVector3T<T> operator*(const gVector3T<T>& _v3, const gMatrix3T<T>& _m3)
{
	gVector3T<T> v;
	v.x = _v3.x*_m3._11 + _v3.y*_m3._21 + _v3.z*_m3._31;
	v.y = _v3.x*_m3._12 + _v3.y*_m3._22 + _v3.z*_m3._32;
	v.z = _v3.x*_m3._13 + _v3.y*_m3._23 + _v3.z*_m3._33;
	return v;
}

template <typename T>
gVector3T<T> gMatrix3T<T>::operator*(const gVector3T<T>& _v3) const
{
	gVector3T<T> v;
	v.x = _11*_v3.x + _12*_v3.y + _13*_v3.z;
	v.y = _21*_v3.x + _22*_v3.y + _23*_v3.z;
	v.z = _31*_v3.x + _32*_v3.y + _33*_v3.z;
	return v;
}

template <typename T>
gMatrix3T<T> gMatrix3T<T>::operator*(const gMatrix3T& _m3) const
{
	gMatrix3T<T> m;
	m._11 = _11*_m3._11 + _12*_m3._21 + _13*_m3._31;
	m._12 = _11*_m3._12 + _12*_m3._22 + _13*_m3._32;
	m._13 = _11*_m3._13 + _12*_m3._23 + _13*_m3._33;

	m._21 = _21*_m3._11 + _22*_m3._21 + _23*_m3._31;
	m._22 = _21*_m3._12 + _22*_m3._22 + _23*_m3._32;
	m._23 = _21*_m3._13 + _22*_m3._23 + _23*_m3._33;

	m._31 = _31*_m3._11 + _32*_m3._21 + _33*_m3._31;
	m._32 = _31*_m3._12 + _32*_m3._22 + _33*_m3._32;
	m._33 = _31*_m3._13 + _32*_m3._23 + _33*_m3._33;
	return m;
}

template <typename T>
gMatrix3T<T> gMatrix3T<T>::operator/(const T& _s) const
{
	gMatrix3T<T> m;
	m._11 = _11 / _s;
	m._12 = _12 / _s;
	m._13 = _13 / _s;

	m._21 = _21 / _s;
	m._22 = _22 / _s;
	m._23 = _23 / _s;

	m._31 = _31 / _s;
	m._32 = _32 / _s;
	m._33 = _33 / _s;
	return m;
}

template <typename T>
void gMatrix3T<T>::operator*=(const T& _s)
{
	_11 *= _s;
	_12 *= _s;
	_13 *= _s;

	_21 *= _s;
	_22 *= _s;
	_23 *= _s;

	_31 *= _s;
	_32 *= _s;
	_33 *= _s;
}

template <typename T>
gMatrix3T<T> gMatrix3T<T>::GetCofactorMatrix() const
{
	gMatrix3T<T> cof;
	cof._11 = (_22*_33 - _23*_32);
	cof._12 = -(_12*_33 - _13*_32);
	cof._13 = (_12*_23 - _13*_22);

	cof._21 = -(_21*_33 - _23*_31);
	cof._22 = (_11*_33 - _13*_31);
	cof._23 = -(_11*_23 - _13*_21);

	cof._31 = (_21*_32 - _22*_31);
	cof._32 = -(_11*_32 - _12*_31);
	cof._33 = (_11*_22 - _12*_21);
	return cof;
}

template <typename T>
gMatrix3T<T> gMatrix3T<T>::GetInverse() const
{
	gMatrix3T<T> m;// = GetCofactorMatrix() / det();
	gMatrix3T<T> Cof = GetCofactorMatrix();
	m = Cof / row(0).Dot(Cof.column(0));
	return m;
}

template <typename T>
gMatrix3T<T> gMatrix3T<T>::GetTranspose() const
{
	gMatrix3T<T> m(_11, _21, _31, _12, _22, _32, _13, _23, _33);
	return m;
}

template <typename T>
void gMatrix3T<T>::toTranspose()
{
	T swap_data = _12;
	_12 = _21; _21 = swap_data;
	swap_data = _13;
	_13 = _31; _31 = swap_data;
	swap_data = _23;
	_23 = _32; _32 = swap_data;
}

template <typename T>
T gMatrix3T<T>::det() const
{
	return (_11*(_22*_33 - _23*_32) - _12*(_21*_33 - _23*_31) + _13*(_21*_32 - _22*_31));
}

template <typename T>
void gMatrix3T<T>::toIdentity()
{
	_11 = 1;
	_12 = 0;
	_13 = 0;
	_21 = 0;
	_22 = 1;
	_23 = 0;
	_31 = 0;
	_32 = 0;
	_33 = 1;
}

template <typename T>
gVector3T<T>& gMatrix3T<T>::operator[](unsigned _i)  const
{
	return ((gVector3T<T>*)&_11)[_i];
}

template <typename T>
gVector3T<T> gMatrix3T<T>::row(unsigned _i) const
{
	switch (_i)
	{
	case 0:
		return gVector3T<T>(_11, _12, _13);
	case 1:
		return gVector3T<T>(_21, _22, _23);
	case 2:
		return gVector3T<T>(_31, _32, _33);
	default:
		return gVector3T<T>();
	}
}

template <typename T>
gVector3T<T> gMatrix3T<T>::column(unsigned _j) const
{
	switch (_j)
	{
	case 0:
		return gVector3T<T>(_11, _21, _31);
	case 1:
		return gVector3T<T>(_12, _22, _32);
	case 2:
		return gVector3T<T>(_13, _23, _33);
	default:
		return gVector3T<T>();
	}
}

template <typename T>
void gMatrix3T<T>::setColumn(unsigned _j, const gVector3T<T>& _v3)
{
	switch (_j)
	{
	case 0:
		_11 = _v3.x, _21 = _v3.y, _31 = _v3.z;
		break;
	case 1:
		_12 = _v3.x, _22 = _v3.y, _32 = _v3.z;
		break;
	case 2:
		_13 = _v3.x, _23 = _v3.y, _33 = _v3.z;
		break;
	default:
		break;
	}
}

template <typename T>
bool gMatrix3T<T>::operator ==(const gMatrix3T<T>& _mat)
{
	if (*this[0] == _mat[0] && *this[1] == _mat[1] && *this[2] == _mat[2])
		return true;
	else
		return false;
}



template <typename T>
gMatrix4T<T>::gMatrix4T()
	:_11(1), _12(0), _13(0), _14(0), _21(0), _22(1), _23(0), _24(0), _31(0), _32(0), _33(1), _34(0), _41(0), _42(0), _43(0), _44(1)
{

}

template <typename T>
gMatrix4T<T>::gMatrix4T(T _s11, T _s12, T _s13, T _s14, T _s21, T _s22, T _s23, T _s24, T _s31, T _s32, T _s33, T _s34, T _s41, T _s42, T _s43, T _s44)
	: _11(_s11), _12(_s12), _13(_s13), _14(_s14)
	, _21(_s21), _22(_s22), _23(_s23), _24(_s24)
	, _31(_s31), _32(_s32), _33(_s33), _34(_s34)
	, _41(_s41), _42(_s42), _43(_s43), _44(_s44)
{

}

template <typename T>
template <typename U>
gMatrix4T<T>::gMatrix4T(const gVector4T<U>& _v1, const gVector4T<U>& _v2, const gVector4T<U>& _v3, const gVector4T<U>& _v4) :
_11(_v1.x), _12(_v1.y), _13(_v1.z), _14(_v1.w),
_21(_v2.x), _22(_v2.y), _23(_v2.z), _24(_v1.w),
_31(_v3.x), _32(_v3.y), _33(_v3.z), _34(_v1.w),
_41(_v4.x), _42(_v4.y), _43(_v4.z), _44(_v1.w)
{

}

template <typename T>
template <typename U>
gMatrix4T<T>::gMatrix4T(const gMatrix4T<U>& _m)
	: _11(_m._11), _12(_m._12), _13(_m._13), _14(_m._14)
	, _21(_m._21), _22(_m._22), _23(_m._23), _24(_m._24)
	, _31(_m._31), _32(_m._32), _33(_m._33), _34(_m._34)
	, _41(_m._41), _42(_m._42), _43(_m._43), _44(_m._44)
{

}

template <typename T>
gMatrix4T<T> gMatrix4T<T>::operator+(const gMatrix4T& _m4) const
{
	gMatrix4T<T> m;
	m._11 = _11 + _m4._11;
	m._12 = _12 + _m4._12;
	m._13 = _13 + _m4._13;
	m._14 = _14 + _m4._14;

	m._21 = _21 + _m4._21;
	m._22 = _22 + _m4._22;
	m._23 = _23 + _m4._23;
	m._24 = _24 + _m4._24;

	m._31 = _31 + _m4._31;
	m._32 = _32 + _m4._32;
	m._33 = _33 + _m4._33;
	m._34 = _34 + _m4._34;

	m._41 = _41 + _m4._41;
	m._42 = _42 + _m4._42;
	m._43 = _43 + _m4._43;
	m._44 = _44 + _m4._44;
	return m;
}

template <typename T>
gMatrix4T<T> gMatrix4T<T>::operator-(const gMatrix4T& _m4) const
{
	gMatrix4T<T> m;
	m._11 = _11 - _m4._11;
	m._12 = _12 - _m4._12;
	m._13 = _13 - _m4._13;
	m._14 = _14 - _m4._14;

	m._21 = _21 - _m4._21;
	m._22 = _22 - _m4._22;
	m._23 = _23 - _m4._23;
	m._24 = _24 - _m4._24;

	m._31 = _31 - _m4._31;
	m._32 = _32 - _m4._32;
	m._33 = _33 - _m4._33;
	m._34 = _34 - _m4._34;

	m._41 = _41 - _m4._41;
	m._42 = _42 - _m4._42;
	m._43 = _43 - _m4._43;
	m._44 = _44 - _m4._44;
	return m;
}

template <typename T>
void gMatrix4T<T>::operator+=(const gMatrix4T& _m4)
{
	_11 += _m4._11;
	_12 += _m4._12;
	_13 += _m4._13;
	_14 += _m4._14;

	_21 += _m4._21;
	_22 += _m4._22;
	_23 += _m4._23;
	_24 += _m4._24;

	_31 += _m4._31;
	_32 += _m4._32;
	_33 += _m4._33;
	_34 += _m4._34;

	_41 += _m4._41;
	_42 += _m4._42;
	_43 += _m4._43;
	_44 += _m4._44;
}

template <typename T>
void gMatrix4T<T>::operator-=(const gMatrix4T& _m4)
{
	_11 -= _m4._11;
	_12 -= _m4._12;
	_13 -= _m4._13;
	_14 -= _m4._14;

	_21 -= _m4._21;
	_22 -= _m4._22;
	_23 -= _m4._23;
	_24 -= _m4._24;

	_31 -= _m4._31;
	_32 -= _m4._32;
	_33 -= _m4._33;
	_34 -= _m4._34;

	_41 -= _m4._41;
	_42 -= _m4._42;
	_43 -= _m4._43;
	_44 -= _m4._44;
}

template <typename T>
void gMatrix4T<T>::operator*=(const gMatrix4T& _m4)
{
	gMatrix4T<T> m;
	m._11 = _11*_m4._11 + _12*_m4._21 + _13*_m4._31 + _14*_m4._41;
	m._12 = _11*_m4._12 + _12*_m4._22 + _13*_m4._32 + _14*_m4._42;
	m._13 = _11*_m4._13 + _12*_m4._23 + _13*_m4._33 + _14*_m4._43;
	m._14 = _11*_m4._14 + _12*_m4._24 + _13*_m4._34 + _14*_m4._44;

	m._21 = _21*_m4._11 + _22*_m4._21 + _23*_m4._31 + _24*_m4._41;
	m._22 = _21*_m4._12 + _22*_m4._22 + _23*_m4._32 + _24*_m4._42;
	m._23 = _21*_m4._13 + _22*_m4._23 + _23*_m4._33 + _24*_m4._43;
	m._24 = _21*_m4._14 + _22*_m4._24 + _23*_m4._34 + _24*_m4._44;

	m._31 = _31*_m4._11 + _32*_m4._21 + _33*_m4._31 + _34*_m4._41;
	m._32 = _31*_m4._12 + _32*_m4._22 + _33*_m4._32 + _34*_m4._42;
	m._33 = _31*_m4._13 + _32*_m4._23 + _33*_m4._33 + _34*_m4._43;
	m._34 = _31*_m4._14 + _32*_m4._24 + _33*_m4._34 + _34*_m4._44;

	m._41 = _41*_m4._11 + _42*_m4._21 + _43*_m4._31 + _44*_m4._41;
	m._42 = _41*_m4._12 + _42*_m4._22 + _43*_m4._32 + _44*_m4._42;
	m._43 = _41*_m4._13 + _42*_m4._23 + _43*_m4._33 + _44*_m4._43;
	m._44 = _41*_m4._14 + _42*_m4._24 + _43*_m4._34 + _44*_m4._44;
	*this = m;
}

template <typename T>
gMatrix4T<T> gMatrix4T<T>::operator*(const T& _s) const
{
	gMatrix4T<T> m;
	m._11 = _s*_11;
	m._12 = _s*_12;
	m._13 = _s*_13;
	m._14 = _s*_14;

	m._21 = _s*_21;
	m._22 = _s*_22;
	m._23 = _s*_23;
	m._24 = _s*_24;

	m._31 = _s*_31;
	m._32 = _s*_32;
	m._33 = _s*_33;
	m._34 = _s*_34;

	m._41 = _s*_41;
	m._42 = _s*_42;
	m._43 = _s*_43;
	m._44 = _s*_44;
	return m;
}

template <typename T>
gMatrix4T<T> operator*(const double & _s, const gMatrix4T<T> & _m4)
{
	gMatrix4T<T> m;
	m._11 = _s*_m4._11;
	m._12 = _s*_m4._12;
	m._13 = _s*_m4._13;
	m._14 = _s*_m4._14;

	m._21 = _s*_m4._21;
	m._22 = _s*_m4._22;
	m._23 = _s*_m4._23;
	m._24 = _s*_m4._24;

	m._31 = _s*_m4._31;
	m._32 = _s*_m4._32;
	m._33 = _s*_m4._33;
	m._34 = _s*_m4._34;

	m._41 = _s*_m4._41;
	m._42 = _s*_m4._42;
	m._43 = _s*_m4._43;
	m._44 = _s*_m4._44;
	return m;
}

template <typename T>
gVector4T<T> gMatrix4T<T>::operator*(const gVector4T<T>& _v4) const
{
	gVector4T<T> v;
	v.x = _11*_v4.x + _12*_v4.y + _13*_v4.z + _14*_v4.w;
	v.y = _21*_v4.x + _22*_v4.y + _23*_v4.z + _24*_v4.w;
	v.z = _31*_v4.x + _32*_v4.y + _33*_v4.z + _34*_v4.w;
	v.w = _41*_v4.x + _42*_v4.y + _43*_v4.z + _44*_v4.w;
	return v;
}

template <typename T>
gVector4T<T> operator*(const gVector4T<T> & _v4, const gMatrix4T<T>& _m4)
{
	gVector4T<T> v;
	v.x = _v4.x*_m4._11 + _v4.y*_m4._21 + _v4.z*_m4._31 + _v4.w*_m4._41;
	v.y = _v4.x*_m4._12 + _v4.y*_m4._22 + _v4.z*_m4._32 + _v4.w*_m4._42;
	v.z = _v4.x*_m4._13 + _v4.y*_m4._23 + _v4.z*_m4._33 + _v4.w*_m4._43;
	v.w = _v4.x*_m4._14 + _v4.y*_m4._24 + _v4.z*_m4._34 + _v4.w*_m4._44;
	return v;
}

template <typename T>
gMatrix4T<T> gMatrix4T<T>::operator*(const gMatrix4T& _m4) const
{
	gMatrix4T<T> m;
	m._11 = _11*_m4._11 + _12*_m4._21 + _13*_m4._31 + _14*_m4._41;
	m._12 = _11*_m4._12 + _12*_m4._22 + _13*_m4._32 + _14*_m4._42;
	m._13 = _11*_m4._13 + _12*_m4._23 + _13*_m4._33 + _14*_m4._43;
	m._14 = _11*_m4._14 + _12*_m4._24 + _13*_m4._34 + _14*_m4._44;

	m._21 = _21*_m4._11 + _22*_m4._21 + _23*_m4._31 + _24*_m4._41;
	m._22 = _21*_m4._12 + _22*_m4._22 + _23*_m4._32 + _24*_m4._42;
	m._23 = _21*_m4._13 + _22*_m4._23 + _23*_m4._33 + _24*_m4._43;
	m._24 = _21*_m4._14 + _22*_m4._24 + _23*_m4._34 + _24*_m4._44;

	m._31 = _31*_m4._11 + _32*_m4._21 + _33*_m4._31 + _34*_m4._41;
	m._32 = _31*_m4._12 + _32*_m4._22 + _33*_m4._32 + _34*_m4._42;
	m._33 = _31*_m4._13 + _32*_m4._23 + _33*_m4._33 + _34*_m4._43;
	m._34 = _31*_m4._14 + _32*_m4._24 + _33*_m4._34 + _34*_m4._44;

	m._41 = _41*_m4._11 + _42*_m4._21 + _43*_m4._31 + _44*_m4._41;
	m._42 = _41*_m4._12 + _42*_m4._22 + _43*_m4._32 + _44*_m4._42;
	m._43 = _41*_m4._13 + _42*_m4._23 + _43*_m4._33 + _44*_m4._43;
	m._44 = _41*_m4._14 + _42*_m4._24 + _43*_m4._34 + _44*_m4._44;

	return m;
}

template <typename T>
gMatrix4T<T> gMatrix4T<T>::operator/(const T& _s) const
{
	gMatrix4T<T> m;
	m._11 = _11 / _s;
	m._12 = _12 / _s;
	m._13 = _13 / _s;
	m._14 = _14 / _s;

	m._21 = _21 / _s;
	m._22 = _22 / _s;
	m._23 = _23 / _s;
	m._24 = _24 / _s;

	m._31 = _31 / _s;
	m._32 = _32 / _s;
	m._33 = _33 / _s;
	m._34 = _34 / _s;

	m._41 = _41 / _s;
	m._42 = _42 / _s;
	m._43 = _43 / _s;
	m._44 = _44 / _s;
	return m;
}

template <typename T>
void gMatrix4T<T>::operator*=(const T& _s)
{
	_11 *= _s;
	_12 *= _s;
	_13 *= _s;
	_14 *= _s;

	_21 *= _s;
	_22 *= _s;
	_23 *= _s;
	_24 *= _s;

	_31 *= _s;
	_32 *= _s;
	_33 *= _s;
	_34 *= _s;

	_41 *= _s;
	_42 *= _s;
	_43 *= _s;
	_44 *= _s;
}

template<typename T>
T gMatrix4T<T>::ComputerCof(unsigned _si, unsigned _sj) const
{
	gMatrix3T<T> ao;
	const T* s_pt = &_11;
	T* d_pt = &ao._11;
	for (size_t i = 0; i < 3; i++)
	{
		int si = i < _si ? i : i + 1;
		for (size_t j = 0; j < 3; j++)
		{
			int sj = j < _sj ? j : j + 1;
			d_pt[3 * i + j] = s_pt[4 * si + sj];
		}
	}
	short _sign = ((_si + _sj) % 2) ? -1 : 1;
	return _sign* ao.det();
}

template <typename T>
gMatrix4T<T> gMatrix4T<T>::GetCofactorMatrix() const
{
	gMatrix4T<T> cof;
	cof._11 = ComputerCof(0, 0);
	cof._12 = ComputerCof(1, 0);
	cof._13 = ComputerCof(2, 0);
	cof._14 = ComputerCof(3, 0);
	cof._21 = ComputerCof(0, 1);
	cof._22 = ComputerCof(1, 1);
	cof._23 = ComputerCof(2, 1);
	cof._24 = ComputerCof(3, 1);
	cof._31 = ComputerCof(0, 2);
	cof._32 = ComputerCof(1, 2);
	cof._33 = ComputerCof(2, 2);
	cof._34 = ComputerCof(3, 2);
	cof._41 = ComputerCof(0, 3);
	cof._42 = ComputerCof(1, 3);
	cof._43 = ComputerCof(2, 3);
	cof._44 = ComputerCof(3, 3);
	return cof;
}

template <typename T>
gMatrix4T<T> gMatrix4T<T>::GetInverse() const
{
	gMatrix4T<T> m;// = GetCofactorMatrix() / det();
	gMatrix4T<T> Cof = GetCofactorMatrix();
	m = Cof / row(0).Dot(Cof.column(0));
	return m;
}

template <typename T>
gMatrix4T<T> gMatrix4T<T>::GetTranspose() const
{
	gMatrix4T<T> m(_11, _21, _31, _41, _12, _22, _32, _42, _13, _23, _33, _43, _14, _24, _34, _44);
	return m;
}

template <typename T>
void gMatrix4T<T>::toTranspose()
{
	T swap_data = _12;
	_12 = _21; _21 = swap_data;
	swap_data = _13;
	_13 = _31; _31 = swap_data;
	swap_data = _14;
	_14 = _41; _41 = swap_data;
	swap_data = _23;
	_23 = _32; _32 = swap_data;
	swap_data = _24;
	_24 = _42; _42 = swap_data;
	swap_data = _34;
	_34 = _43; _43 = swap_data;
}

template <typename T>
T gMatrix4T<T>::det() const
{
	return _11*ComputerCof(0, 0) + _12*ComputerCof(0, 1) + _13*ComputerCof(0, 2) + _14*ComputerCof(0, 3);
}

template <typename T>
void gMatrix4T<T>::toIdentity()
{
	_11 = 1;
	_12 = 0;
	_13 = 0;
	_14 = 0;
	_21 = 0;
	_22 = 1;
	_23 = 0;
	_24 = 0;
	_31 = 0;
	_32 = 0;
	_33 = 1;
	_34 = 0;
	_31 = 0;
	_32 = 0;
	_33 = 0;
	_34 = 1;
}

template <typename T>
gVector4T<T>& gMatrix4T<T>::operator[](unsigned _i)  const
{
	return ((gVector4T<T>*)&_11)[_i];
}

template <typename T>
gVector4T<T> gMatrix4T<T>::row(unsigned _i) const
{
	switch (_i)
	{
	case 0:
		return gVector4T<T>(_11, _12, _13, _14);
	case 1:
		return gVector4T<T>(_21, _22, _23, _24);
	case 2:
		return gVector4T<T>(_31, _32, _33, _34);
	case 3:
		return gVector4T<T>(_41, _42, _43, _44);
	default:
		return gVector4T<T>();
	}
}

template <typename T>
gVector4T<T> gMatrix4T<T>::column(unsigned _j) const
{
	switch (_j)
	{
	case 0:
		return gVector4T<T>(_11, _21, _31, _41);
	case 1:
		return gVector4T<T>(_12, _22, _32, _42);
	case 2:
		return gVector4T<T>(_13, _23, _33, _43);
	case 3:
		return gVector4T<T>(_14, _24, _34, _44);
	default:
		return gVector4T<T>();
	}
}

template <typename T>
void gMatrix4T<T>::setColumn(unsigned _j, const gVector4T<T>& _v4)
{
	switch (_j)
	{
	case 0:
		_11 = _v4.x, _21 = _v4.y, _31 = _v4.z, _41 = _v4.w;
		break;
	case 1:
		_12 = _v4.x, _22 = _v4.y, _32 = _v4.z, _42 = _v4.w;
		break;
	case 2:
		_13 = _v4.x, _23 = _v4.y, _33 = _v4.z, _43 = _v4.w;
		break;
	case 3:
		_14 = _v4.x, _24 = _v4.y, _34 = _v4.z, _44 = _v4.w;
		break;
	default:
		break;
	}
}

template <typename T>
bool gMatrix4T<T>::operator == (const gMatrix4T<T>& _mat)
{
	if (*this[0] == _mat[0] && *this[1] == _mat[1] && *this[2] == _mat[2] && *this[3] == _mat[3])
		return true;
	else
		return false;
}


typedef gMatrix2T<char>				gMatrix2c;
typedef gMatrix2T<short>			gMatrix2s;
typedef gMatrix2T<int>				gMatrix2i;
typedef gMatrix2T<unsigned char>	gMatrix2uc;
typedef gMatrix2T<unsigned short>	gMatrix2us;
typedef gMatrix2T<unsigned int>		gMatrix2ui;
typedef gMatrix2T<float>			gMatrix2f;
typedef gMatrix2T<double>			gMatrix2d;

typedef gMatrix3T<char>				gMatrix3c;
typedef gMatrix3T<short>			gMatrix3s;
typedef gMatrix3T<int>				gMatrix3i;
typedef gMatrix3T<unsigned char>	gMatrix3uc;
typedef gMatrix3T<unsigned short>	gMatrix3us;
typedef gMatrix3T<unsigned int>		gMatrix3ui;
typedef gMatrix3T<float>			gMatrix3f;
typedef gMatrix3T<double>			gMatrix3d;

typedef gMatrix4T<char>				gMatrix4c;
typedef gMatrix4T<short>			gMatrix4s;
typedef gMatrix4T<int>				gMatrix4i;
typedef gMatrix4T<unsigned char>	gMatrix4uc;
typedef gMatrix4T<unsigned short>	gMatrix4us;
typedef gMatrix4T<unsigned int>		gMatrix4ui;
typedef gMatrix4T<float>			gMatrix4f;
typedef gMatrix4T<double>			gMatrix4d;

#pragma region 矩阵计算

template <typename T>
gMatrix3T<T>  MatrixUpOrder3(const gMatrix2T<T>& _m2)
{
	gMatrix3T<T> out_m;
	out_m._11 = _m2._11;
	out_m._12 = _m2._12;
	out_m._21 = _m2._21;
	out_m._22 = _m2._22;
	return out_m;
}

template <typename T>
gMatrix4T<T>  MatrixUpOrder4(const gMatrix2T<T>& _m3)
{
	gMatrix4T<T> out_m;
	out_m._11 = _m2._11;
	out_m._12 = _m2._12;
	out_m._21 = _m2._21;
	out_m._22 = _m2._22;
	return out_m;
}

template <typename T>
gMatrix4T<T>  MatrixUpOrder4(const gMatrix3T<T>& _m3)
{
	gMatrix4T<T> out_m;
	out_m._11 = _m3._11;
	out_m._12 = _m3._12;
	out_m._13 = _m3._13;
	out_m._21 = _m3._21;
	out_m._22 = _m3._22;
	out_m._23 = _m3._23;
	out_m._31 = _m3._31;
	out_m._32 = _m3._32;
	out_m._33 = _m3._33;
	return out_m;
}

//区分矩阵左乘右乘
template <typename T>
gMatrix4T<T>  MatrixUpOrder4(const gMatrix3T<T>& _m3, const gVector3T<T>& _v3, GMath::MatrxMulOrder _mul_ord = GMath::mm_order_)
{
	gMatrix4T<T> out_m;
	out_m._11 = _m3._11;
	out_m._12 = _m3._12;
	out_m._13 = _m3._13;
	out_m._21 = _m3._21;
	out_m._22 = _m3._22;
	out_m._23 = _m3._23;
	out_m._31 = _m3._31;
	out_m._32 = _m3._32;
	out_m._33 = _m3._33;
	if (_mul_ord == GMath::LEFT_MUL){
		out_m._14 = _v3.x;
		out_m._24 = _v3.y;
		out_m._34 = _v3.z;
	}
	else{
		//常用右乘
		out_m._41 = _v3.x;
		out_m._42 = _v3.y;
		out_m._43 = _v3.z;
	}
	return out_m;
}

template <typename T>
gMatrix3T<T>  MatrixDownOrder3(const gMatrix4T<T>& _m4, gVector3T<T>* _v3 = nullptr, GMath::MatrxMulOrder _mul_ord = GMath::mm_order_)
{
	gMatrix3T<T> out_m;
	out_m._11 = _m4._11;
	out_m._12 = _m4._12;
	out_m._13 = _m4._13;
	out_m._21 = _m4._21;
	out_m._22 = _m4._22;
	out_m._23 = _m4._23;
	out_m._31 = _m4._31;
	out_m._32 = _m4._32;
	out_m._33 = _m4._33;
	if (_v3)
	{
		if (_mul_ord == GMath::LEFT_MUL)
			*_v3 = { _m4._14, _m4._24, _m4._34 };
		else
			//常用右乘
			*_v3 = { _m4._41, _m4._42, _m4._43 };
	}
	return out_m;
}

//平面旋转,区分矩阵左乘右乘
inline gMatrix2d  Matrix2Rotate(double _rad, GMath::MatrxMulOrder _mul_ord = GMath::mm_order_)
{
	double _cos, _sin;
	if (_mul_ord == GMath::LEFT_MUL)
	{
	}
	else
	{
		//常用右乘
		_rad = -_rad;
	}
	_cos = cos(_rad);
	_sin = sin(_rad);
	return gMatrix2d(_cos, -_sin, _sin, _cos);
}

namespace leftRotate
{
	template <typename T>
	void RotateNormalAxis(T& _mat, const gVector3d& v, const double& _rad)
	{
		double _cos, _sin;
		_cos = cos(_rad);
		_sin = sin(_rad);
		_mat[0][0] = v.x*v.x*(1 - _cos) + _cos;
		_mat[0][1] = v.x*v.y*(1 - _cos) - _sin*v.z;
		_mat[0][2] = v.x*v.z*(1 - _cos) + _sin*v.y;
		_mat[1][0] = v.y*v.x*(1 - _cos) + _sin*v.z;
		_mat[1][1] = v.y*v.y*(1 - _cos) + _cos;
		_mat[1][2] = v.y*v.z*(1 - _cos) - _sin*v.x;
		_mat[2][0] = v.z*v.x*(1 - _cos) - _sin*v.y;
		_mat[2][1] = v.z*v.y*(1 - _cos) + _sin*v.x;
		_mat[2][2] = v.z*v.z*(1 - _cos) + _cos;
	}

	template <typename T>
	void RotateX(T& _mat, const double& _rad)
	{
		double _cos, _sin;
		_cos = cos(_rad);
		_sin = sin(_rad);
		_mat[0][0] = 1;
		_mat[0][1] = 0;
		_mat[0][2] = 0;
		_mat[1][0] = 0;
		_mat[1][1] = _cos;
		_mat[1][2] = -_sin;
		_mat[2][0] = 0;
		_mat[2][1] = _sin;
		_mat[2][2] = _cos;
	}

	template <typename T>
	void RotateY(T& _mat, const double& _rad)
	{
		double _cos, _sin;
		_cos = cos(_rad);
		_sin = sin(_rad);
		_mat[0][0] = _cos;
		_mat[0][1] = 0;
		_mat[0][2] = _sin;
		_mat[1][0] = 0;
		_mat[1][1] = 1;
		_mat[1][2] = 0;
		_mat[2][0] = -_sin;
		_mat[2][1] = 0;
		_mat[2][2] = _cos;
	}

	template <typename T>
	void RotateZ(T& _mat, const double& _rad)
	{
		double _cos, _sin;
		_cos = cos(_rad);
		_sin = sin(_rad);
		_mat[0][0] = _cos;
		_mat[0][1] = -_sin;
		_mat[0][2] = 0;
		_mat[1][0] = _sin;
		_mat[1][1] = _cos;
		_mat[1][2] = 0;
		_mat[2][0] = 0;
		_mat[2][1] = 0;
		_mat[2][2] = 1;
	}
}

//绕指定轴旋转,区分矩阵左乘右乘
inline gMatrix3d  Matrix3RotateAxis(gVector3d v, double _rad, GMath::MatrxMulOrder _mul_ord = GMath::mm_order_)
{
	gMatrix3d m;
	if (_mul_ord == GMath::LEFT_MUL)
	{
	}
	else
	{
		//常用右乘
		_rad = -_rad;
	}
	v.toNormalize();
	leftRotate::RotateNormalAxis(m, v, _rad);
	return m;
}

inline gMatrix4d  Matrix4RotateAxis(gVector3d v, double _rad, GMath::MatrxMulOrder _mul_ord = GMath::mm_order_)
{
	gMatrix4d m;
	if (_mul_ord == GMath::LEFT_MUL)
	{
	}
	else
	{
		//常用右乘
		_rad = -_rad;
	}
	v.toNormalize();
	leftRotate::RotateNormalAxis(m, v, _rad);
	return m;
}

//反算旋转轴
inline double GetRotateAxisAngleFromMatrix3(const gMatrix3d& _rata, gVector3d& v, GMath::MatrxMulOrder _mul_ord = GMath::mm_order_)
{
	double _angle = 0;
	double sin_x, sin_y, sin_z;
	if (_mul_ord == GMath::LEFT_MUL)
	{
		sin_x = (_rata._32 - _rata._23) / 2;
		sin_y = (_rata._13 - _rata._31) / 2;
		sin_z = (_rata._21 - _rata._12) / 2;
	}
	else
	{
		sin_x = (_rata._23 - _rata._32) / 2;
		sin_y = (_rata._31 - _rata._13) / 2;
		sin_z = (_rata._12 - _rata._21) / 2;
	}

	double sin_angle = sqrt(sin_x*sin_x + sin_y*sin_y + sin_z*sin_z);
	_angle = asin(sin_angle);
	v.x = sin_x / sin_angle;
	v.y = sin_y / sin_angle;
	v.z = sin_z / sin_angle;

	return sin_angle;
}

//绕X轴旋转,区分矩阵左乘右乘
inline gMatrix3d  Matrix3RotateX(double _rad, GMath::MatrxMulOrder _mul_ord = GMath::mm_order_)
{
	gMatrix3d m;
	double _cos, _sin;
	if (_mul_ord == GMath::LEFT_MUL)
	{
	}
	else
	{
		//常用右乘
		_rad = -_rad;
	}

	leftRotate::RotateX(m, _rad);
	return m;
}

inline gMatrix4d  Matrix4RotateX(double _rad, GMath::MatrxMulOrder _mul_ord = GMath::mm_order_)
{
	gMatrix4d m;
	double _cos, _sin;
	if (_mul_ord == GMath::LEFT_MUL)
	{
	}
	else
	{
		//常用右乘
		_rad = -_rad;
	}

	leftRotate::RotateX(m, _rad);
	return m;
}

//绕Y轴旋转,区分矩阵左乘右乘
inline gMatrix3d  Matrix3RotateY(double _rad, GMath::MatrxMulOrder _mul_ord = GMath::mm_order_)
{
	gMatrix3d m;
	double _cos, _sin;
	if (_mul_ord == GMath::LEFT_MUL)
	{
	}
	else
	{
		//常用右乘
		_rad = -_rad;
	}

	leftRotate::RotateY(m, _rad);
	return m;
}

inline gMatrix4d  Matrix4RotateY(double _rad, GMath::MatrxMulOrder _mul_ord = GMath::mm_order_)
{
	gMatrix4d m;
	double _cos, _sin;
	if (_mul_ord == GMath::LEFT_MUL)
	{
	}
	else
	{
		//常用右乘
		_rad = -_rad;
	}

	leftRotate::RotateY(m, _rad);
	return m;
}

//绕Z轴旋转,区分矩阵左乘右乘
inline gMatrix3d  Matrix3RotateZ(double _rad, GMath::MatrxMulOrder _mul_ord = GMath::mm_order_)
{
	gMatrix3d m;
	double _cos, _sin;
	if (_mul_ord == GMath::LEFT_MUL)
	{
	}
	else
	{
		//常用右乘
		_rad = -_rad;
	}

	leftRotate::RotateZ(m, _rad);
	return m;
}

inline gMatrix4d  Matrix4RotateZ(double _rad, GMath::MatrxMulOrder _mul_ord = GMath::mm_order_)
{
	gMatrix4d m;
	double _cos, _sin;
	if (_mul_ord == GMath::LEFT_MUL)
	{
	}
	else
	{
		//常用右乘
		_rad = -_rad;
	}

	leftRotate::RotateZ(m, _rad);
	return m;
}

// Transform (x, y, z, 1) by matrix.
template <typename T>
gVector4T<T>	Vec3Transform(const gVector3T<T>& _v, const gMatrix4T<T>& _m, GMath::MatrxMulOrder _mul_ord = GMath::mm_order_)
{
	gVector4T<T> _out = { _v ,1};
	if (_mul_ord == GMath::LEFT_MUL)
		_out = _m * _out;
	else
		_out = _out * _m;
	return _out;
}

// Transform (x, y, z, 1) by matrix, project result back into w=1.
template <typename T>
gVector3T<T>	Vec3TransformCoord(const gVector3T<T>& _v, const gMatrix4T<T>& _m, GMath::MatrxMulOrder _mul_ord = GMath::mm_order_)
{
	gVector4T<T> _out = { _v, 1 };
	gVector3T<T>* _transform = (gVector3T<T>*)_out.ptr();
	gVector4T<T> _unit_sign = {};
	if (_mul_ord == GMath::LEFT_MUL)
	{
		_unit_sign = _m.row(3);
		if (_unit_sign.LengthSquare() == _unit_sign.w*_unit_sign.w)
		{
			//矩阵符合刚体变换特征
			gMatrix3T<T> _m3x3 = MatrixDownOrder3<T>(_m, nullptr, _mul_ord);
			*_transform = (_m3x3 * _v + *(gVector3T<T>*)_unit_sign.ptr())/ _unit_sign.w;
		}
		else
		{
			_out = _m * _out;
			_out /= _out.w;
		}
	}
	else
	{
		_unit_sign = _m.column(3);
		if (_unit_sign.LengthSquare() == _unit_sign.w*_unit_sign.w)
		{
			//矩阵符合刚体变换特征
			gMatrix3T<T> _m3x3 = MatrixDownOrder3<T>(_m, nullptr, _mul_ord);
			*_transform = (_v*_m3x3 + *(gVector3T<T>*)_unit_sign.ptr()) / _unit_sign.w;
		}
		else
		{
			_out = _out * _m;
			_out /= _out.w;
		}
	}
	return *_transform;
}

// Transform (x, y, z, 0) by matrix.
template <typename T>
gVector3T<T>	Vec3TransformNormal(const gVector3T<T>& _v, const gMatrix4T<T>& _m, GMath::MatrxMulOrder _mul_ord = GMath::mm_order_)
{
	gVector3T<T> _out;
	gMatrix3T<T> _m3x3 = MatrixDownOrder3<T>(_m, nullptr, _mul_ord);
	if (_mul_ord == GMath::LEFT_MUL)
		_out = _m * _v;
	else
		_out = _v * _m;
	_out.toNormalize();
	return _out;
}

inline gMatrix4d  Matrix4Scaling(double _scale)
{
	return gMatrix4d(
		_scale, 0, 0, 0,
		0, _scale, 0, 0,
		0, 0, _scale, 0,
		0, 0, 0, 1);
}

inline gMatrix4d  Matrix4Scaling(const gVector3d& _s)
{
	return gMatrix4d(
		_s.x, 0, 0, 0,
		0, _s.y, 0, 0,
		0, 0, _s.z, 0,
		0, 0, 0, 1);
}

inline gMatrix4d  Matrix4Translation(const gVector3d& _translation, GMath::MatrxMulOrder _mul_ord = GMath::mm_order_)
{
	if (_mul_ord == GMath::LEFT_MUL)
	{
		return gMatrix4d(
			1, 0, 0, _translation.x,
			0, 1, 0, _translation.y,
			0, 0, 1, _translation.z,
			0, 0, 0, 1);
	}
	else
	{
		return gMatrix4d(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			_translation.x, _translation.y, _translation.z, 1);
	}
}

//透视投影计算
inline gMatrix4d MatrixPerspectiveFovRH(double fovy, double Aspect, double zn, double zf)
{
	double yScale = 1.0 / tan(fovy / 2);
	double xScale = yScale / Aspect;

	//	return glm::dmat4( xScale,0,0,0,
	//					0,yScale,0,0,
	//					0,0,zf/(zn-zf),-1,
	//					0,0,zn*zf/(zn-zf),0
	//					);
	return gMatrix4d(xScale, 0, 0, 0,
		0, yScale, 0, 0,
		0, 0, zf / (zn - zf), -1,
		0, 0, zn*zf / (zn - zf), 0
	);
}
#pragma endregion


#endif // !GEOMATRIX



