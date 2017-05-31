#include "LXBombPlane.h"
#include "Shooter/LXShooter.h"
#include "Shooter/LXShooterManager.h"
#include "Effect/LXEffectManager.h"
#include "Effect/ParticleEffect.h"
#include "GameDefine.h"
#include "DB/DBManager.h"
#include "ObjectEx/LXObjectManager.h"
#include "Action/CCActionIntervalEx.h"
#include "Sound/LXSoundSystem.h"
#include "LXPlayerPlane.h"
#include "Scene/GameScene.h"

const int BombDir[] = {0, 30, -30, 30, 0, -30, 30, -30};
LXBombPlane::LXBombPlane()
	: LXPlane()
	, m_bBomb(false)
	, m_fBombTime(0.0f)
{
}

LXBombPlane::~LXBombPlane() 
{
}

void LXBombPlane::Init(int value)
{
	LXPlane::Init(value);
	// 初始化必杀
	m_Rect = Rect(-42, -35, 85, 60);
	std::string path = "dijiall";
	std::string key = "baoxianfeiji";
	PlayAnimaton(path, key);
	
	std::string shooterName = "baoxian";
	LXShooterGroup *pShooters = ShooterManager().GetShooterByName(shooterName);
	if (pShooters)
	{
		InitShooters(pShooters);
	}

	// 播放特效
	std::string effectName = "p0/baoxian.p";
	EffectManager().CreateEffect(this, effectName, 0, 0, GAME_ZORDER_OBJECT_PARTICLE, false, true);
	
	// 设置攻击力
	m_nAttack = 100;
	m_fBombTime = 0.0f;
}

void LXBombPlane::Tick(float fDelta)
{
	LXPlane::Tick(fDelta);
	
	if (ObjectManager().GetPlayer())
	{
		if (ObjectManager().GetPlayer()->IsPause())
		{
			return;
		}

		if(dynamic_cast<LXPlayerPlane*>(ObjectManager().GetPlayer())->GetFireState())
		{
			m_fBombTime += fDelta;
		}
	}
	
	if (!m_bBomb)
	{
		int bombLevel = DBManager().GetData(DATA_BOMB_LEVEL);
		if(m_fBombTime >= 4.0F + 0.7F * bombLevel) 
		{
			m_bBomb = true;
			StopShootLogic();

			LXObject* pObject = ObjectManager().GetRandomObject(false);
			float degree = 0.0f;
			if (pObject)
			{
				float sx = getPositionX();
				float sy = getPositionY();
				float tx = pObject->getPositionX();
				float ty = pObject->getPositionY();

				if( sx == tx && sy == ty) 
				{
					degree = 0.0F;
				}
				else 
				{
					float var4 = tx - sx;
					float var5 = ty - sy;
					degree = 57.295776F * (float)std::acos((double)(var4 / (float)std::sqrt((double)(var4 * var4 + var5 * var5))));
					if(var5 > 0.0F) 
					{
						degree = 360.0F - degree;
					}
				}
			}
			else
			{
				degree = (float)(90 + BombDir[m_nResID]);		// 类型 this.c
			}

			// 撞向飞机
			runAction(Sequence::create(MoveBy::create(1.0f, Vec2(1000.0F * std::cos(CC_DEGREES_TO_RADIANS(degree)), -(1000.0F * std::sin(CC_DEGREES_TO_RADIANS(degree))))), Finish::create(true), NULL));
		}
	}
	
	bool bHit = ObjectManager().BombHitLogic(this);
	if(bHit)
	{
		SetCleanUp(true);
		//SoundSystem().playEffect(Mic_boom_m);
	}
}

void LXBombPlane::Destroy()
{
	LXPlane::Destroy();
}
