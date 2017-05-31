//-----------------------------------------------------------------------------
// declaration: Copyright(c), by XJ , 2014 . All right reserved .
// brief	: The math structure and operator
// author	: XJ
// version	: 1.0
// date		: 2014 / 2 / 16
//------------------------------------------------------------------------------


//���������ļ򵥲�������


#pragma once

//
#define EPSILON 0.0001f

//@Vector
//********************************************************************************
typedef struct VECTOR2
{
	union
	{
		float m[2] ;
		struct
		{float x, y ;};
	};
}*LPVECTOR2;

typedef struct VECTOR3
{
	union
	{
		float m[3] ;

		struct
		{float x, y, z ;} ;
	};
	
}*LPVECTOR3 ;

typedef struct VECTOR4
{
	union
	{
		float m[4] ;

		struct
		{float x, y, z, w ;};
	};
	
}*LPVECTOR4 ;

//Build
VECTOR4 MAKE_VECTOR4(float x,float y,float z,float w)  ;
VECTOR3 MAKE_VECTOR3(float x, float y, float z);
VECTOR2 MAKE_VECTOR2(float x, float y);
	
//Method
VECTOR4 Vec4Add(VECTOR4& pOut, VECTOR4& pV1, VECTOR4& pV2);
VECTOR4 Vec4Sub(VECTOR4& pOut, VECTOR4& pV1, VECTOR4& pV2);
VECTOR4 Vec4Mul(VECTOR4& pOut, VECTOR4& pV1,float n);
VECTOR4 Vec4Div(VECTOR4& pOut, VECTOR4& pV1,float n);
VECTOR4 Vec4Cross(VECTOR4& pOut, VECTOR4& pV1, VECTOR4& pV2);
float Vec4Dot(float& pOut, VECTOR4& pV1, VECTOR4& pV2);
float Vec4Length(float& pOut, VECTOR4& p);
VECTOR4 Vec4Normalize(VECTOR4& pOut, VECTOR4& p);

VECTOR3 Vec3Add(VECTOR3& pOut, VECTOR3& pV1, VECTOR3& pV2);
VECTOR3 Vec3Sub(VECTOR3& pOut, VECTOR3& pV1, VECTOR3& pV2);
VECTOR3 Vec3Mul(VECTOR3& pOut, VECTOR3& pV1,float n);
VECTOR3 Vec3Div(VECTOR3& pOut, VECTOR3& pV1,float n);
VECTOR3 Vec3Cross(VECTOR3& pOut, VECTOR3& pV1, VECTOR3& pV2);
float Vec3Dot(float& pOut, VECTOR3& pV1, VECTOR3& pV2);
float Vec3Length(float& pOut, VECTOR3& p);
VECTOR3 Vec3Normalize(VECTOR3& pOut, VECTOR3& p);

VECTOR2 Vec2Add(VECTOR2& pOut, VECTOR2& pV1, VECTOR2& pV2);
VECTOR2 Vec2Sub(VECTOR2& pOut, VECTOR2& pV1, VECTOR2& pV2);
VECTOR2 Vec2Mul(VECTOR2& pOut, VECTOR2& pV1,float n);
VECTOR2 Vec2Div(VECTOR2& pOut, VECTOR2& pV1,float n);
float Vec2Dot(float& pOut, VECTOR2& pV1, VECTOR2& pV2);
float Vec2Length(float& pOut, VECTOR2& p);
VECTOR2 Vec2Normalize(VECTOR2& pOut, VECTOR2& p);

//********************************************************************************
//@Vector end