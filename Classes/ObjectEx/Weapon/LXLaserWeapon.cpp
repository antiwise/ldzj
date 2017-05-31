#include "LXLaserWeapon.h"
#include "Action/CCActionIntervalEx.h"
#include "Shooter/LXShooterLogic.h"
#include "Shooter/LXShooter.h"
#include "ObjectEx/LXObjectManager.h"
#include "ObjectEx/Plane/LXPlayerPlane.h"
#include "GameDefine.h"
#include "Common/FileUtils.h"
#include "Effect/LXEffectManager.h"
#include "Scene/GameScene.h"

LXLaserWeapon::LXLaserWeapon()
	: LXWeapon(),
	m_fLastTick(0.0f),
	m_bCanThrough(false),
	m_nPos(0),
	m_nPosOffset(5),
	m_nTickNum(0),
	m_pObject(nullptr),
	m_fHeight(0),
	m_Effect_Fire(nullptr),
	m_Effect_Hited(nullptr),
	m_fScale(1.0f)
{
}

LXLaserWeapon::~LXLaserWeapon() 
{
	for(std::vector<Sprite*>::reverse_iterator it = m_SpritePool.rbegin(); it != m_SpritePool.rend(); it++)
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
}

void LXLaserWeapon::Init(int value)
{
	LXWeapon::Init(value);

}

void LXLaserWeapon::Tick(float fDelta)
{
	auto glview = Director::getInstance()->getOpenGLView();
	const auto screenSize = glview->getDesignResolutionSize();
	m_fHeight = screenSize.height;
	LXWeapon::Tick(fDelta);

	// 设置位置
	setPosition(ccpAdd(m_pShooterLogic->GetParent()->getPosition(), m_Pos));
	
	if ( m_Effect_Fire )
	{
		m_Effect_Fire->setPosition( getPositionX(), getPositionY());
	}

	if ( m_Effect_Hited )
	{
		m_Effect_Hited->setPosition( 0, m_fHeight - 10 );
	}

	m_fLastTick = fDelta;
	m_nPos = (m_nPos + m_nPosOffset) % (int)m_Rect.size.height;

	for (int i = 0; i < m_SpritePool.size(); i++)
	{
		if (m_SpritePool[i])
		{
			m_SpritePool[i]->setVisible(false);
		}
	}
	int num = 0;
	// 拼接三个精灵
	Sprite *pSprite = nullptr;
	if (m_fHeight < m_nPos)
	{
		Sprite *pSprite = GetRenderElement(num++);
		pSprite->setTextureRect(Rect(m_Rect.origin.x, m_Rect.origin.y + m_nPos - m_fHeight, m_Rect.size.width, m_fHeight), false, Size(m_Rect.size.width, m_fHeight));
		if (pSprite->getParent() == nullptr)
		{	
			m_pRenderNode->addChild(pSprite);
		}
	}
	else
	{
		Sprite *pSprite = GetRenderElement(num++);
		pSprite->setTextureRect(Rect(m_Rect.origin.x, m_Rect.origin.y, m_Rect.size.width, m_nPos), false, Size(m_Rect.size.width, m_nPos));
		if (pSprite->getParent() == nullptr)
		{	
			m_pRenderNode->addChild(pSprite);
		}
	}
	int nums = (int)(((int)m_fHeight - m_nPos) / (int)m_Rect.size.height);
	int leftHeight = (int)(((int)m_fHeight - m_nPos) % (int)m_Rect.size.height);

	for(int i = 0; i < nums; i++)
	{
		Sprite *pSprite = GetRenderElement(num++);
		pSprite->setTextureRect(Rect(m_Rect.origin.x, m_Rect.origin.y, m_Rect.size.width, m_Rect.size.height), false, m_Rect.size);
		pSprite->setPosition(ccp(0, m_Rect.size.height*i + m_nPos - (i + 1)));
		if (pSprite->getParent() == nullptr)
		{	
			m_pRenderNode->addChild(pSprite);
		}
	}

	if (leftHeight > 0)
	{
		Sprite *pSprite = GetRenderElement(num++);
		pSprite->setTextureRect(Rect(m_Rect.origin.x, m_Rect.origin.y + m_Rect.size.height - leftHeight, m_Rect.size.width, leftHeight), false, Size(m_Rect.size.width, leftHeight));
		pSprite->setPosition(ccp(0, m_Rect.size.height*nums + m_nPos - (1 + nums)));
		if (pSprite->getParent() == nullptr)
		{	
			m_pRenderNode->addChild(pSprite);
		}
	}

}

void LXLaserWeapon::Destroy()
{
	LXWeapon::Destroy();

	if ( m_Effect_Hited )
	{
		EffectManager().DeleteEffect(m_Effect_Hited);
		m_Effect_Hited = nullptr;
	}

	if ( m_Effect_Fire )
	{
		EffectManager().DeleteEffect(m_Effect_Fire);
		m_Effect_Fire = nullptr;
	}
}

void LXLaserWeapon::InitAction()
{

}

void LXLaserWeapon::SetShooterLogic(LXShooterLogic* pLogic)
{
	LXWeapon::SetShooterLogic(pLogic);

	// 计算屏幕高度
	m_fLastTick = 0.0f;

	// 设置渲染信息
	std::string hitEffectName;
	std::string weaponName;
	switch (m_nResID)
	{
	case 17:
		{
			hitEffectName = "p0/laser_hited.p";
			weaponName = "laser1";				 
			m_nPosOffset = 25;
		}
		break;
	case 25:
	case 26:
	case 37:
		{
			hitEffectName = "p0/laser_hited_red2.p";
			weaponName = "laser3";				 
			m_nPosOffset = 15;
		}
		break;
	case 27:
	case 28:
		{
			hitEffectName = "p0/laser_hited_red.p";
			weaponName = "laser3";				 
			m_nPosOffset = 15;
		}
		break;
	case 29:
		{
			hitEffectName = "p0/laser_hited.p";
			weaponName = "laser2";				 
			m_nPosOffset = 25;
		}
		break;
	default:
		break;
	}

	std::string path = "sucai_" + weaponName + ".png";
	if (m_ResPath != path)
	{
		m_ResPath = "sucai_" + weaponName + ".png";
		SpriteFrame* pSpriteFrame = SpriteFrameCache::sharedSpriteFrameCache()->getSpriteFrameByName(m_ResPath);
		m_Rect = pSpriteFrame->getRect();

		for(std::vector<Sprite*>::reverse_iterator it = m_SpritePool.rbegin(); it != m_SpritePool.rend(); it++)
		{
			(*it)->setSpriteFrame(pSpriteFrame);
			(*it)->setAnchorPoint(ccp(0.5, 0));
		}
	}
	// 开火特效
	//this.U = p.a("laser_fire", p.a(), this.getX(), this.getY(), true);
	Node *pParent = nullptr;
	if(IsUIObject())
	{
		pParent = getParent();
	}
	else
	{
		pParent = dynamic_cast<GameScene*>(ObjectManager().GetGameSceneNode())->GetNodeByZOrder(GAME_ZORDER_BULLET_EFFECT);
	}
	if (m_Effect_Fire == nullptr )
	{
		std::string strName = "p0/laser_fire.p";
		
		m_Effect_Fire = EffectManager().CreateEffect(pParent,strName, getPositionX(), getPositionY(),GAME_ZORDER_BULLET_EFFECT,true);
	}

	// 击中特效
	//this.T = p.a(var4, p.b(), 0.0F, 0.0F, true);
	if (m_Effect_Hited == nullptr )
	{
		m_Effect_Hited = EffectManager().CreateEffect(this,hitEffectName,0,0,GAME_ZORDER_BULLET_EFFECT,true);
	}

	// 隐藏击中特效
	//this.T.setVisible(false);
	//m_Effect_Hited->GetParent()->setVisible(false);

	// 设置缩放
	switch(m_nResID) 
	{
	case 25:
	case 37:
		{
			m_fScale = 0.25F;
		}
		break;
	case 26:
		{
			m_fScale = 0.5F;
		}
		break;
	case 27:
		{
			m_fScale = 0.75F;
		}
		break;
	default:
			m_fScale = 1.0F;
		break;
	}
}

void LXLaserWeapon::PlayShootEffect()
{

}

void LXLaserWeapon::playerWeaponLogic()
{
	// 检测与敌机的碰撞
	LXPlayerPlane *pPlayer = dynamic_cast<LXPlayerPlane*>(ObjectManager().GetPlayer());
	if (pPlayer->GetFireState())	// 主角当前允许发射子弹
	{
		LXObject *pTarget = ObjectManager().PlayerLaserHitLogic(this, m_fLastTick);
		if (pTarget)
		{
			m_fHeight = pTarget->getPosition().y - pPlayer->getPosition().y;
		}
	}
}

void LXLaserWeapon::AIWeaponLogic()
{
	LXPlayerPlane *pPlayer = dynamic_cast<LXPlayerPlane*>(ObjectManager().GetPlayer());

	//护盾状态玩家免伤
	if (pPlayer->GetProtectTime()>0)
	{
		return;
	}

	LXObject *pTarget = ObjectManager().EnemyLaserHitLogic(this, m_fLastTick);
	if (pTarget)
	{
		m_fHeight = m_pShooterLogic->GetParent()->getPosition().y - pTarget->getPosition().y;
	}

	//		不是无敌状态												// 是否活着				// 是否受击
	if ( ( static_cast<int>(pPlayer->GetInvincibleTime()) <= 0 ) && ( pPlayer->IsAlive() ) && pTarget )
	{
		// m_bHit = true;
		// m_bCleanFlag = true;

		ObjectManager().PlayerPlaneBeHit();
	}
}

Sprite* LXLaserWeapon::GetRenderElement(int index)
{
	// 创建缓存对象
	if (index == m_SpritePool.size())
	{
		Sprite *pSprite = Sprite::createWithSpriteFrameName(m_ResPath);
		pSprite->setAnchorPoint(ccp(0.5, 0));
		pSprite->setScaleX(m_fScale);
		m_SpritePool.push_back(pSprite);
	}

	m_SpritePool[index]->setVisible(true);
	return m_SpritePool[index];
}
