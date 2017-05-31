//------------------------------------------------------------------------------
// declaration	: Copyright (c), by XJ , 2014 . All right reserved .
// brief		: This file will define the OBB(Oriented bounding box)
// author		: XJ
// date			: 2014 / 6 / 16
// version		: 1.0
//------------------------------------------------------------------------------
#pragma once

//¼òµ¥µÄÅö×²¼ì²â

#include"XJMath.h"

class Projection
{
public:
	Projection(float min, float max);
	~Projection();

public:
	bool overlap(Projection* proj);

public:
	float getMin() const;
	float getMax() const ;

private:
	float min ;
	float max ;
};

class OBB
{
public:
	OBB();
	~OBB();

public:
	void getAxies(VECTOR2 * axie);
	Projection getProjection(VECTOR2 axie);

public:
	VECTOR2 getVertex(int index) const;
	void setVertex(int index, float x, float y);
	void setVertex(int index, VECTOR2 v);

public:
	bool isCollidWithOBB(OBB* obb);

private:
	VECTOR2 vertex[4] ;
	VECTOR2 axie1[4] ;
	VECTOR2 axie2[4] ;
};