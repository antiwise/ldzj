#include "Particle.h"

Particle* Particle::create()
{
	Particle *particle = new (std::nothrow) Particle();
    if (particle && particle->init())
    {
        particle->autorelease();
        return particle;
    }
    CC_SAFE_DELETE(particle);
    return nullptr;
}

Particle* Particle::create(const std::string& filename)
{
    Particle *particle = new (std::nothrow) Particle();
    if (particle && particle->initWithFile(filename))
    {
        particle->autorelease();
        return particle;
    }
    CC_SAFE_DELETE(particle);
    return nullptr;
}

Particle* Particle::createWithSpriteFrameName(const std::string& spriteFrameName)
{
	Particle *particle = new (std::nothrow) Particle();
	if (particle && particle->initWithSpriteFrameName(spriteFrameName))
    {
        particle->autorelease();
        return particle;
    }
    CC_SAFE_DELETE(particle);
    return nullptr;
}