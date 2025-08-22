#pragma once
#include "pch.h"

struct Mat3x3;

class SimpleMathHelper
{
public:
	inline static Vec4 GetRow0(const Matrix& InMatrix)
	{
		return Vec4(InMatrix._11, InMatrix._12, InMatrix._13, InMatrix._14);
	}
	inline static Vec4 GetRow1(const Matrix& InMatrix)
	{
		return Vec4(InMatrix._21, InMatrix._22, InMatrix._23 ,InMatrix._24);
	}
	inline static Vec4 GetRow2(const Matrix& InMatrix)
	{
		return Vec4(InMatrix._31 ,InMatrix._32 ,InMatrix._33 ,InMatrix._34);
	}
	inline static Vec4 GetRow3(const Matrix& InMatrix)
	{
		return Vec4(InMatrix._41 ,InMatrix._42 ,InMatrix._43 ,InMatrix._44);
	}


	inline static Vec4 GetCol0(const Matrix& InMatrix)
	{
		return Vec4(InMatrix._11, InMatrix._21, InMatrix._31, InMatrix._41);
	}
	inline static Vec4 GetCol1(const Matrix& InMatrix)
	{
		return Vec4(InMatrix._12, InMatrix._22, InMatrix._32, InMatrix._42);
	}inline static Vec4 GetCol2(const Matrix& InMatrix)
	{
		return Vec4(InMatrix._13, InMatrix._23, InMatrix._33, InMatrix._43);
	}inline static Vec4 GetCol3(const Matrix& InMatrix)
	{
		return Vec4(InMatrix._14, InMatrix._24, InMatrix._34, InMatrix._44);
	}




	// Rows (0~2, 3열까지만)
	inline static Vec3 Get3Row0(const Matrix& m) { return Vec3(m._11, m._12, m._13); }
	inline static Vec3 Get3Row1(const Matrix& m) { return Vec3(m._21, m._22, m._23); }
	inline static Vec3 Get3Row2(const Matrix& m) { return Vec3(m._31, m._32, m._33); }

	// Cols (0~2, 3행까지만)
	inline static Vec3 Get3Col0(const Matrix& m) { return Vec3(m._11, m._21, m._31); }
	inline static Vec3 Get3Col1(const Matrix& m) { return Vec3(m._12, m._22, m._32); }
	inline static Vec3 Get3Col2(const Matrix& m) { return Vec3(m._13, m._23, m._33); }




};

class Mat3x3
{
public:
	FORCEINLINE Mat3x3(const Matrix& matrix)
	{
		std::memcpy(_mat[0], &matrix._11, sizeof(float) * 3);
		std::memcpy(_mat[1], &matrix._21, sizeof(float) * 3);
		std::memcpy(_mat[2], &matrix._31, sizeof(float) * 3);
	}

	FORCEINLINE float* operator[](int row)
	{
		return _mat[row];
	}
	FORCEINLINE const float* operator[](int row) const 
	{
		return _mat[row]; 
	}


private:
	float _mat[3][3];
};
