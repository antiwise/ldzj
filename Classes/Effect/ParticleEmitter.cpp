#include "ParticleEmitter.h"
#include "Common/FileUtils.h"

ParticleEmitter::ParticleEmitter (Node *pParent, std::istringstream &ifs, std::string& texturePath, int ZOrder) 
	: m_pParent(pParent),
	m_ZOrder(ZOrder),
	m_TexturePath(texturePath),
	accumulator(0.0f),
	minParticleCount(0),
	maxParticleCount(0),
	x(0.0f),
	y(0.0f),
	ox(0.0f),
	oy(0.0f),
	activeCount(0),
	firstUpdate(true),
	flipX(false),
	flipY(false),
	updateFlags(0),
	allowCompletion(false),
	emission(0),
	emissionDiff(0),
	emissionDelta(0),
	lifeOffset(0),
	lifeOffsetDiff(0),
	life(0),
	lifeDiff(0),
	spawnWidth(0.0f),
	spawnWidthDiff(0.0f),
	spawnHeight(0.0f),
	spawnHeightDiff(0.0f),
	duration(0.0f),
	durationTimer(0.0f),
	delay(0.0f),
	delayTimer(0.0f),
	attached(false),
	continuous(false),
	aligned(false),
	behind(false),
	additive(false),
	oldVersion(false)
{
	initialize();
	load(ifs);
	resPath = m_TexturePath + "_" + imagePath;

	for (int i = 0; i < maxParticleCount; i++) {
		m_bActive[i] = false;
		particles[i] = nullptr;
	}
}

ParticleEmitter::ParticleEmitter (const ParticleEmitter& emitter)
{
	name = emitter.name;
	imagePath = emitter.imagePath;
	resPath = emitter.resPath;
	setMaxParticleCount(emitter.maxParticleCount);
	minParticleCount = emitter.minParticleCount;
	delayValue.load(emitter.delayValue);
	durationValue.load(emitter.durationValue);
	emissionValue.load(emitter.emissionValue);
	lifeValue.load(emitter.lifeValue);
	lifeOffsetValue.load(emitter.lifeOffsetValue);
	scaleValue.load(emitter.scaleValue);
	rotationValue.load(emitter.rotationValue);
	velocityValue.load(emitter.velocityValue);
	angleValue.load(emitter.angleValue);
	windValue.load(emitter.windValue);
	gravityValue.load(emitter.gravityValue);
	transparencyValue.load(emitter.transparencyValue);
	tintValue.load(emitter.tintValue);
	xOffsetValue.load(emitter.xOffsetValue);
	yOffsetValue.load(emitter.yOffsetValue);
	spawnWidthValue.load(emitter.spawnWidthValue);
	spawnHeightValue.load(emitter.spawnHeightValue);
	spawnShapeValue.load(emitter.spawnShapeValue);
	attached = emitter.attached;
	continuous = emitter.continuous;
	aligned = emitter.aligned;
	behind = emitter.behind;
	additive = emitter.additive;
	
	m_pLayer = Layer::create();
	m_pLayer->retain();
	m_pParent        = 	emitter.m_pParent        ;
	m_ZOrder         = 	emitter.m_ZOrder         ;
	m_TexturePath    = 	emitter.m_TexturePath    ;
	accumulator      = 	emitter.accumulator      ;
	minParticleCount = 	emitter.minParticleCount ;
	maxParticleCount = 	emitter.maxParticleCount ;
	x                = 	emitter.x                ;
	y                = 	emitter.y                ;
	ox               = 	emitter.ox                ;
	oy               = 	emitter.oy                ;
	activeCount      = 	emitter.activeCount      ;
	firstUpdate      = 	emitter.firstUpdate      ;
	flipX            = 	emitter.flipX            ;
	flipY            = 	emitter.flipY            ;
	updateFlags      = 	emitter.updateFlags      ;
	allowCompletion  = 	emitter.allowCompletion  ;
	emission         = 	emitter.emission         ;
	emissionDiff     = 	emitter.emissionDiff     ;
	emissionDelta    = 	emitter.emissionDelta    ;
	lifeOffset       = 	emitter.lifeOffset       ;
	lifeOffsetDiff   = 	emitter.lifeOffsetDiff   ;
	life             = 	emitter.life             ;
	lifeDiff         = 	emitter.lifeDiff         ;
	spawnWidth       = 	emitter.spawnWidth       ;
	spawnWidthDiff   = 	emitter.spawnWidthDiff   ;
	spawnHeight      =	emitter.spawnHeight      ;
	spawnHeightDiff  =	emitter.spawnHeightDiff  ;
	duration         =	emitter.duration         ;
	durationTimer    =	emitter.durationTimer    ;
	delay            =	emitter.delay            ;
	delayTimer       =	emitter.delayTimer       ;
	attached         =	emitter.attached         ;
	continuous       =	emitter.continuous       ;
	aligned          =	emitter.aligned          ;
	behind           =	emitter.behind           ;
	additive         =	emitter.additive         ;
	oldVersion       =	emitter.oldVersion       ;
	
	for (int i = 0; i < maxParticleCount; i++) {
		m_bActive[i] = false;
		particles[i] = nullptr;
	}
}

ParticleEmitter::~ParticleEmitter()
{
	for(std::list<Particle*>::reverse_iterator it = m_SpritePool.rbegin(); it != m_SpritePool.rend(); it++)
	{
		if ((*it)->getParent())
		{
			(*it)->removeFromParentAndCleanup(true);
		}
		else
		{
			(*it)->release();
		}
	}
	m_SpritePool.clear();

	for (int i = 0; i < maxParticleCount; i++)
	{
		if (m_bActive[i])
		{
			if (particles[i]->getParent())
			{
				particles[i]->removeFromParentAndCleanup(true);
			}
			else
			{
				particles[i]->release();
			}
			particles[i] = nullptr;
		}
	}

	if (m_pLayer)
	{
		m_pLayer->release();
		if (m_pLayer->getParent())
		{
			m_pLayer->removeFromParentAndCleanup(true);
		}
		else
		{
			m_pLayer->release();
		}
	}

	m_pParent = nullptr;
}

void ParticleEmitter::initialize () 
{
	durationValue.setAlwaysActive(true);
	emissionValue.setAlwaysActive(true);
	lifeValue.setAlwaysActive(true);
	scaleValue.setAlwaysActive(true);
	transparencyValue.setAlwaysActive(true);
	spawnShapeValue.setAlwaysActive(true);
	spawnWidthValue.setAlwaysActive(true);
	spawnHeightValue.setAlwaysActive(true);
}

void ParticleEmitter::setMaxParticleCount (int value) 
{
	maxParticleCount = value;
	if (maxParticleCount > 100)
	{
		maxParticleCount = 100;
	}
	activeCount = 0;
}

void ParticleEmitter::addParticle () 
{
	if (activeCount == maxParticleCount) return;
	for (int i = 0; i < maxParticleCount; i++) {
		if (!m_bActive[i]) {
			activateParticle(i);
			m_bActive[i] = true;
			activeCount = activeCount + 1;
			break;
		}
	}
}

void ParticleEmitter::addParticles (int count) 
{
	count = std::min(count, maxParticleCount - activeCount);
	if (count == 0) return;
		
	int num = count;
	for (int index = 0; index < maxParticleCount; index++) {
		if (!m_bActive[index]) {
			activateParticle(index);
			m_bActive[index] = true;
			num--;

			if (num == 0)
			{
				break;
			}
		}
	}

	activeCount += count;
}

void ParticleEmitter::update (float delta) 
{
	if (m_pParent == nullptr)
	{
		return;
	} 
	
	accumulator += delta * 1000;
	if (accumulator < 1) return;
	int deltaMillis = (int)accumulator;
	accumulator -= deltaMillis;

	if (delayTimer < delay) {
		delayTimer += deltaMillis;
	} else {
		bool done = false;
		if (firstUpdate) {
			firstUpdate = false;
			addParticle();
		}

		if (durationTimer < duration)
			durationTimer += deltaMillis;
		else {
			if (!continuous || allowCompletion)
				done = true;
			else
				restart();
		}

		if (!done) {
			emissionDelta += deltaMillis;
			float emissionTime = emission + emissionDiff * emissionValue.getScale(durationTimer / (float)duration);
			if (emissionTime > 0) {
				emissionTime = 1000 / emissionTime;
				if (emissionDelta >= emissionTime) {
					int emitCount = (int)(emissionDelta / emissionTime);
					emitCount = std::min(emitCount, maxParticleCount - activeCount);
					emissionDelta -= emitCount * emissionTime;
					while (emissionDelta > emissionTime)
					{
						emissionDelta -= emissionTime;
					}
					addParticles(emitCount);
				}
			}
			if (activeCount < minParticleCount) addParticles(minParticleCount - activeCount);
		}
	}
	
	for (int i = 0; i < maxParticleCount; i++) {
		if (m_bActive[i] && !updateParticle(particles[i], delta, deltaMillis)) {
			m_bActive[i] = false;
			particles[i] = nullptr;
			activeCount--;
		}
	}
} 

void ParticleEmitter::start () 
{
	firstUpdate = true;
	allowCompletion = false;
	restart();
}

void ParticleEmitter::reset () {
	emissionDelta = 0;
	durationTimer = duration;
	
	for (int i = 0; i < maxParticleCount; i++) {
		m_bActive[i] = false;
		particles[i] = nullptr;
	}

	activeCount = 0;
	start();
}

void ParticleEmitter::restart () 
{
	x = ox;
	y = oy;

	delay = delayValue.m_bActive ? delayValue.newLowValue() : 0;
	delayTimer = 0;

	durationTimer -= duration;
	duration = durationValue.newLowValue();

	emission = (int)emissionValue.newLowValue();
	emissionDiff = (int)emissionValue.newHighValue();
	if (!emissionValue.isRelative()) emissionDiff -= emission;

	life = (int)lifeValue.newLowValue();
	lifeDiff = (int)lifeValue.newHighValue();
	if (!lifeValue.isRelative()) lifeDiff -= life;

	lifeOffset = lifeOffsetValue.m_bActive ? (int)lifeOffsetValue.newLowValue() : 0;
	lifeOffsetDiff = (int)lifeOffsetValue.newHighValue();
	if (!lifeOffsetValue.isRelative()) lifeOffsetDiff -= lifeOffset;

	spawnWidth = spawnWidthValue.newLowValue();
	spawnWidthDiff = spawnWidthValue.newHighValue();
	if (!spawnWidthValue.isRelative()) spawnWidthDiff -= spawnWidth;

	spawnHeight = spawnHeightValue.newLowValue();
	spawnHeightDiff = spawnHeightValue.newHighValue();
	if (!spawnHeightValue.isRelative()) spawnHeightDiff -= spawnHeight;

	updateFlags = 0;
	if (angleValue.m_bActive && angleValue.m_nResNum > 1) updateFlags |= UPDATE_ANGLE;
	if (velocityValue.m_bActive) updateFlags |= UPDATE_VELOCITY;
	if (scaleValue.m_nResNum > 1) updateFlags |= UPDATE_SCALE;
	if (rotationValue.m_bActive && rotationValue.m_nResNum > 1) updateFlags |= UPDATE_ROTATION;
	if (windValue.m_bActive) updateFlags |= UPDATE_WIND;
	if (gravityValue.m_bActive) updateFlags |= UPDATE_GRAVITY;
	if (tintValue.m_nResNum > 1) updateFlags |= UPDATE_TINT;
}

void ParticleEmitter::activateParticle (int index) 
{
	if (m_pParent == nullptr)
	{
		return;
	}

	int targetX = x;
	int targetY = y;
	Particle* particle = particles[index];
	if (particle == nullptr) {
		particles[index] = particle = GetRenderElement(resPath);
		particle->setFlipX(flipX);
		particle->setFlipY(flipY);
		if (particle->getParent() == nullptr)
		{
			m_pLayer->addChild(particle, m_ZOrder);
		}
	}

	particle->setVisible(true);

	float percent = durationTimer / (float)duration;
	particle->currentLife = particle->life = life + (int)(lifeDiff * lifeValue.getScale(percent));

	if (velocityValue.m_bActive) {
		particle->velocity = velocityValue.newLowValue();
		particle->velocityDiff = velocityValue.newHighValue();
		if (!velocityValue.isRelative()) particle->velocityDiff -= particle->velocity;
	}

	particle->angle = angleValue.newLowValue();
	particle->angleDiff = angleValue.newHighValue();
	if (!angleValue.isRelative()) particle->angleDiff -= particle->angle;
	float angle = 0;
	if ((updateFlags & UPDATE_ANGLE) == 0) {
		angle = particle->angle + particle->angleDiff * angleValue.getScale(0);
		particle->angle = angle;
		particle->angleCos = std::cos(CC_DEGREES_TO_RADIANS(angle));
		particle->angleSin = std::sin(CC_DEGREES_TO_RADIANS(angle));
	}

	particle->scale = scaleValue.newLowValue() / particle->getContentSize().width;
	particle->scaleDiff = scaleValue.newHighValue() / particle->getContentSize().width;
	if (!scaleValue.isRelative()) particle->scaleDiff -= particle->scale;
	particle->setScale(particle->scale + particle->scaleDiff * scaleValue.getScale(0));

	if (rotationValue.m_bActive) {
		particle->rotation = rotationValue.newLowValue();
		particle->rotationDiff = rotationValue.newHighValue();
		if (!rotationValue.isRelative()) particle->rotationDiff -= particle->rotation;
		float rotation = particle->rotation + particle->rotationDiff * rotationValue.getScale(0);
		if (aligned) rotation += angle;
		particle->setRotation(360 - rotation);
	}

	if (windValue.m_bActive) {
		particle->wind = windValue.newLowValue();
		particle->windDiff = windValue.newHighValue();
		if (!windValue.isRelative()) particle->windDiff -= particle->wind;
	}

	if (gravityValue.m_bActive) {
		particle->gravity = gravityValue.newLowValue();
		particle->gravityDiff = gravityValue.newHighValue();
		if (!gravityValue.isRelative()) particle->gravityDiff -= particle->gravity;
	}

	particle->tint = tintValue.getColor(0);
	particle->setColor(particle->tint);
	particle->transparency = transparencyValue.newLowValue();
	particle->transparencyDiff = transparencyValue.newHighValue() - particle->transparency;

	// Spawn.
	if (xOffsetValue.m_bActive)
	{
		targetX += xOffsetValue.newLowValue();
	}
	if (yOffsetValue.m_bActive)
	{
		targetY += yOffsetValue.newLowValue();
	}

	if(spawnShapeValue.m_nShape == "square") 
	{
		float width = spawnWidth + (spawnWidthDiff * spawnWidthValue.getScale(percent));
		float height = spawnHeight + (spawnHeightDiff * spawnHeightValue.getScale(percent));
		targetX += CCRANDOM_0_1() * (width) - width / 2;
		targetY += CCRANDOM_0_1() * (height) - height / 2;
	}
	else if(spawnShapeValue.m_nShape == "ellipse") 
	{
		float width = spawnWidth + (spawnWidthDiff * spawnWidthValue.getScale(percent));
		float height = spawnHeight + (spawnHeightDiff * spawnHeightValue.getScale(percent));
		float radiusX = width / 2;
		float radiusY = height / 2;
		if (radiusX == 0 || radiusY == 0)
		{
		}
		else
		{
			float scaleY = radiusX / (float)radiusY;
			if (spawnShapeValue.m_bEdges)
			{
				float spawnAngle;
				if (spawnShapeValue.m_nSide == "top") 
				{
					spawnAngle = -CCRANDOM_0_1() * 179.f;
				}
				else if (spawnShapeValue.m_nSide == "bottom") 
				{
					spawnAngle = CCRANDOM_0_1() * 179.f;
				}
				else
				{
					spawnAngle = CCRANDOM_0_1() * 360.f;
				}

				float cosDeg = std::cos(CC_DEGREES_TO_RADIANS(spawnAngle));
				float sinDeg = std::sin(CC_DEGREES_TO_RADIANS(spawnAngle));
				targetX += cosDeg * radiusX;
				targetY += sinDeg * radiusX / scaleY;
				if ((updateFlags & UPDATE_ANGLE) == 0) {
					particle->angle = spawnAngle;
					particle->angleCos = cosDeg;
					particle->angleSin = sinDeg;
				}
			} else {
				float radius2 = radiusX * radiusX;
				while (true) {
					float px = CCRANDOM_0_1() *(width) - radiusX;
					float py = CCRANDOM_0_1() *(width) - radiusX;
					if (px * px + py * py <= radius2) {
						targetX += px;
						targetY += py / scaleY;
						break;
					}
				}
			}
		}
	}
	else if(spawnShapeValue.m_nShape == "line") 
	{
		float width = spawnWidth + (spawnWidthDiff * spawnWidthValue.getScale(percent));
		float height = spawnHeight + (spawnHeightDiff * spawnHeightValue.getScale(percent));
		if (width != 0) 
		{
			float lineX = width * CCRANDOM_0_1();
			targetX += lineX;
			targetY += lineX * (height / (float)width);
		} 
		else
		{
			targetY += height * CCRANDOM_0_1();
		}
	}
	
	particle->setPosition(ccp(targetX, targetY));
	int offsetTime = (int)(lifeOffset + lifeOffsetDiff * lifeOffsetValue.getScale(percent));
	if (offsetTime > 0) {
		if (offsetTime >= particle->currentLife) offsetTime = particle->currentLife - 1;
		updateParticle(particle, offsetTime / 1000.f, offsetTime);
	}
}

bool ParticleEmitter::updateParticle (Particle* particle, float delta, int deltaMillis) 
{
	int life = particle->currentLife - deltaMillis;
	if (life <= 0) 
	{
		ReleaseRenderElement(particle);
		return false;
	}

	particle->currentLife = life;

	float percent = 1 - particle->currentLife / (float)particle->life;

	if ((updateFlags & UPDATE_SCALE) != 0)
		particle->setScale(particle->scale + particle->scaleDiff * scaleValue.getScale(percent));

	if ((updateFlags & UPDATE_VELOCITY) != 0) {
		float velocity = (particle->velocity + particle->velocityDiff * velocityValue.getScale(percent)) * delta;

		float velocityX, velocityY;
		if ((updateFlags & UPDATE_ANGLE) != 0) {
			float angle = particle->angle + particle->angleDiff * angleValue.getScale(percent);
			velocityX = velocity * std::cos(CC_DEGREES_TO_RADIANS(angle));
			velocityY = velocity * std::sin(CC_DEGREES_TO_RADIANS(angle));
			if ((updateFlags & UPDATE_ROTATION) != 0) {
				float rotation = particle->rotation + particle->rotationDiff * rotationValue.getScale(percent);
				if (aligned) rotation += angle;
				particle->setRotation(360 - rotation);
			}
		} else {
			velocityX = velocity * particle->angleCos;
			velocityY = velocity * particle->angleSin;
			if (aligned || (updateFlags & UPDATE_ROTATION) != 0) {
				float rotation = particle->rotation + particle->rotationDiff * rotationValue.getScale(percent);
				if (aligned) rotation += particle->angle;
				particle->setRotation(360 - rotation);
			}
		}

		if ((updateFlags & UPDATE_WIND) != 0)
			velocityX += (particle->wind + particle->windDiff * windValue.getScale(percent)) * delta;

		if ((updateFlags & UPDATE_GRAVITY) != 0)
			velocityY += (particle->gravity + particle->gravityDiff * gravityValue.getScale(percent)) * delta;

		particle->setPosition(ccpAdd(particle->getPosition(), ccp(velocityX, velocityY)));
	} else {
		if ((updateFlags & UPDATE_ROTATION) != 0)
			particle->setRotation(360 - particle->rotation + particle->rotationDiff * rotationValue.getScale(percent));
	}

	Color3B color;
	if ((updateFlags & UPDATE_TINT) != 0)
	{
		particle->setColor(tintValue.getColor(percent));
	}

	int Opacity = (particle->transparency + particle->transparencyDiff * transparencyValue.getScale(percent))*255;
	particle->setOpacity(Opacity);

	return true;
}

void ParticleEmitter::setPosition (float valueX, float valueY) 
{
	if (attached) {
		float xAmount = valueX - x;
		float yAmount = valueY - y;
		for (int i = 0; i < maxParticleCount; i++)
		{
			if (m_bActive[i])
			{
				particles[i]->setPosition(ccpAdd(particles[i]->getPosition(), ccp(xAmount, yAmount)));
			}
		}
	}
	x = valueX;
	y = valueY;

	ox = x;
	oy = y;
}

void ParticleEmitter::setFlip (bool valueX, bool valueY)
{
	flipX = valueX;
	flipY = valueY;
	for (int i = 0; i < maxParticleCount; i++) {
		Particle* particle = particles[i];
		if (particle != nullptr) {
			particle->setFlipX(flipX);
			particle->setFlipY(flipY);
		}
	}
}

void ParticleEmitter::FlipY () 
{
	angleValue.setHigh(-angleValue.getHighMin(), -angleValue.getHighMax());
	angleValue.setLow(-angleValue.getLowMin(), -angleValue.getLowMax());

	gravityValue.setHigh(-gravityValue.getHighMin(), -gravityValue.getHighMax());
	gravityValue.setLow(-gravityValue.getLowMin(), -gravityValue.getLowMax());

	windValue.setHigh(-windValue.getHighMin(), -windValue.getHighMax());
	windValue.setLow(-windValue.getLowMin(), -windValue.getLowMax());

	rotationValue.setHigh(-rotationValue.getHighMin(), -rotationValue.getHighMax());
	rotationValue.setLow(-rotationValue.getLowMin(), -rotationValue.getLowMax());

	yOffsetValue.setLow(-yOffsetValue.getLowMin(), -yOffsetValue.getLowMax());
}

Rect ParticleEmitter::getBoundingBox () 
{
	Rect rect;
	for (int i = 0; i < maxParticleCount; i++)
	{
		if (m_bActive[i]) {
			Rect r = particles[i]->getBoundingBox();
			float x, y, w, h;
			x = r.getMinX() < rect.getMinX()?r.getMinX():rect.getMinX();
			y = r.getMinY() < rect.getMinY()?r.getMinY():rect.getMinY();
			w = r.getMaxX() < rect.getMaxX()?r.getMaxX():rect.getMaxX();
			h = r.getMaxY() < rect.getMaxY()?r.getMaxY():rect.getMaxY();

			rect = Rect(x, y, w, h);
		}
	}

	return rect;
}

void ParticleEmitter::load (std::istringstream &ifs)
{
	int maxchars = 8192;				// Alloc enough size.
	std::vector<char> buf(maxchars);	// Alloc enough size.
	ifs.getline(&buf[0], maxchars);
	std::string key(&buf[0]);
	if (key.find("version") != std::string::npos)
	{
		oldVersion = true;
		ifs.getline(&buf[0], maxchars);
	}

	std::string value = std::string(&buf[0]);
	name = readString(value); //, "name"
	ifs.getline(&buf[0], maxchars);
	delayValue.load(ifs);
	ifs.getline(&buf[0], maxchars);
	durationValue.load(ifs);
	ifs.getline(&buf[0], maxchars);
	ifs.getline(&buf[0], maxchars);

	value = std::string(&buf[0]);
	setMinParticleCount(readInt(value)); //, "minParticleCount"
	ifs.getline(&buf[0], maxchars);

	value = std::string(&buf[0]);
	setMaxParticleCount(readInt(value)); //, "maxParticleCount"
	ifs.getline(&buf[0], maxchars);
	emissionValue.load(ifs);
	ifs.getline(&buf[0], maxchars);
	lifeValue.load(ifs);
	ifs.getline(&buf[0], maxchars);
	lifeOffsetValue.load(ifs);
	ifs.getline(&buf[0], maxchars);
	xOffsetValue.load(ifs);
	ifs.getline(&buf[0], maxchars);
	yOffsetValue.load(ifs);
	ifs.getline(&buf[0], maxchars);
	spawnShapeValue.load(ifs);
	ifs.getline(&buf[0], maxchars);
	spawnWidthValue.load(ifs);
	ifs.getline(&buf[0], maxchars);
	spawnHeightValue.load(ifs);
	ifs.getline(&buf[0], maxchars);
	scaleValue.load(ifs);
	if (oldVersion)
	{
		ifs.getline(&buf[0], maxchars);
		scaleValue.load(ifs);
	}
	ifs.getline(&buf[0], maxchars);
	velocityValue.load(ifs);
	ifs.getline(&buf[0], maxchars);
	angleValue.load(ifs);
	ifs.getline(&buf[0], maxchars);
	rotationValue.load(ifs);
	ifs.getline(&buf[0], maxchars);
	windValue.load(ifs);
	ifs.getline(&buf[0], maxchars);
	gravityValue.load(ifs);
	ifs.getline(&buf[0], maxchars);
	tintValue.load(ifs);
	ifs.getline(&buf[0], maxchars);
	transparencyValue.load(ifs);
	ifs.getline(&buf[0], maxchars);
	ifs.getline(&buf[0], maxchars);
	
	value = std::string(&buf[0]);
	attached = readBool(value);	//, "attached"
	ifs.getline(&buf[0], maxchars);
	
	value = std::string(&buf[0]);
	continuous = readBool(value); //, "continuous"
	ifs.getline(&buf[0], maxchars);
	
	value = std::string(&buf[0]);
	aligned = readBool(value); //, "aligned"
	ifs.getline(&buf[0], maxchars);
	
	value = std::string(&buf[0]);
	additive = readBool(value); //, "additive"
	if (oldVersion)
	{
		ifs.getline(&buf[0], maxchars);
		value = std::string(&buf[0]);
		behind = readBool(value); //, "behind"

		ifs.getline(&buf[0], maxchars);
		value = std::string(&buf[0]);
		behind = readBool(value); //, "behind"
	}
	else
	{
		ifs.getline(&buf[0], maxchars);
		value = std::string(&buf[0]);
		behind = readBool(value); //, "behind"
	}
}

void  ParticleEmitter::releaseRenderInfo()
{
	// 清理
	for(std::list<Particle*>::reverse_iterator it = m_SpritePool.rbegin(); it != m_SpritePool.rend(); it++)
	{
		if ((*it)->getParent())
		{
			(*it)->removeFromParentAndCleanup(true);
		}
		else
		{
			(*it)->release();
		}
	}
	m_SpritePool.clear();

	for (int i = 0; i < maxParticleCount; i++)
	{
		if (m_bActive[i])
		{
			if (particles[i]->getParent())
			{
				particles[i]->removeFromParentAndCleanup(true);
			}
			else
			{
				particles[i]->release();
			}

			particles[i] = nullptr;
			m_bActive[i] = false;
		}
	}

	activeCount = 0;
	// 其他部分reset 负责清理
}

void  ParticleEmitter::release()
{
	// 清理
	for (int i = 0; i < maxParticleCount; i++)
	{
		if (m_bActive[i])
		{
			particles[i]->setVisible(false);
			m_SpritePool.push_back(particles[i]);
			particles[i] = nullptr;
			m_bActive[i] = false;
		}
	}

	activeCount = 0;
	// 其他部分reset 负责清理
}

void ParticleEmitter::SetParent(Node *pNode) 
{ 
	if (m_pLayer->getParent() == pNode)
	{
		return;
	}
	else if (m_pLayer->getParent() && m_pLayer->getParent() != pNode)
	{
		m_pLayer->removeFromParent();
	}

	m_pParent = pNode;
	m_pParent->addChild(m_pLayer, m_ZOrder);
}

Particle* ParticleEmitter::GetRenderElement(std::string& key)
{
	Particle* pParticle = nullptr;
	if (m_SpritePool.size() > 0)
	{
		pParticle = m_SpritePool.front();
		m_SpritePool.pop_front();
	}
	else
	{
		pParticle = Particle::createWithSpriteFrameName(key);
		if (isAdditive())
		{
			//BlendFunc blf = {GL_SRC_ALPHA,GL_ONE};
			BlendFunc blf = {GL_ONE,GL_ONE};
			pParticle->setBlendFunc(blf);
		}
	}

	return pParticle;
}

void ParticleEmitter::ReleaseRenderElement(Particle* pParticle)
{
	pParticle->setVisible(false);
	m_SpritePool.push_back(pParticle);
}