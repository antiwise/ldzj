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

	// 反转
	void setFlip (bool valueX, bool valueY);

	// 按X轴反转
	void FlipY ();

	// 获取轴对齐包围盒AABB
	Rect getBoundingBox ();

	// 加载特效脚本
	void load (std::istringstream &ifs);

	void	setDurationValue(float value) { duration = value; } 
	float	getDurationValue() { return duration; }

	void	setDurationTimer(float value) { durationTimer = value; } 
	float	getDurationTimer() { return durationTimer; }

	// 清理渲染对象
	void	releaseRenderInfo();
	void	release();

	// 设置父节点
	void	SetParent(Node *pNode);

	// 设置Z值
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
	std::list<Particle*> m_SpritePool;	// 动画元素缓存池

	RangedNumericValue delayValue		;	//Delay: 当粒子系统开始后，发射器等待多少时间开始发射
	ScaledNumericValue lifeOffsetValue	;	//Life Offset: 决定粒子在显示之前已经用掉它生命多少值 ，这样可以实现让一个粒子在它生命50%时再显现
	RangedNumericValue durationValue	;	//Duration：发射器生存跨度，也就是粒子效果持续的的时间，注意这个时间和粒子生存时间不同
	ScaledNumericValue lifeValue		;	//Life: 一个粒子的生存时间，
	ScaledNumericValue emissionValue 	;	//每秒钟发射多少个粒子，这个参数带了一张图表，图表中间写着duration，代表图表X 轴表示发射器生存时间，意思是说发射器的生存时间内，该图表控制各个时间点粒子每秒钟发射多少个，左边的上下两个文本框用于控制生成初始值时的范围，左边的'>'字符用于输入另外一个数值，比如说下面那个”>“, 发射器会在这两个值中间选择一下随机值作为上限值，Relative，如果选中，表示图表中的值是相对值，相对于初始值。否则是个绝对值.
	ScaledNumericValue scaleValue 		;	//缩放
	ScaledNumericValue rotationValue	;	//旋转
	ScaledNumericValue velocityValue	;	//速度
	ScaledNumericValue angleValue		;	//发射角度
	ScaledNumericValue windValue		;	//风 xoffset
	ScaledNumericValue gravityValue 	;	//重力 yoffset
	ScaledNumericValue transparencyValue;	//粒子的透明度
	GradientColorValue tintValue		;	//粒子的颜色，可以在粒子生存期内变化任意多个颜色
	ScaledNumericValue xOffsetValue 	;	//粒子出现在位置相对中心位置的像素偏移
	ScaledNumericValue yOffsetValue		;	//粒子出现在位置相对中心位置的像素偏移
	ScaledNumericValue spawnWidthValue	;	//发射器的形状的宽，高
	ScaledNumericValue spawnHeightValue	;	//发射器的形状的宽，高
	SpawnShapeValue spawnShapeValue		;	//用于产生粒子的发射器的形状

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
