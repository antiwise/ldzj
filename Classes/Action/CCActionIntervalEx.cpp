#include "CCActionIntervalEx.h"
#include "ObjectEx/LXObject.h"
#include "ObjectEx/LXObjectManager.h"
#include "ObjectEx/Plane/LXPlayerPlane.h"
#include "ObjectEx/Weapon/LXWeapon.h"
#include "Shooter/LXShooterLogic.h"
#include "Shooter/LXShooter.h"
#include "Common/LXUtils.h"
#include "GameDefine.h"

//
// arcMoveBy
//

arcMoveBy* arcMoveBy::create(float duration, float x, float y, float xparam, float yparam)
{
	arcMoveBy *ret = new arcMoveBy();
	ret->initWithDuration(duration, x, y, xparam, yparam);
	ret->autorelease();

	return ret;
}

bool arcMoveBy::initWithDuration(float duration, float x, float y, float xparam, float yparam)
{
	if (ActionInterval::initWithDuration(duration))
	{
		_x = x;
		_y = y;
		_xparam = xparam;
		_yparam = yparam;
		return true;
	}

	return false;
}

arcMoveBy* arcMoveBy::clone() const
{
	// no copy constructor
	auto a = new arcMoveBy();
	a->initWithDuration(_duration, _x, _y, _xparam, _yparam);
	a->autorelease();
	return a;
}

void arcMoveBy::startWithTarget(Node *target)
{
	auto glview = Director::getInstance()->getOpenGLView();
	const auto screenSize = glview->getDesignResolutionSize();

	ActionInterval::startWithTarget(target);
	_lastTime = 0.0f;

	_orignx = target->getPositionX() + 100;
	_origny = screenSize.height - target->getPositionY();

	float var1 = _x;								// OffsetX		400
	float var2 = _y;								// OffsetY		200
	float var3 = _xparam;							// 300
	float var4 = var1 - _xparam;					// 400-300 = 100
	float var5 = 2.0F * _orignx + _xparam;			// 2.0*200 + 300
	float var6 = var1 + 2.0F * _orignx + _xparam;	// 400 + 2.0*200 + 300
	float var7 = _yparam;							// 80
	float var8 = var2 - _yparam;					// 200 - 80
	float var9 = 2.0F * _origny + _yparam;			// 

	_param1 = (var7 * (var8 * (var2 + 2.0F * _origny + _yparam) + var6 * var4) - var8 * (var9 * var7 + var5 * var3)) / (2.0F * (var4 * var7 - var3 * var8));
	_param2 = -var3 * _param1 / var7 + (var9 * var7 + var3 * var5) / (2.0F * var7);
	_param3 = (float)std::sqrt((double)((_param1 - _orignx) * (_param1 - _orignx) + (_param2 - _origny) * (_param2 - _origny)));

	float var10 = _x;
	float var11 = _y;
	float var12 = var10 + _orignx;
	float var13 = var11 + _origny;
	float var14 = _orignx + _xparam;
	float var15 = _origny + _yparam;
	float var16 = (float)(180.0 * std::acos((double)((_orignx - _param1) / _param3)) / 3.141592653589793);
	if(_origny < _param2) {
		var16 = 360.0F - var16;
	}

	float var17 = (float)(180.0 * std::acos((double)((var12 - _param1) / _param3)) / 3.141592653589793);
	if(var13 < _param2) {
		var17 = 360.0F - var17;
	}

	float var18 = (float)(180.0 * std::acos((double)((var14 - _param1) / _param3)) / 3.141592653589793);
	if(var15 < _param2) {
		var18 = 360.0F - var18;
	}

	if(var17 >= var16) 
	{
		if(var18 >= var16 && var18 <= var17) 
		{
			_param4 = var17 - var16;
		}
		else
		{
			_param4 = -(var16 + (360.0F - var17));
		}
	} 
	else if(var18 >= var17 && var18 <= var16) 
	{
		_param4 = var17 - var16;
	} 
	else 
	{
		_param4 = var17 + (360.0F - var16);
	}

	_param5 = var16;
}

arcMoveBy* arcMoveBy::reverse() const
{
	return arcMoveBy::create(_duration, -_x, -_y, -_xparam, -_yparam);
}


void arcMoveBy::update(float t)
{
	if (_target)
	{
		float fDelta = t - _lastTime < 0?0:t - _lastTime;
		_lastTime = t;

		float var2 = fDelta * _param4;
		float var3 = _param3 * (std::cos(CC_DEGREES_TO_RADIANS(var2 + _param5)) - std::cos(CC_DEGREES_TO_RADIANS(_param5)));
		float var4 = -_param3 * (std::sin(CC_DEGREES_TO_RADIANS(var2 + _param5)) - std::sin(CC_DEGREES_TO_RADIANS(_param5)));
		_param5 = 360.0 + var2 + _param5;
		while(_param5 > 360.0f)
		{
			_param5 -= 360.0f;
		}

		Point targetPoint = ccpAdd(_target->getPosition(), ccp(var3, var4));
		_target->setPosition(targetPoint);
	}
}

//
// arcMoveTo
//

arcMoveTo* arcMoveTo::create(float duration, float x, float y, float xparam, float yparam)
{
	arcMoveTo *ret = new arcMoveTo();
	ret->initWithDuration(duration, x, y, xparam, yparam);
	ret->autorelease();

	return ret;
}

bool arcMoveTo::initWithDuration(float duration, float x, float y, float xparam, float yparam)
{
	if (ActionInterval::initWithDuration(duration))
	{
		_x = x;
		_y = y;
		_xparam = xparam;
		_yparam = yparam;
		return true;
	}

	return false;
}

arcMoveTo* arcMoveTo::clone() const
{
	// no copy constructor
	auto a = new arcMoveTo();
	a->initWithDuration(_duration, _x, _y, _xparam, _yparam);
	a->autorelease();
	return a;
}

void arcMoveTo::startWithTarget(Node *target)
{
	auto glview = Director::getInstance()->getOpenGLView();
	const auto screenSize = glview->getDesignResolutionSize();

	ActionInterval::startWithTarget(target);
	_lastTime = 0.0f;

	_targetOrignx = target->getPositionX() + 100;
	_targetOrigny = screenSize.height - target->getPositionY();

	float var1 = _xparam - _targetOrignx;
	float var2 = _x - _xparam;
	float var3 = _xparam + _targetOrignx;
	float var4 = _x + _xparam;
	float var5 = _yparam - _targetOrigny;
	float var6 = _y - _yparam;
	float var7 = _yparam + _targetOrigny;
	_param1 = (var5 * (var6 * (_y + _yparam) + var4 * var2) - var6 * (var7 * var5 + var3 * var1)) / (2.0F * (var2 * var5 - var6 * var1));
	_param2 = -var1 * _param1 / var5 + (var7 * var5 + var1 * var3) / (2.0F * var5);
	_param3 = (float)std::sqrt((double)((_param1 - _targetOrignx) * (_param1 - _targetOrignx) + (_param2 - _targetOrigny) * (_param2 - _targetOrigny)));
	_param4 = (float)(180.0 * std::acos((double)((_targetOrignx - _param1) / _param3)) / 3.141592653589793);
	if(_targetOrigny < _param2) {
		_param4 = 360.0F - _param4;
	}

	_param5 = (float)(180.0 * std::acos((double)((_x - _param1) / _param3)) / 3.141592653589793);
	if(_y < _param2) {
		_param5 = 360.0F - _param5;
	}

	float var8 = (float)(180.0 * std::acos((double)((_xparam - _param1) / _param3)) / 3.141592653589793);
	if(_yparam < _param2) {
		var8 = 360.0F - var8;
	}

	if(_param5 >= _param4) 
	{
		if(var8 >= _param4 && var8 <= _param5) 
		{
			_param6 = _param5 - _param4;
		}
		else 
		{
			_param6 = -(360.0F - _param5 + _param4);
		}
	} 
	else if(var8 >= _param5 && var8 <= _param4) 
	{
		_param6 = _param5 - _param4;
	} 
	else
	{
		_param6 = 360.0F - _param4 + _param5;
	}
}


arcMoveTo* arcMoveTo::reverse() const
{
	return arcMoveTo::create(_duration, -_x, -_y, -_xparam, -_yparam);
}

void arcMoveTo::update(float t)
{
	if (_target)
	{
		auto glview = Director::getInstance()->getOpenGLView();
		const auto screenSize = glview->getDesignResolutionSize();

		float var2 = _param1 + _param3 * std::cos(CC_DEGREES_TO_RADIANS(_param4 + t * _param6));
		float var3 = _param2 - _param3 * std::sin(CC_DEGREES_TO_RADIANS(_param4 + t * _param6));

		var2 = var2 - 100;
		var3 = screenSize.height -  var3;

		_target->setPosition(ccp(var2, var3));
	}
}

//
// CheckValue
//

CheckValue* CheckValue::create(int value)
{
	CheckValue *ret = new CheckValue();
	ret->initWithDuration(value);
	ret->autorelease();

	return ret;
}

bool CheckValue::initWithDuration(int value)
{
	// 这里随便给个很大的数
	if (ActionInterval::initWithDuration(9999))
	{
		_value = value;
		return true;
	}

	return false;
}

CheckValue* CheckValue::clone() const
{
	// no copy constructor
	auto a = new CheckValue();
	a->initWithDuration(_value);
	a->autorelease();
	return a;
}

void CheckValue::startWithTarget(Node *target)
{
	ActionInterval::startWithTarget(target);
}

CheckValue* CheckValue::reverse() const
{
	return CheckValue::create(_value);
}

void CheckValue::update(float t)
{
	if (_target && t < 1.0)
	{
		LXObject *pObject = dynamic_cast<LXObject*>(_target);
		if(pObject)
		{
			if (pObject->CheckHP(_value)) // 到达指定数值
			{
				// 清理AI动作
				_target->stopActionByTag(ai_tag_bossai);
				ActionInterval *pAction = dynamic_cast<ActionInterval*>(_target->getActionByTag(ai_tag_common));
				if (pAction)
				{
					pAction->changeElapsed((1 - t) * _duration);
				}
			}
		}
	}
}


//
// ChangeAnimation
//

ChangeAnimation* ChangeAnimation::create(const std::string &animation, int type)
{
	ChangeAnimation *ret = new ChangeAnimation();
	ret->initWithDuration(animation, type);
	ret->autorelease();

	return ret;
}

bool ChangeAnimation::initWithDuration(const std::string &animation, int type)
{
	// 这里随便给个很大的数
	if (ActionInterval::initWithDuration(0))
	{
		_animation = animation;
		_type = type;
		return true;
	}

	return false;
}

ChangeAnimation* ChangeAnimation::clone() const
{
	// no copy constructor
	auto a = new ChangeAnimation();
	a->initWithDuration(_animation, _type);
	a->autorelease();
	return a;
}

void ChangeAnimation::startWithTarget(Node *target)
{
	ActionInterval::startWithTarget(target);
}

ChangeAnimation* ChangeAnimation::reverse() const
{
	return ChangeAnimation::create(_animation, _type);
}

void ChangeAnimation::update(float t)
{
	if (_target)
	{
		LXObject *pObject = dynamic_cast<LXObject*>(_target);
		if(pObject)
		{
			std::string strPath = pObject->GetPath();
			pObject->PlayAnimaton(strPath, _animation, pObject->GetNameFix(), _type);
		}
	}
}


//
// NextAnimation
//

NextAnimation* NextAnimation::create(const std::string &animation)
{
	NextAnimation *ret = new NextAnimation();
	ret->initWithDuration(animation);
	ret->autorelease();

	return ret;
}

bool NextAnimation::initWithDuration(const std::string &animation)
{
	// 这里随便给个很大的数
	if (ActionInterval::initWithDuration(0))
	{
		_animation = animation;
		return true;
	}

	return false;
}

NextAnimation* NextAnimation::clone() const
{
	// no copy constructor
	auto a = new NextAnimation();
	a->initWithDuration(_animation);
	a->autorelease();
	return a;
}

void NextAnimation::startWithTarget(Node *target)
{
	ActionInterval::startWithTarget(target);
}

NextAnimation* NextAnimation::reverse() const
{
	return NextAnimation::create(_animation);
}

void NextAnimation::update(float t)
{
	if (_target)
	{
		LXObject *pObject = dynamic_cast<LXObject*>(_target);
		if(pObject)
		{
			pObject->SetNextAnimation(_animation);
		}
	}
}

//
// BossAI
//

BossAI* BossAI::create(const std::string &action)
{
	BossAI *ret = new BossAI();
	ret->initWithDuration(action);
	ret->autorelease();

	return ret;
}

bool BossAI::initWithDuration(const std::string &action)
{
	// 这里随便给个很大的数
	if (ActionInterval::initWithDuration(0))
	{
		_action = action;
		return true;
	}

	return false;
}

BossAI* BossAI::clone() const
{
	// no copy constructor
	auto a = new BossAI();
	a->initWithDuration(_action);
	a->autorelease();
	return a;
}

void BossAI::startWithTarget(Node *target)
{
	ActionInterval::startWithTarget(target);
}

BossAI* BossAI::reverse() const
{
	return BossAI::create(_action);
}

void BossAI::update(float t)
{
	if (_target)
	{
		LXObject *pObject = dynamic_cast<LXObject*>(_target);
		if(pObject)
		{
			pObject->InitAction(_action, ai_tag_bossai);
		}
	}
}

//
// ChangeHit
//

ChangeHit* ChangeHit::create(int type, int a, int b, int c, int d)
{
	ChangeHit *ret = new ChangeHit();
	ret->initWithDuration(type, a, b, c, d);
	ret->autorelease();

	return ret;
}

bool ChangeHit::initWithDuration(int type, int a, int b, int c, int d)
{
	// 这里随便给个很大的数
	if (ActionInterval::initWithDuration(0))
	{
		_type = type;
		_a = a;
		_b = b;
		_c = c;
		_d = d;

		return true;
	}

	return false;
}

ChangeHit* ChangeHit::clone() const
{
	// no copy constructor
	auto a = new ChangeHit();
	a->initWithDuration(_type, _a, _b, _c, _d);
	a->autorelease();
	return a;
}

void ChangeHit::startWithTarget(Node *target)
{
	ActionInterval::startWithTarget(target);
}

ChangeHit* ChangeHit::reverse() const
{
	return ChangeHit::create(_type, _a, _b, _c, _d);
}

void ChangeHit::update(float t)
{
	if (_target)
	{
		LXObject *pObject = dynamic_cast<LXObject*>(_target);
		if(pObject)
		{
			// 重置包围盒信息
			pObject->SetCollideInfo(Rect(_a, _b, _c, _d));
		}
	}
}

//
//  AddEffect
//

AddEffect*  AddEffect::create(const std::string &effect, int value1, int value2, int value3)
{
	AddEffect *ret = new  AddEffect();
	ret->initWithDuration(effect, value1, value2, value3);
	ret->autorelease();

	return ret;
}

bool  AddEffect::initWithDuration(const std::string &effect, int value1, int value2, int value3)
{
	// 这里随便给个很大的数
	if (ActionInterval::initWithDuration(0))
	{
		_effect = effect;
		_value1 = value1;
		_value2 = value1;
		_value3 = value1;
		return true;
	}

	return false;
}

AddEffect*  AddEffect::clone() const
{
	// no copy constructor
	auto a = new  AddEffect();
	a->initWithDuration(_effect, _value1, _value2, _value3);
	a->autorelease();
	return a;
}

void  AddEffect::startWithTarget(Node *target)
{
	ActionInterval::startWithTarget(target);
}

AddEffect*  AddEffect::reverse() const
{
	return  AddEffect::create(_effect, _value1, _value2, _value3);
}

void  AddEffect::update(float t)
{
	if (_target)
	{
		LXObject *pObject = dynamic_cast<LXObject*>(_target);
		if(pObject)
		{
			//播放特效
		}
	}
}

//
//  AddFollowEffect
//

AddFollowEffect*  AddFollowEffect::create(const std::string &effect, int value1, int value2)
{
	AddFollowEffect *ret = new  AddFollowEffect();
	ret->initWithDuration(effect, value1, value2);
	ret->autorelease();

	return ret;
}

bool  AddFollowEffect::initWithDuration(const std::string &effect, int value1, int value2)
{
	// 这里随便给个很大的数
	if (ActionInterval::initWithDuration(0))
	{
		_effect = effect;
		_value1 = value1;
		_value2 = value1;
		return true;
	}

	return false;
}

AddFollowEffect*  AddFollowEffect::clone() const
{
	// no copy constructor
	auto a = new  AddFollowEffect();
	a->initWithDuration(_effect, _value1, _value2);
	a->autorelease();
	return a;
}

void  AddFollowEffect::startWithTarget(Node *target)
{
	ActionInterval::startWithTarget(target);
}

AddFollowEffect*  AddFollowEffect::reverse() const
{
	return  AddFollowEffect::create(_effect, _value1, _value2);
}

void  AddFollowEffect::update(float t)
{
	if (_target)
	{
		LXObject *pObject = dynamic_cast<LXObject*>(_target);
		if(pObject)
		{
			//播放特效
		}
	}
}

//
//  FireEffect
//

FireEffect*  FireEffect::create(const std::string &effect, std::vector<int> &vec)
{
	FireEffect *ret = new  FireEffect();
	ret->initWithDuration(effect, vec);
	ret->autorelease();

	return ret;
}

bool  FireEffect::initWithDuration(const std::string &effect, std::vector<int> &vec)
{
	// 这里随便给个很大的数
	if (ActionInterval::initWithDuration(0))
	{
		_effect = effect;
		_vec.swap(vec);
		return true;
	}

	return false;
}

FireEffect*  FireEffect::clone() const
{
	// no copy constructor
	auto a = new  FireEffect();
	a->initWithDuration(_effect, const_cast<std::vector<int>&>(_vec));
	a->autorelease();
	return a;
}

void  FireEffect::startWithTarget(Node *target)
{
	ActionInterval::startWithTarget(target);
}

FireEffect*  FireEffect::reverse() const
{
	return  FireEffect::create(_effect, const_cast<std::vector<int>&>(_vec));
}

void  FireEffect::update(float t)
{
	if (_target)
	{
		LXObject *pObject = dynamic_cast<LXObject*>(_target);
		if(pObject)
		{
			// 播放特效
		}
	}
}


//
//  UserBullet
//

UserBullet*  UserBullet::create(bool value)
{
	UserBullet *ret = new  UserBullet();
	ret->initWithDuration(value);
	ret->autorelease();

	return ret;
}

bool  UserBullet::initWithDuration(bool value)
{
	// 这里随便给个很大的数
	if (ActionInterval::initWithDuration(0))
	{
		_value = value;
		return true;
	}

	return false;
}

UserBullet*  UserBullet::clone() const
{
	// no copy constructor
	auto a = new  UserBullet();
	a->initWithDuration(_value);
	a->autorelease();
	return a;
}

void  UserBullet::startWithTarget(Node *target)
{
	ActionInterval::startWithTarget(target);
}

UserBullet*  UserBullet::reverse() const
{
	return  UserBullet::create(_value);
}

void  UserBullet::update(float t)
{
	if (_target)
	{
		LXPlayerPlane* pObject = dynamic_cast<LXPlayerPlane*>(ObjectManager().GetPlayer());
		if(pObject)
		{
			pObject->SetFireState(_value);
			if (_value)
			{
				pObject->InitShootLogic();
			}
		}
	}
}

//
//  Finish
//

Finish*  Finish::create(bool value)
{
	Finish *ret = new  Finish();
	ret->initWithDuration(value);
	ret->autorelease();

	return ret;
}

bool  Finish::initWithDuration(bool value)
{
	// 这里随便给个很大的数
	if (ActionInterval::initWithDuration(0))
	{
		_clean = value;
		return true;
	}

	return false;
}

Finish*  Finish::clone() const
{
	// no copy constructor
	auto a = new  Finish();
	a->initWithDuration(_clean);
	a->autorelease();
	return a;
}

void  Finish::startWithTarget(Node *target)
{
	ActionInterval::startWithTarget(target);
}

Finish*  Finish::reverse() const
{
	return  Finish::create(_clean);
}

void  Finish::update(float t)
{
	if (_target && _clean)
	{
		LXObject *pObject = dynamic_cast<LXObject*>(_target);
		if(pObject)
		{
			pObject->SetCleanUp(true);
		}
	}
}

//
//  Sound
//

Sound*  Sound::create(const std::string &sound)
{
	Sound *ret = new  Sound();
	ret->initWithDuration(sound);
	ret->autorelease();

	return ret;
}

bool  Sound::initWithDuration(const std::string &sound)
{
	// 这里随便给个很大的数
	if (ActionInterval::initWithDuration(0))
	{
		_sound = sound;
		return true;
	}

	return false;
}

Sound*  Sound::clone() const
{
	// no copy constructor
	auto a = new  Sound();
	a->initWithDuration(_sound);
	a->autorelease();
	return a;
}

void  Sound::startWithTarget(Node *target)
{
	ActionInterval::startWithTarget(target);
}

Sound*  Sound::reverse() const
{
	return  Sound::create(_sound);
}

void  Sound::update(float t)
{
	if (_target)
	{
		LXObject *pObject = dynamic_cast<LXObject*>(_target);
		if(pObject)
		{
			// 播放音效
		}
	}
}

//
//  Music
//

Music*  Music::create(const std::string &music)
{
	Music *ret = new  Music();
	ret->initWithDuration(music);
	ret->autorelease();

	return ret;
}

bool  Music::initWithDuration(const std::string &music)
{
	// 这里随便给个很大的数
	if (ActionInterval::initWithDuration(0))
	{
		_music = music;
		return true;
	}

	return false;
}

Music*  Music::clone() const
{
	// no copy constructor
	auto a = new  Music();
	a->initWithDuration(_music);
	a->autorelease();
	return a;
}

void  Music::startWithTarget(Node *target)
{
	ActionInterval::startWithTarget(target);
}

Music*  Music::reverse() const
{
	return  Music::create(_music);
}

void  Music::update(float t)
{
	if (_target)
	{
		LXObject *pObject = dynamic_cast<LXObject*>(_target);
		if(pObject)
		{
			// 播放背景音乐
		}
	}
}

//
//  Teach
//

Teach*  Teach::create(int index)
{
	Teach *ret = new  Teach();
	ret->initWithDuration(index);
	ret->autorelease();

	return ret;
}

bool  Teach::initWithDuration(int index)
{
	// 这里随便给个很大的数
	if (ActionInterval::initWithDuration(0))
	{
		_index = index;
		return true;
	}

	return false;
}

Teach*  Teach::clone() const
{
	// no copy constructor
	auto a = new  Teach();
	a->initWithDuration(_index);
	a->autorelease();
	return a;
}

void  Teach::startWithTarget(Node *target)
{
	ActionInterval::startWithTarget(target);
}

Teach*  Teach::reverse() const
{
	return  Teach::create(_index);
}

void  Teach::update(float t)
{
	if (_target)
	{
		LXObject *pObject = dynamic_cast<LXObject*>(_target);
		if(pObject)
		{
			// Teach 开始

		}
	}
}


//
// LXMoveBy
//

LXMoveBy* LXMoveBy::create(float speed, 
						   float maxSpeed, 
						   float minSpeed, 
						   float degree, 
						   float addSpeed, 
						   float addDegree, 
						   bool topSpeedLimit,
						   bool needReverse)
{
	LXMoveBy *ret = new LXMoveBy();
	ret->initWithDuration(speed,			
		maxSpeed,		
		minSpeed,		
		degree,	
		addSpeed,
		addDegree,		
		topSpeedLimit,	
		needReverse);

	ret->autorelease();

	return ret;
}

bool LXMoveBy::initWithDuration(float speed, 
								float maxSpeed, 
								float minSpeed, 
								float degree, 
								float addSpeed, 
								float addDegree, 
								bool  topSpeedLimit,
								bool  needReverse)
{
	_speed			= speed;
	_maxSpeed		= maxSpeed;
	_minSpeed		= minSpeed;
	_degree			= degree;
	_addSpeed		= addSpeed;
	_addDegree		= addDegree;
	_topSpeedLimit	= topSpeedLimit;
	_needReverse	= needReverse;

	return true;
}

LXMoveBy* LXMoveBy::clone() const
{
	// no copy constructor
	auto a = new LXMoveBy();
	a->initWithDuration(_speed,			
		_maxSpeed,		
		_minSpeed,		
		_degree,	
		_addSpeed,
		_addDegree,		
		_topSpeedLimit,	
		_needReverse);
	a->autorelease();
	return a;
}

void LXMoveBy::startWithTarget(Node *target)
{
	Action::startWithTarget(target);
}

LXMoveBy* LXMoveBy::reverse() const
{
	return LXMoveBy::create(_speed,			
		_maxSpeed,		
		_minSpeed,		
		_degree,	
		_addSpeed,
		_addDegree,		
		_topSpeedLimit,	
		_needReverse);
}

bool LXMoveBy::isDone() const
{
	return false;
}

void LXMoveBy::step(float dt)
{
	if (_target)
	{
		// 计算偏移
		float fOffsetX = dt * _speed * cos(CC_DEGREES_TO_RADIANS(_degree));
		float fOffsetY = dt * -_speed * sin(CC_DEGREES_TO_RADIANS(_degree));

		// 计算速度加成
		_speed += dt * _addSpeed;

		// 是否限速
		if((_topSpeedLimit && _speed >= _maxSpeed) || _speed <= _minSpeed)
		{
			// 重置速度
			_speed = std::min(std::max(_speed, _maxSpeed), _minSpeed);
			if (_needReverse)
			{
				_addSpeed = -_addSpeed;
			}
		}

		// 角度变化
		_degree = 360 + _degree + dt * _addDegree;
		while(_degree > 360)
		{
			_degree -= 360;
		}
		_target->setPosition(ccpAdd(_target->getPosition(), ccp(fOffsetX, fOffsetY)));
	}
}

//
// LXMoveByRect
//

LXMoveByRect* LXMoveByRect::create(float speed, 
								   float maxSpeed, 
								   float minSpeed, 
								   float degree, 
								   float addSpeed, 
								   float addDegree, 
								   bool topSpeedLimit,
								   bool needReverse,
								   int x,
								   int y,
								   int w,
								   int h)
{
	LXMoveByRect *ret = new LXMoveByRect();
	ret->initWithDuration(speed,			
		maxSpeed,		
		minSpeed,		
		degree,	
		addSpeed,
		addDegree,		
		topSpeedLimit,	
		needReverse,
		x,
		y,
		w,
		h);

	ret->autorelease();

	return ret;
}

bool LXMoveByRect::initWithDuration(float speed, 
									float maxSpeed, 
									float minSpeed, 
									float degree, 
									float addSpeed, 
									float addDegree, 
									bool topSpeedLimit,
									bool needReverse,
									int x,
									int y,
									int w,
									int h)
{
	_speed			= speed;
	_maxSpeed		= maxSpeed;
	_minSpeed		= minSpeed;
	_degree			= degree;
	_addSpeed		= addSpeed;
	_addDegree		= addDegree;
	_topSpeedLimit	= topSpeedLimit;
	_needReverse	= needReverse;
	_x				= x;
	_y				= y;
	_w				= w;
	_h				= h;

	return true;
}

LXMoveByRect* LXMoveByRect::clone() const
{
	// no copy constructor
	auto a = new LXMoveByRect();
	a->initWithDuration(_speed,			
		_maxSpeed,		
		_minSpeed,		
		_degree,	
		_addSpeed,
		_addDegree,		
		_topSpeedLimit,	
		_needReverse,
		_x,
		_y,
		_w,
		_h);
	a->autorelease();
	return a;
}

void LXMoveByRect::startWithTarget(Node *target)
{
	Action::startWithTarget(target);
}

LXMoveByRect* LXMoveByRect::reverse() const
{
	return LXMoveByRect::create(_speed,			
		_maxSpeed,		
		_minSpeed,		
		_degree,	
		_addSpeed,
		_addDegree,		
		_topSpeedLimit,	
		_needReverse,
		_x,
		_y,
		_w,
		_h);
}


void LXMoveByRect::step(float dt)
{
	if (_target)
	{
		LXMoveBy::step(dt);

		// 移动范围判定
		int xPos = _target->getPosition().x;
		int yPos = _target->getPosition().y;

		if((xPos < _x && _degree > 90.0F && _degree < 270.0F) || (xPos > _x + _w && (_degree < 90.0F || _degree > 270.0F))) {
			_degree = 540.0F - _degree;
			while(_degree > 360)
			{
				_degree -= 360;
			}
		}

		if((yPos < _y && _degree > 0.0F && _degree < 180.0F) || (yPos  > _y + _h && _degree > 180.0F && _degree < 360.0F)) {
			_degree = 360.0F - _degree;
		}
	}
}

//
// LXShoot
//

LXShoot* LXShoot::create(LXShooterLogic *pLogic,
						 int group, 
						 float groupWaitTime, 
						 float groupDegreeDelta, 
						 int num, 
						 float waitTime, 
						 float degreeDelta, 
						 float speedDelta,
						 float delay)
{
	LXShoot *ret = new LXShoot();
	ret->initWithDuration(pLogic,			
		group,		
		groupWaitTime,		
		groupDegreeDelta,	
		num,
		waitTime,		
		degreeDelta,	
		speedDelta,
		delay);

	ret->autorelease();

	return ret;
}

bool LXShoot::initWithDuration(LXShooterLogic *pLogic,
							   int group, 
							   float groupWaitTime, 
							   float groupDegreeDelta, 
							   int num, 
							   float waitTime, 
							   float degreeDelta, 
							   float speedDelta,
							   float delay)
{
	_pLogic				= pLogic;
	_group				= group;
	_groupWaitTime		= groupWaitTime;
	_groupDegreeDelta	= groupDegreeDelta;
	_num				= num;
	_waitTime			= waitTime;
	_degreeDelta		= degreeDelta;
	_speedDelta			= speedDelta;
	_delay				= delay;
	_pastTime			= 0.0f;		

	_done				= false;
	_degree				= 0.0f;	
	_speed				= 0.0f;	
	_targetDegree		= 0.0f;	
	_targetSpeed		= 0.0f;	
	_targetNum			= 0.0f;	
	_groupInit			= false;

	return true;
}

LXShoot* LXShoot::clone() const
{
	// no copy constructor
	auto a = new LXShoot();
	a->initWithDuration(_pLogic,			
		_group,		
		_groupWaitTime,		
		_groupDegreeDelta,	
		_num,
		_waitTime,		
		_degreeDelta,	
		_speedDelta,
		_delay);
	a->autorelease();
	return a;
}

void LXShoot::startWithTarget(Node *target)
{
	Action::startWithTarget(target);

	// 获取弹幕信息
	LXShooter *pShooter = _pLogic->GetShooter();
	_degree			= pShooter->GetDegree();
	_targetDegree	= 360 + _degree + calcDegree();
	while(_targetDegree > 360)
	{
		_targetDegree -= 360;
	}
	_speed			= pShooter->GetSpeed();
	_targetSpeed	= _speed;
	_targetNum		= _num;
	_groupInit		= false;
}

LXShoot* LXShoot::reverse() const
{
	return LXShoot::create(_pLogic,			
		_group,		
		_groupWaitTime,		
		_groupDegreeDelta,	
		_num,
		_waitTime,		
		_degreeDelta,	
		_speedDelta,
		_delay);
}

bool LXShoot::isDone() const
{
	if (_done)
	{
		return true;
	}

	return false;
}

float LXShoot::calcDegree()
{
	LXShooter *pShooter = _pLogic->GetShooter();

	LXObject *pObject = dynamic_cast<LXObject*>(_target);
	if (pObject)
	{
		if (pObject->IsPlayer() == false && pShooter->GetDegree() > 640)
		{
			return _pLogic->CalcDegree() - 640.f;
		}
		else
		{
			return 0.0f;
		}
	}

	return 0.0f;
}

void LXShoot::step(float dt)
{
	if (_target)
	{
		float fTime = std::min(dt, 0.5f);

		// action累计执行时间
		_pastTime += fTime;

		// 延迟
		_delay -= fTime;

		// group == 0 && num == 0
		if (_group == 0 && _targetNum == 0)
		{
			_done = true;
		}

		bool bPlayEffect = false;

		// 未结束，没有延迟
		while(_done == false && _delay <= 0)
		{
			// 组没有初始化
			if (_groupInit)
			{
				_targetDegree	= 360 + _degree + calcDegree();
				while(_targetDegree > 360)
				{
					_targetDegree -= 360;
				}

				_groupInit		= false;
			}

			if (dynamic_cast<LXObject*>(_target)->IsPlayer() || _pLogic->GetShooter()->GetDegree() < 640.0f)
			{
				float tempDegree = 360 - _targetDegree;
				while(tempDegree > 360)
				{
					tempDegree -= 360;
				}

				_pLogic->Fire(tempDegree, _targetSpeed);
			}
			else
			{
				_pLogic->Fire(_targetDegree, _targetSpeed);
			}
			// 开火

			// 播放特效
			if (!bPlayEffect)
			{
				_pLogic->PlayEffect();
				bPlayEffect = true;
			}

			// 计算角度加成
			_targetDegree = 360 + _targetDegree + _degreeDelta;
			while(_targetDegree > 360)
			{
				_targetDegree -= 360;
			}

			// 计算速度加成
			_targetSpeed += _speedDelta;

			// 延迟计算
			_delay += _waitTime;

			if (_targetNum > 0)
			{
				_targetNum--;
				if (_targetNum ==0)
				{
					// 一波子弹发射完毕
					if (_group > 0)
					{
						_group--;
					}

					if (_group == 0)
					{
						_done = true;
						break;
					}

					// 计算延迟
					_delay += _groupWaitTime;

					// 计算角度
					_targetDegree = 360 + _targetDegree + _groupDegreeDelta;
					while(_targetDegree > 360)
					{
						_targetDegree -= 360;
					}

					// 速度还原
					_targetSpeed = _speed;

					// 数量还原
					_targetNum = _num;

					// 需要重新计算角度
					_groupInit = true;
				}
			}

		}
	}
}


//
// ShootData
//

ShootData* ShootData::create(const std::string &shootData)
{
	ShootData *ret = new ShootData();
	ret->initWithDuration(shootData);
	ret->autorelease();

	return ret;
}

bool ShootData::initWithDuration(const std::string &shootData)
{
	// 这里随便给个很大的数
	if (ActionInterval::initWithDuration(0))
	{
		_shootData = shootData;
		return true;
	}

	return false;
}

ShootData* ShootData::clone() const
{
	// no copy constructor
	auto a = new ShootData();
	a->initWithDuration(_shootData);
	a->autorelease();
	return a;
}

void ShootData::startWithTarget(Node *target)
{
	ActionInterval::startWithTarget(target);
}

ShootData* ShootData::reverse() const
{
	return ShootData::create(_shootData);
}

void ShootData::update(float t)
{
	if (_target)
	{
		LXObject *pObject = dynamic_cast<LXObject*>(_target);
		if(pObject)
		{
			std::vector<int> weaponInfo;
			std::vector<std::string> shooterVec;
			Utils().split(_shootData, shooterVec, ",");
			for (int i = 1; i < shooterVec.size(); i++)
			{
				weaponInfo.push_back(atoi(shooterVec[i].c_str()));
			}
			pObject->InitShootLogicByIndex(weaponInfo);
		}
	}
}


//
// ShootAll
//

ShootAll* ShootAll::create()
{
	ShootAll *ret = new ShootAll();
	ret->initWithDuration();
	ret->autorelease();

	return ret;
}

bool ShootAll::initWithDuration()
{
	// 这里随便给个很大的数
	if (ActionInterval::initWithDuration(0))
	{
		return true;
	}

	return false;
}

ShootAll* ShootAll::clone() const
{
	// no copy constructor
	auto a = new ShootAll();
	a->initWithDuration();
	a->autorelease();
	return a;
}

void ShootAll::startWithTarget(Node *target)
{
	ActionInterval::startWithTarget(target);
}

ShootAll* ShootAll::reverse() const
{
	return ShootAll::create();
}

void ShootAll::update(float t)
{
	if (_target)
	{
		LXObject *pObject = dynamic_cast<LXObject*>(_target);
		if(pObject)
		{
			pObject->InitShootLogic();
		}
	}
}

//
// StopAll
//

StopAll* StopAll::create()
{
	StopAll *ret = new StopAll();
	ret->initWithDuration();
	ret->autorelease();

	return ret;
}

bool StopAll::initWithDuration()
{
	// 这里随便给个很大的数
	if (ActionInterval::initWithDuration(0))
	{
		return true;
	}

	return false;
}

StopAll* StopAll::clone() const
{
	// no copy constructor
	auto a = new StopAll();
	a->initWithDuration();
	a->autorelease();
	return a;
}

void StopAll::startWithTarget(Node *target)
{
	ActionInterval::startWithTarget(target);
}

StopAll* StopAll::reverse() const
{
	return StopAll::create();
}

void StopAll::update(float t)
{
	if (_target)
	{
		LXObject *pObject = dynamic_cast<LXObject*>(_target);
		if(pObject)
		{
			pObject->StopShootLogic();
		}
	}
}

//
// MoveShooter
//

MoveShooter* MoveShooter::create(int index, float x, float y)
{
	MoveShooter *ret = new MoveShooter();
	ret->initWithDuration(index, x, y);
	ret->autorelease();

	return ret;
}

bool MoveShooter::initWithDuration(int index, float x, float y)
{
	// 这里随便给个很大的数
	if (ActionInterval::initWithDuration(0))
	{
		return true;
	}

	return false;
}

MoveShooter* MoveShooter::clone() const
{
	// no copy constructor
	auto a = new MoveShooter();
	a->initWithDuration(_index, _x, _y);
	a->autorelease();
	return a;
}

void MoveShooter::startWithTarget(Node *target)
{
	ActionInterval::startWithTarget(target);
}

MoveShooter* MoveShooter::reverse() const
{
	return MoveShooter::create(_index, _x, _y);
}

void MoveShooter::update(float t)
{
	if (_target)
	{
		LXObject *pObject = dynamic_cast<LXObject*>(_target);
		if(pObject)
		{
			pObject->MoveShooter(_index, _x, _y);
		}
	}
}

//
// LXMoveTrack
//

LXMoveTrack* LXMoveTrack::create(float speed, 
								 float maxSpeed, 
								 float minSpeed, 
								 float degree, 
								 float addSpeed, 
								 float addDegree, 
								 bool topSpeedLimit,
								 bool needReverse,
								 float delta,
								 float maxDegreeDelta,
								 LXObject* pTarget,
								 int x,
								 int y)
{
	LXMoveTrack *ret = new LXMoveTrack();
	ret->initWithDuration(speed,			
		maxSpeed,		
		minSpeed,		
		degree,	
		addSpeed,
		addDegree,		
		topSpeedLimit,	
		needReverse,
		delta,
		maxDegreeDelta,
		pTarget,
		x,
		y);

	ret->autorelease();

	return ret;
}

bool LXMoveTrack::initWithDuration(float speed, 
								   float maxSpeed, 
								   float minSpeed, 
								   float degree, 
								   float addSpeed, 
								   float addDegree, 
								   bool topSpeedLimit,
								   bool needReverse,
								   float delta,
								   float maxDegreeDelta,
								   LXObject* pTarget,
								   int x,
								   int y)
{
	_speed			= speed;
	_maxSpeed		= maxSpeed;
	_minSpeed		= minSpeed;
	_degree			= degree;
	_addSpeed		= addSpeed;
	_addDegree		= addDegree;
	_topSpeedLimit	= topSpeedLimit;
	_needReverse	= needReverse;
	_delta			= delta;
	_maxDegreeDelta	= maxDegreeDelta;
	_pTarget		= pTarget;
	_x				= x;
	_y				= y;

	return true;
}

LXMoveTrack* LXMoveTrack::clone() const
{
	// no copy constructor
	auto a = new LXMoveTrack();
	a->initWithDuration(_speed,			
		_maxSpeed,		
		_minSpeed,		
		_degree,	
		_addSpeed,
		_addDegree,		
		_topSpeedLimit,	
		_needReverse,
		_delta,
		_maxDegreeDelta,
		_pTarget,
		_x,
		_y);
	a->autorelease();
	return a;
}

void LXMoveTrack::startWithTarget(Node *target)
{
	Action::startWithTarget(target);

	if (_pTarget == nullptr)
	{
		_bHasTarget = true;
	}
	else
	{
		_bHasTarget = false;
	}

	_rect = dynamic_cast<LXObject*>(_target)->GetCollideRect();
}

LXMoveTrack* LXMoveTrack::reverse() const
{
	return LXMoveTrack::create(_speed,			
		_maxSpeed,		
		_minSpeed,		
		_degree,	
		_addSpeed,
		_addDegree,		
		_topSpeedLimit,	
		_needReverse,
		_delta,
		_maxDegreeDelta,
		_pTarget,
		_x,
		_y);
}


void LXMoveTrack::step(float dt)
{
	if (_target)
	{
		LXMoveBy::step(dt);

		LXWeapon *pWeapon = dynamic_cast<LXWeapon *>(_target);
		if (pWeapon)
		{
			// 没有目标对象重新定位目标
			if (_pTarget == nullptr)
			{
				if (_x == 0.0f && _y == 0.0f || !_bHasTarget)
				{
					return;
				}

				if (pWeapon->IsPlayer())
				{
					_pTarget = ObjectManager().GetRandomObject(false);
				}
				else
				{
					_pTarget = ObjectManager().GetRandomObject(true);
				}
			}
			else
			{
				// 判定当前状态
				LXPlayerPlane *pPlayer = dynamic_cast<LXPlayerPlane *>(ObjectManager().GetPlayer());
				if (pPlayer->GetFireState() == false || _pTarget->IsAlive() == false)
				{
					_pTarget = NULL;
					return;
				}

				_bHasTarget = false;
			}

			// 位置信息

			float targetX = 0.0f;
			float targetY = 0.0f;
			if (_pTarget == nullptr)
			{
				targetX = _x;
				targetY = _y;
			}
			else
			{
				targetX = _pTarget->getPosition().x;
				targetY = _pTarget->getPosition().y;
			}

			float srcX = _target->getPosition().x;
			float srcY = _target->getPosition().y;

			// 计算距离
			float dist = (float)std::sqrt((double)((srcX - targetX) * (srcX - targetX) + (srcY - targetY) * (srcY - targetY)));
			if (dist != 0.0f)
			{
				float degree = (float)(57.2957763671875 * std::acos((double)(targetX - srcX) / dist));
				if(targetY > srcY) {
					degree = 360.0F - degree;
				}

				degree = degree - _degree;
				if (std::abs(degree) > 180.0F)
				{
					if(degree > 0.0F) {
						degree -= 360.0F;
					} else {
						degree += 360.0F;
					}
				} 

				float degreeTarget = dt * _maxDegreeDelta;
				if(std::abs(degree) > degreeTarget) {
					if(degree > 0.0F) {
						degree = degreeTarget;
					} else {
						degree = -degreeTarget;
					}
				}

				_degree += degree;
				pWeapon->SetBulletDegree(_degree);
				pWeapon->CalcBulletDegree();
			}
		}
	}
}
