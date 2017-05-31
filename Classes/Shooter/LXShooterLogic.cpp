#include "LXShooterLogic.h"
#include "LXShooter.h"
#include "ObjectEx/Weapon/LXWeapon.h"
#include "ObjectEx/Weapon/LXTrackWeapon.h"
#include "ObjectEx/Weapon/LXSpeedWeapon.h"
#include "ObjectEx/Weapon/LXParentWeapon.h"
#include "ObjectEx/Weapon/LXLaserWeapon.h"
#include "ObjectEx/Weapon/LXCommonWeapon.h"
#include "ObjectEx/Weapon/LXBombWeapon.h"
#include "ObjectEx/LXObjectManager.h"
#include "Action/CCActionIntervalEx.h"
#include "GameDefine.h"
#include "Sound/LXSoundSystem.h"
#include "Scene/GameScene.h"
#include "Scene/MainScene.h"

LXShooterLogic::LXShooterLogic()
	: m_pParent(nullptr),
	m_pWeapon(nullptr),
	m_pShooter(nullptr),
	m_fOffX(0.0f),
	m_fOffY(0.0f),
	m_fEffectDelat(0.0f),
	m_nTag(-1)
{
}

LXShooterLogic::~LXShooterLogic() 
{
}

void LXShooterLogic::ChangeAnchorPoint(float x, float y)
{
	m_fOffX += x;
	m_fOffY += y;
}

void LXShooterLogic::Fire(float fDegree, float fSpeed)
{
	// 创建武器
	LXObject *pObject = nullptr;
	if (m_pShooter->GetType() != shooter_type_laser || m_pWeapon == nullptr )
	{
		Node *pParent = nullptr;
		if(m_pParent->IsUIObject())
		{
			pParent = m_pParent->getParent();
		}
		else
		{
			pParent = dynamic_cast<GameScene*>(ObjectManager().GetGameSceneNode())->GetNodeByZOrder(GAME_ZORDER_PLAYER_BULLET);
		}

		switch (m_pShooter->GetType())
		{
		case shooter_type_common:
			{
				pObject = ObjectManager().CreateObject(m_pShooter->GetResId(), pParent, object_type_weapon_common, m_pParent->IsPlayer());
			}
			break;
		case shooter_type_track:
			{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
				pObject = ObjectManager().CreateObject(m_pShooter->GetResId(), pParent, object_type_weapon_track, m_pParent->IsPlayer());
#else 
				if (m_pParent->GetObjectType() == object_type_plane_player && m_pParent->GetResID() == 1)
				{
					pObject = ObjectManager().CreateObject(m_pShooter->GetResId(), pParent, object_type_weapon_track, m_pParent->IsPlayer());
				}
#endif
			}
			break;
		case shooter_type_speed:
			{
				pObject = ObjectManager().CreateObject(m_pShooter->GetResId(), pParent, object_type_weapon_speed, m_pParent->IsPlayer());
			}
			break;
		case shooter_type_parent:
			{
				pObject = ObjectManager().CreateObject(m_pShooter->GetResId(), pParent, object_type_weapon_parent, m_pParent->IsPlayer());
			}
			break;
		case shooter_type_laser:
			{
				pObject = ObjectManager().CreateObject(m_pShooter->GetResId(), pParent, object_type_weapon_laser, m_pParent->IsPlayer());
				dynamic_cast<LXLaserWeapon*>(pObject)->SetPos(Vec2(m_fOffX, m_fOffY));
			}
			break;
		case shooter_type_bomb:
			{
				pObject = ObjectManager().CreateObject(m_pShooter->GetResId(), pParent, object_type_weapon_bomb, m_pParent->IsPlayer());
			}
			break;
		default:
			{
				pObject = ObjectManager().CreateObject(m_pShooter->GetResId(), pParent, object_type_weapon_bomb, m_pParent->IsPlayer());
			}
			break;
		}
		

	}

	m_pWeapon = dynamic_cast<LXWeapon*>(pObject);
	if (m_pWeapon)
	{
		// 初始化武器
		m_pWeapon->SetUIObject(m_pParent->IsUIObject());
		m_pWeapon->Init(m_pShooter->GetResId());
		m_pWeapon->setPosition(ccpAdd(m_pParent->getPosition(), ccp(m_fOffX, -m_fOffY)));

		// 计算子弹攻击力
		m_pWeapon->SetAttack(m_pParent->GetAttack() * m_pWeapon->GetAttackFactor() / 100);
	
		// 设置子弹速度
		m_pWeapon->SetBulletSpeed(fSpeed);

		// 设置子弹发射角度
		m_pWeapon->SetBulletDegree(fDegree);

		// 初始化子弹朝向
		m_pWeapon->CalcBulletDegree();

		// 设置子弹发射逻辑
		m_pWeapon->SetShooterLogic(this);

		// 动作初始化
		m_pWeapon->InitAction();

		// 播放音效
		//m_pWeapon->PlayShootEffect();
	}
}

void LXShooterLogic::InitAction()
{
	if (m_nTag == -1)
	{
		m_nTag = ObjectManager().GetTag();
	}
	m_pParent->stopActionByTag(m_nTag);
	Action* pAction = LXShoot::create(this, 
		m_pShooter->GetGroup(), 
		m_pShooter->GetGroupWaitTime(),
		m_pShooter->GetGroupDegreeDelta(),
		m_pShooter->GetNum(),
		m_pShooter->GetWaitTime(),
		m_pShooter->GetDegreeDelta(),
		m_pShooter->GetSpeedDelta(),
		m_pShooter->GetDelay());

	pAction->setTag(m_nTag);
	m_pParent->runAction(pAction);
}

void LXShooterLogic::StopAction()
{
	if (m_pParent && m_nTag != -1)
	{
		m_pParent->stopActionByTag(m_nTag);
		m_nTag = -1;
	}

	if (m_pShooter->GetType() == shooter_type_laser && m_pWeapon)
	{
		// 激光武器清理
		m_pWeapon->SetCleanUp(true);
		m_pWeapon = nullptr;
	}
}

float LXShooterLogic::CalcDegree()
{
	// 获取玩家
	LXObject *pObject = ObjectManager().GetPlayer();
	if (pObject)
	{
		Point point = m_pParent->getPosition();
		point = ccpAdd(point, ccp(m_fOffX, -m_fOffY));
		Point disPoint = ccpSub(point, pObject->getPosition());
		
		// 计算武器与玩家之前的距离

		float degree = 0.0f;
		float dist = (float)std::sqrt((double)(disPoint.x * disPoint.x + disPoint.y * disPoint.y));
		if (dist != 0)
		{
			float degree = (float)(57.2957763671875 * std::acos((double)((pObject->getPosition().x - point.x) / dist)));
			if(pObject->getPosition().y > point.y) {
				degree = 360.0F - degree;
			}
			return degree;
		}
	}

	return 0.0f;
}

void LXShooterLogic::PlayEffect()
{
	
}