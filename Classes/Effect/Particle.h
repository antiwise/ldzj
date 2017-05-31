#pragma once
#include "cocos2d.h"

USING_NS_CC;
class Particle : public Sprite 
{
CC_CONSTRUCTOR_ACCESS:
    Particle(void)
		: Sprite(),
		life(0),
		currentLife(0),
		scale(0.0f),
		scaleDiff(0.0f),
		rotation(0.0f),
		rotationDiff(0.0f),
		velocity(0.0f),
		velocityDiff(0.0f),
		angle(0.0f),
		angleDiff(0.0f),
		angleCos(0.0f),
		angleSin(0.0f),
		transparency(0.0f),
		transparencyDiff(0.0f),
		wind(0.0f),
		windDiff(0.0f),
		gravity(0.0f),
		gravityDiff(0.0f),
		tint(Color3B::WHITE)
	{
	}

	virtual ~Particle(void) {}

public:
	static Particle* create();

    static Particle* create(const std::string& filename);

	static Particle* createWithSpriteFrameName(const std::string& spriteFrameName);

public:
	int life;
	int currentLife;
	float scale;
	float scaleDiff;
	float rotation;
	float rotationDiff;
	float velocity;
	float velocityDiff;
	float angle;
	float angleDiff;
	float angleCos;
	float angleSin;
	float transparency;
	float transparencyDiff;
	float wind;
	float windDiff;
	float gravity;
	float gravityDiff;
	Color3B tint;
};
