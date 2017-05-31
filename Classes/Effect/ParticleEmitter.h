#pragma once
#include "ParticleType.h"
#include "Particle.h"
#include "cocos2d.h"

USING_NS_CC;

enum
{
	UPDATE_SCALE = 1 << 0,
	UPDATE_ANGLE = 1 << 1,
	UPDATE_ROTATION = 1 << 2,
	UPDATE_VELOCITY = 1 << 3,
	UPDATE_WIND = 1 << 4,
	UPDATE_GRAVITY = 1 << 5,
	UPDATE_TINT = 1 << 6
};

class ParticleEmitter
{
public:
	ParticleEmitter (Node *pParent, std::istringstream &ifs, std::string& texturePath, int ZOrder);
	ParticleEmitter (const ParticleEmitter& emitter);

	~ParticleEmitter();
	Particle*			GetRenderElement(std::string& key);
	void				ReleaseRenderElement(Particle* pParticle);

public:
	
	void initialize ();

	void setMaxParticleCount (int value) ;

	void addParticle ();

	void addParticles (int count);

	void update (float delta);

	void start ();

	void reset ();

	void restart ();

	void activateParticle (int index);

	bool updateParticle (Particle* particle, float delta, int deltaMillis);

	void setPosition (float valueX, float valueY);

	/** Ignores the {@link #setContinuous(bool) continuous} setting until the emitter is started again. This allows the emitter
	 * to stop smoothly. */
	void AllowCompletion () {
		allowCompletion = true;
		durationTimer = duration;
	}

	std::string getName () {
		return name;
	}

	void setName (std::string value) {
		name = value;
	}

	ScaledNumericValue getLife () {
		return lifeValue;
	}

	ScaledNumericValue getScale () {
		return scaleValue;
	}

	ScaledNumericValue getRotation () {
		return rotationValue;
	}

	GradientColorValue getTint () {
		return tintValue;
	}

	ScaledNumericValue getVelocity () {
		return velocityValue;
	}

	ScaledNumericValue getWind () {
		return windValue;
	}

	ScaledNumericValue getGravity () {
		return gravityValue;
	}

	ScaledNumericValue getAngle () {
		return angleValue;
	}

	ScaledNumericValue getEmission () {
		return emissionValue;
	}

	ScaledNumericValue getTransparency () {
		return transparencyValue;
	}

	RangedNumericValue getDuration () {
		return durationValue;
	}

	RangedNumericValue getDelay () {
		return delayValue;
	}

	ScaledNumericValue getLifeOffset () {
		return lifeOffsetValue;
	}

	RangedNumericValue getXOffsetValue () {
		return xOffsetValue;
	}

	RangedNumericValue getYOffsetValue () {
		return yOffsetValue;
	}

	ScaledNumericValue getSpawnWidth () {
		return spawnWidthValue;
	}

	ScaledNumericValue getSpawnHeight () {
		return spawnHeightValue;
	}

	SpawnShapeValue getSpawnShape () {
		return spawnShapeValue;
	}

	bool isAttached () {
		return attached;
	}

	void setAttached (bool value) {
		attached = value;
	}

	bool isContinuous () {
		return continuous;
	}

	void setContinuous (bool value) {
		continuous = value;
	}

	bool isAligned () {
		return aligned;
	}

	void setAligned (bool value) {
		aligned = value;
	}

	bool isAdditive () {
		return additive;
	}

	void setAdditive (bool value) {
		additive = value;
	}

	bool isBehind () {
		return behind;
	}

	void setBehind (bool value) {
		behind = value;
	}

	int getMinParticleCount () {
		return minParticleCount;
	}

	void setMinParticleCount (int value) {
		minParticleCount = value;
	}

	int getMaxParticleCount () {
		return maxParticleCount;
	}

	bool isComplete () {
		if (delayTimer < delay) return false;
		return durationTimer >= duration && activeCount == 0;
	}

	float getPercentComplete () {
		if (delayTimer < delay) return 0;

		float value = durationTimer / (float)duration;
		return value > 1?value:1;
	}

	float getX () {
		return x;
	}

	float getY () {
		return y;
	}

	int getActiveCount () {
		return activeCount;
	}

	std::string getImagePath () {
		return imagePath;
	}

	void setImagePath (std::string& value) {
		imagePath = value;
		resPath = m_TexturePath + "_" + imagePath;
	}

	// ��ת
	void setFlip (bool valueX, bool valueY);

	// ��X�ᷴת
	void FlipY ();

	// ��ȡ������Χ��AABB
	Rect getBoundingBox ();

	// ������Ч�ű�
	void load (std::istringstream &ifs);

	void	setDurationValue(float value) { duration = value; } 
	float	getDurationValue() { return duration; }

	void	setDurationTimer(float value) { durationTimer = value; } 
	float	getDurationTimer() { return durationTimer; }

	// ������Ⱦ����
	void	releaseRenderInfo();
	void	release();

	// ���ø��ڵ�
	void	SetParent(Node *pNode);

	// ����Zֵ
	void		SetZOrder(int value) { m_ZOrder = value; }

	void		setVisible(bool value) 
	{
		if (m_pLayer)
		{
			m_pLayer->setVisible(value);
		}
	}
	
	void		SetScale(float value) 
	{
		if (m_pLayer)
		{
			m_pLayer->setScale(value);
		}
	}

protected:
	std::list<Particle*> m_SpritePool;	// ����Ԫ�ػ����

	RangedNumericValue delayValue		;	//Delay: ������ϵͳ��ʼ�󣬷������ȴ�����ʱ�俪ʼ����
	ScaledNumericValue lifeOffsetValue	;	//Life Offset: ������������ʾ֮ǰ�Ѿ��õ�����������ֵ ����������ʵ����һ��������������50%ʱ������
	RangedNumericValue durationValue	;	//Duration�������������ȣ�Ҳ��������Ч�������ĵ�ʱ�䣬ע�����ʱ�����������ʱ�䲻ͬ
	ScaledNumericValue lifeValue		;	//Life: һ�����ӵ�����ʱ�䣬
	ScaledNumericValue emissionValue 	;	//ÿ���ӷ�����ٸ����ӣ������������һ��ͼ��ͼ���м�д��duration������ͼ��X ���ʾ����������ʱ�䣬��˼��˵������������ʱ���ڣ���ͼ����Ƹ���ʱ�������ÿ���ӷ�����ٸ�����ߵ����������ı������ڿ������ɳ�ʼֵʱ�ķ�Χ����ߵ�'>'�ַ�������������һ����ֵ������˵�����Ǹ���>��, ����������������ֵ�м�ѡ��һ�����ֵ��Ϊ����ֵ��Relative�����ѡ�У���ʾͼ���е�ֵ�����ֵ������ڳ�ʼֵ�������Ǹ�����ֵ.
	ScaledNumericValue scaleValue 		;	//����
	ScaledNumericValue rotationValue	;	//��ת
	ScaledNumericValue velocityValue	;	//�ٶ�
	ScaledNumericValue angleValue		;	//����Ƕ�
	ScaledNumericValue windValue		;	//�� xoffset
	ScaledNumericValue gravityValue 	;	//���� yoffset
	ScaledNumericValue transparencyValue;	//���ӵ�͸����
	GradientColorValue tintValue		;	//���ӵ���ɫ�������������������ڱ仯��������ɫ
	ScaledNumericValue xOffsetValue 	;	//���ӳ�����λ���������λ�õ�����ƫ��
	ScaledNumericValue yOffsetValue		;	//���ӳ�����λ���������λ�õ�����ƫ��
	ScaledNumericValue spawnWidthValue	;	//����������״�Ŀ���
	ScaledNumericValue spawnHeightValue	;	//����������״�Ŀ���
	SpawnShapeValue spawnShapeValue		;	//���ڲ������ӵķ���������״

	Node*			m_pParent;
	Layer			*m_pLayer;
	int				m_ZOrder;
	std::string		m_TexturePath;

	float accumulator;
	Particle*	particles[100];
	bool		m_bActive[100];
	int minParticleCount;
	int maxParticleCount;
	float x;
	float y;
	float ox;
	float oy;
	std::string name;
	std::string imagePath;
	std::string resPath;
	int activeCount;
	bool firstUpdate;
	bool flipX;
	bool flipY;
	int updateFlags;
	bool allowCompletion;
	Rect bounds;

	int emission;
	int emissionDiff;
	int emissionDelta;

	int lifeOffset;
	int lifeOffsetDiff;
	int life;
	int lifeDiff;
	float spawnWidth;
	float spawnWidthDiff;
	float spawnHeight;
	float spawnHeightDiff;
	float duration;
	float durationTimer;
	float delay;
	float delayTimer;

	bool attached;
	bool continuous;
	bool aligned;
	bool behind;
	bool additive;
	bool oldVersion;
};
