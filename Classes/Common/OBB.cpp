#include"OBB.h"
#include<assert.h>

Projection::Projection(float _min, float _max)
	:min(_min),
	max(_max)
{

}

Projection::~Projection()
{

}

float Projection::getMax() const
{
	return max ;
}

float Projection::getMin() const
{
	return min ;
}

bool Projection::overlap(Projection* proj)
{
	if(min > proj->getMax()) return false ;
	if(max < proj->getMin()) return false ;

	return true ;
}

OBB::OBB()
{

}

OBB::~OBB()
{

}

VECTOR2 OBB::getVertex(int index) const
{
	assert(0 <= index && index <=3 && "The index must be from 0 to 3");
	return vertex[index] ;
}

void OBB::setVertex(int index , float x, float y)
{
	assert(0 <= index && index <=3 && "The index must be from 0 to 3");
	vertex[index].x = x ;
	vertex[index].y = y ;
}

void OBB::setVertex(int index, VECTOR2 v)
{
	assert(0 <= index && index <=3 && "The index must be from 0 to 3");
	vertex[index].x = v.x ;
	vertex[index].y = v.y ;
}

void OBB::getAxies(VECTOR2* axie)
{
	for(int i = 0 ; i < 4 ; i ++)
	{
		VECTOR2 s ;
		Vec2Sub(s,vertex[i],vertex[(i+1)%4]);
		Vec2Normalize(s, s);
		axie[i].x = -s.y ;
		axie[i].y = s.x ;
	}
}

Projection OBB::getProjection(VECTOR2 axie)
{
	float min = 0 ;
	Vec2Dot(min,vertex[0], axie);
	float max = min ;

	for(int i = 1 ; i < 4 ; i ++)
	{
		float temp = 0 ;
		Vec2Dot(temp, vertex[i], axie);
		if(temp > max)
			max = temp ;
		else if(temp < min)
			min = temp ;
	}// end for

	return Projection(min, max);
}

bool OBB::isCollidWithOBB(OBB* obb)
{
	//Get the seperat axie
	getAxies(axie1);
	obb->getAxies(axie2);

	//Check for overlap for all of the axies
	for(int i = 0 ; i < 4 ; i ++)
	{
		Projection p1 = getProjection(axie1[i]);
		Projection p2 = obb->getProjection(axie1[i]);

		if(!p1.overlap(&p2)){
			return false ;
		}
	}

	for(int i = 0 ; i < 4 ; i ++)
	{
		Projection p1 = getProjection(axie2[i]);
		Projection p2 = obb->getProjection(axie2[i]);

		if(!p1.overlap(&p2)){
			return false ;
		}
	}

	return true ;
}