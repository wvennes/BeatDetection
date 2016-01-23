//---------------------------------------------------------------------------
// Matrix4.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_MATRIX4_
#define _INCLUDED_MATRIX4_

#define STATIC

#include "Vec3.hpp"
#include "GameMath.hpp"


/////////////////////////////////////////////////////////////////////////////
class Matrix4
{
public:
	Matrix4();
	Matrix4( const Vec3f& i, const Vec3f& j, const Vec3f& k );
	Matrix4( const Matrix4& matrixToCopy );
	~Matrix4() {}

	const Matrix4& operator=( const Matrix4& rightHandSide );

	static const Matrix4 GetTranslationMatrix( const Vec3f& translation );
	static const Matrix4 GetRotationMatrix( const float rotationDegrees, const Vec3f& rotationAxis );
	static const Matrix4 GetScaleMatrix( const Vec3f& scale );
	static const Matrix4 GetScaleMatrix( const float scale );
	void SetToIdentity();
	void SetFrustum( const float left, const float right, const float bottom, const float top, const float zNear, const float zFar );
	void SetPerspective( const float fovy, const float aspect, const float zNear, const float zFar );
	void SetOrtho( const float left, const float right, const float bottom, const float top, const float zNear, const float zFar );
	float* GetMatrixData();
	const Matrix4 GetTransformedMatrix( const Matrix4& newTransform ) const;
	void ApplyTransformationMatrix( const Matrix4& newTransform );
	void ApplyThisMatrixToVector( Vec3f& value ) const;

private:
	float	m_matrix[ 16 ];
};


//---------------------------------------------------------------------------
inline float* Matrix4::GetMatrixData()
{
	return &m_matrix[ 0 ];
}


//---------------------------------------------------------------------------
STATIC inline const Matrix4 Matrix4::GetTranslationMatrix( const Vec3f& translation )
{
	Matrix4 result;

	result.m_matrix[ 12 ] = translation.x;
	result.m_matrix[ 13 ] = translation.y;
	result.m_matrix[ 14 ] = translation.z;

	return result;
}


//---------------------------------------------------------------------------
STATIC inline const Matrix4 Matrix4::GetScaleMatrix( const Vec3f& scale )
{
	Matrix4 result;

	result.m_matrix[ 0 ] *= scale.x;
	result.m_matrix[ 5 ] *= scale.y;
	result.m_matrix[ 10 ] *= scale.z;

	return result;
}


//---------------------------------------------------------------------------
STATIC inline const Matrix4 Matrix4::GetScaleMatrix( const float scale )
{
	Matrix4 result;

	result.m_matrix[ 0 ] *= scale;
	result.m_matrix[ 5 ] *= scale;
	result.m_matrix[ 10 ] *= scale;

	return result;
}


#endif