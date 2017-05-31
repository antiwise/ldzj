#include "LXWeapon.h"
#include "DataBase/DataBase.h"
#include "Shooter/LXShooterLogic.h"
#include "Shooter/LXShooter.h"
#include "ObjectEx/LXObjectManager.h"
#include "ObjectEx/Plane/LXPlayerPlane.h"
#include "DB/DBManager.h"
#include "Sound/LXSoundSystem.h"
#include "Effect/LXEffectManager.h"
#include "GameDefine.h"

LXWeapon::LXWeapon()
	: LXObject(),
	m_pShooterLogic(nullptr),
	m_bResDir(false),
	m_bHit(false),
	m_bAnimation(true),
	m_pBulletData(nullptr)
{
}

LXWeapon::~LXWeapon() 
{
}

void LXWeapon::Relive()
{
	LXObject::Relive();

	if (m_pBulletData && m_pBulletData->nType != bullet_type_animiaton)
	{
		InitRenderInfo(m_pBulletData);
	}
}

void LXWeapon::Init(int value)
{
	m_bHit = false;
	
	if (m_nResID == value)
	{
		Relive();
		return;
	}
	
	LXObject::Init(value);

	// 获取武器数据
	DBC_DEFINEHANDLE(s_pBulletDBC, DBC_BULLET);
	m_pBulletData = (const _DBC_BULLET*)s_pBulletDBC->Search_LineNum_EQU(value);
	if (m_pBulletData)
	{
		InitRenderInfo(m_pBulletData);

		// 初始化包围盒
		SetCollideInfo(Rect(m_pBulletData->nRect1, m_pBulletData->nRect2, m_pBulletData->nRect3, m_pBulletData->nRect4));

		// 初始化攻击系数
		SetAttackFactor(m_pBulletData->nValue3);

		// 资源朝向
		m_bResDir = m_pBulletData->nValue1 == 0?false:true;
	}
}

void LXWeapon::InitRenderInfo(const _DBC_BULLET *pBulletData)
{
	if (pBulletData->nType == bullet_type_animiaton)
	{
		std::string str = pBulletData->szName;
		PlayAnimaton(str,g_AnimationName_g0);
	}
	else if (pBulletData->nType == bullet_type_particle)
	{
		m_bAnimation = false;
		// 播放特效
		std::string strName = pBulletData->szName;
		PlayEffect(EffectManager().GetFullPath(strName));
	}
}

void LXWeapon::Tick(float fDelta)
{
	LXObject::Tick(fDelta);
	
	if (!m_bUIObject && ObjectManager().GetPlayer() && ObjectManager().GetPlayer()->IsPause())
	{
		return;
	}

	bool inRange = false;
	
	// 判断子弹是否在有效范围
	Point point = getPosition();
	if(point.x >= -100.0F && point.x <= 580.0F && point.y >= -100.0F && point.y < 900.0F) {
        inRange = true;
    }

	if (!inRange && m_nObjectType != object_type_weapon_laser)
	{
		m_bCleanFlag = true;
	}  
	else
	{
		if (!m_bHit && m_bUIObject == false)		// 是否击中目标
		{
			if(IsPlayer()) {	// 玩家的子弹
				playerWeaponLogic();
            }
			else
			{
				AIWeaponLogic();
			}
		}
	}
}

void LXWeapon::Destroy()
{
	LXObject::Destroy();
}

void LXWeapon::CalcBulletDegree()
{
	if(m_bResDir) 
	{			// 子弹是否有方向
		short degree;
		if(IsPlayer()) 
		{
			degree = 270;
		} else 
		{
			degree = 90;
		}

		setRotation(GetBulletDegree() - degree);
	}
}

void LXWeapon::PlayShootEffect()
{
	switch (m_nResID)
	{
	case 15:
	case 16:
		//SoundSystem().playEffect(Mic_bullet1);
		//break;
	case 22:
	case 23:
	case 34:
		//SoundSystem().playEffect(Mic_bullet2);
		break;
	default:
		break;
	}
}

void LXWeapon::playerWeaponLogic()
{
	LXPlayerPlane *pPlayer = dynamic_cast<LXPlayerPlane*>(ObjectManager().GetPlayer());
	if (pPlayer->GetFireState())	// 主角当前允许发射子弹
	{
		bool bHit = ObjectManager().PlayerHitLogic(this);
		if (bHit)
		{
			m_bHit = true;
			m_bCleanFlag = true;
		}
	}
}

void LXWeapon::AIWeaponLogic()
{
	// 参考 b.a.b.b public void E()
	LXPlayerPlane *pPlayer = dynamic_cast<LXPlayerPlane*>(ObjectManager().GetPlayer());
	
	//护盾状态玩家免伤
	if (pPlayer->GetProtectTime()>0)
	{
		return;
	}

	//		不是无敌状态												// 是否活着				// 是否受击
	if ( ( static_cast<int>(pPlayer->GetInvincibleTime()) <= 0 ) && ( pPlayer->IsAlive() ) && ( ObjectManager().EnemyHitLogic(this) ) )
	{
		m_bHit = true;
		m_bCleanFlag = true;

		ObjectManager().PlayerPlaneBeHit();
	}
}
