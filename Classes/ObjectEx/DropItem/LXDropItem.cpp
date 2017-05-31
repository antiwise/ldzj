#include "LXDropItem.h"
#include "Action/CCActionIntervalEx.h"
#include "ObjectEx/LXObjectManager.h"
#include "ObjectEx/Plane/LXPlayerPlane.h"
#include "GameDefine.h"

// 道具动画名称
std::string itemName[] = {"jinbi06", "huoli02", "huoli03", "zhuanhuan05", "jiaxie04", "baoxian01", "jinbi06", "jinbi07", "jinbi08", "jinbi09", "jinbi10", "jinbi11", "zuanshi"};
LXDropItem::LXDropItem()
	: LXObject(),
	m_bTarget(false),
	m_fTargetRange(100),
	m_fTargetMoveSpeed(1000),
	m_fTargetTime(0.0f),
	m_fMoveTime(0.0f),
	m_bArrived(false),
	m_fTargetDelay(0.0f),
	m_fActionDelay(0.0f),
	m_fSpeed(0.0f),
	m_fMaxSpeed(0.0f),
	m_fMinSpeed(0.0f),
	m_fDegree(0.0f),
	m_fAddSpeed(0.0f),
	m_fAddDegree(0.0f),
	m_bTopSpeedLimit(false),
	m_bNeedReverse(false),
	m_nx(0),
	m_ny(0),
	m_nw(0),
	m_nh(0),
	m_bActionDone(false)
{
}

LXDropItem::~LXDropItem() 
{
}

void LXDropItem::Init(int value)
{
	LXObject::Init(value);

	// 掉落物品创建逻辑
	PlayAnimaton(itemName[value], g_AnimationName_g0, "");

	// 设置碰撞信息
	SetCollideInfo(Rect(-20, -20, 40, 40));
}

void LXDropItem::InitAction(float fDelay, 
		float speed, 						
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
	m_fActionDelay	= fDelay;
	m_fSpeed		= speed;
	m_fMaxSpeed		= maxSpeed;
	m_fMinSpeed		= minSpeed;
	m_fDegree		= degree;
	m_fAddSpeed		= addSpeed;
	m_fAddDegree	= addDegree;
	m_bTopSpeedLimit= topSpeedLimit;
	m_bNeedReverse	= needReverse;
	m_nx			= x;
	m_ny			= y;
	m_nw			= w;
	m_nh			= h;
}

void LXDropItem::Tick(float fDelta)
{
	LXObject::Tick(fDelta);
	
	if(m_bCleanFlag)
	{
		return;
	}

	if (ObjectManager().GetPlayer() && ObjectManager().GetPlayer()->IsPause())
	{
		return;
	}

	m_fActionDelay -= fDelta;
	m_fTargetDelay -= fDelta;

	if (m_fActionDelay < 0 && m_bActionDone == false)
	{
		LXMoveByRect *pAction = LXMoveByRect::create(m_fSpeed,			
		m_fMaxSpeed,		
		m_fMinSpeed,		
		m_fDegree,	
		m_fAddSpeed,
		m_fAddDegree,		
		m_bTopSpeedLimit,	
		m_bNeedReverse,
		m_nx,
		m_ny,
		m_nw,
		m_nh);

		runAction(pAction);
		m_bActionDone = true;
	}

	// 碰撞检测
	LXPlayerPlane *pPlayer = dynamic_cast<LXPlayerPlane*>(ObjectManager().GetPlayer());
	if (pPlayer && pPlayer->IsAlive())
	{
		if (m_bArrived)
		{
			// 通知玩家获取道具
			pPlayer->PickItem(this);
			m_bCleanFlag = true;
			return; 
		}

		int x = pPlayer->getPositionX() - getPositionX();
		int y = pPlayer->getPositionY() - getPositionY();
		if (m_bTarget && m_fTargetDelay <= 0)
		{
			// 接近逻辑
			if(m_fTargetTime == 0.0f)
			{
				m_fTargetTime = std::sqrt(x * x + y * y) / m_fTargetMoveSpeed;
			}

			// 计算目标位置
			float time = m_fTargetTime - m_fMoveTime;
			if (time <= 0)
			{
				setPosition(pPlayer->getPosition());
				// 下一帧执行清理逻辑
				m_bArrived = true;
			}
			else
			{
				// 
				float offsetX = fDelta * x / time;
				float offsetY = fDelta * y / time;

				setPosition(ccpAdd(getPosition(), ccp(offsetX, offsetY)));
				m_fMoveTime += fDelta;
			}
		}
		else
		{
			// 创建立即锁定目标与范围判定两种方式
			if (m_bTarget || ((x * x + y * y) < m_fTargetRange * m_fTargetRange))
			{
				m_bTarget = true;
			}
		}
	}
}

void LXDropItem::Destroy()
{
	// 清理
	LXObject::Destroy();

	m_bTarget = false;
	m_fTargetRange = 100;
	m_fTargetMoveSpeed = 1000;
	m_fTargetTime = 0.0f;
	m_fMoveTime = 0.0f;
	m_bArrived = false;
	m_fTargetDelay = 0.0f;
	m_fActionDelay = 0.0f;
	m_fSpeed = 0.0f;
	m_fMaxSpeed = 0.0f;
	m_fMinSpeed = 0.0f;
	m_fDegree = 0.0f;
	m_fAddSpeed = 0.0f;
	m_fAddDegree = 0.0f;
	m_bTopSpeedLimit = false;
	m_bNeedReverse = false;
	m_nx = 0;
	m_ny = 0;
	m_nw = 0;
	m_nh = 0;
	m_bActionDone = false;
}
