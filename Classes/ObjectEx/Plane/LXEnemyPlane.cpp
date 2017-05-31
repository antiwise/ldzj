#include "LXEnemyPlane.h"
#include "DataBase/DataBase.h"
#include "Common/LXUtils.h"
#include "Common/FileUtils.h"
#include "Shooter/LXShooterManager.h"
#include "DB/DBManager.h"
#include "Sound/LXSoundSystem.h"
#include "GameDefine.h"
#include "Effect/LXEffectManager.h"
#include "ObjectEx/LXObjectManager.h"
#include "Scene/GameScene.h"

LXEnemyPlane::LXEnemyPlane()
	: LXPlane(),
	m_pEnemyData(nullptr),
	m_pPlaneData(nullptr),
	m_pBloodBar(nullptr),
	m_pBloodBack(nullptr)
{
	
}

LXEnemyPlane::~LXEnemyPlane() 
{
}

void LXEnemyPlane::Relive()
{
	LXPlane::Relive();

	if (m_pEnemyData)
	{
		// 战机血量
		m_nCurrentHP = m_pEnemyData->nHP;
		m_nTotalHP = m_pEnemyData->nHP;

		if (m_pBloodBar)
		{
			m_pBloodBar->setPercentage(100);
		}
	}

	// 播放飞机初始动画
	std::string animationName = GetStartAnimationName();
	if (animationName.length() > 0)
	{
		// 解析
		std::vector<std::string> vecName;
		Utils().split(animationName, vecName, ",");
		std::string strAnimationPath = GetStartAnimationPath();
		if (vecName.size() > 1)		// 修订版本
		{
			m_NameFix = vecName[1];
			// 播放动画
			PlayAnimaton(strAnimationPath, vecName[0], m_NameFix);
		}
		else
		{
			PlayAnimaton(strAnimationPath, animationName);
		}
	}
}

void LXEnemyPlane::Init(int value)
{
	if(m_nResID == value)
	{
		Relive();
		return;
	}

	// 加载怪物信息
	DBC_DEFINEHANDLE(s_pEnemyDBC, DBC_ENEMY);
	m_pEnemyData = (const _DBC_ENEMY*)s_pEnemyDBC->Search_LineNum_EQU(value);

	// 初始化数据
	InitData();

	if (m_pEnemyData)
	{
		// 战机血量
		m_nCurrentHP = m_pEnemyData->nHP;
		m_nTotalHP = m_pEnemyData->nHP;

		// 弹幕初始化
		if (strlen(m_pEnemyData->szShooter) > 0 )
		{
			std::string str = m_pEnemyData->szShooter;
			LXShooterGroup *pShooters = ShooterManager().GetShooterByName(str);
			if (pShooters)
			{
				InitShooters(pShooters);
			}
		}
	}

	// 播放飞机初始动画
	std::string animationName = GetStartAnimationName();
	if (animationName.length() > 0)
	{
		// 解析
		std::vector<std::string> vecName;
		Utils().split(animationName, vecName, ",");
		std::string strAnimationPath = GetStartAnimationPath();
		if (vecName.size() > 1)		// 修订版本
		{
			m_NameFix = vecName[1];
			// 播放动画
			PlayAnimaton(strAnimationPath, vecName[0], m_NameFix);
		}
		else
		{
			PlayAnimaton(strAnimationPath, animationName);
		}
	}

	LXPlane::Init(value);
}

void LXEnemyPlane::InitData()
{
	if (m_pEnemyData && m_pEnemyData->nType == enemy_type_monster)
	{
		DBC_DEFINEHANDLE(s_pPlaneDBC, DBC_PLANE);
		m_pPlaneData = (const _DBC_PLANE*)s_pPlaneDBC->Search_LineNum_EQU(m_pEnemyData->nResID);
		
		SetCollideInfo(Rect(m_pPlaneData->nRect1, m_pPlaneData->nRect2, m_pPlaneData->nRect3, m_pPlaneData->nRect4));
		
		// 战机血量
		m_nCurrentHP = m_pEnemyData->nHP;
		m_nTotalHP = m_pEnemyData->nHP;

		SetDropItemID( m_pEnemyData->nItemID );
		SetDropItemNum( m_pEnemyData->nDiamondNum );
		SetScore( m_pEnemyData->nScore );
		SetDieAnimate( m_pPlaneData->szDeadEffect );

		// 弹幕初始化
		if (strlen(m_pEnemyData->szShooter) > 0 )
		{
			std::string str = m_pEnemyData->szShooter;
			LXShooterGroup *pShooters = ShooterManager().GetShooterByName(str);
			if (pShooters)
			{
				InitShooters(pShooters);
			}
		}

		//精英怪添加血条初始化
		std::string strEffect = m_pPlaneData->szDeadEffect;
		if (strEffect == "boom_m")
		{
			SpriteFrame* pFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("play_15.png");
			m_pBloodBack = Sprite::createWithSpriteFrame(pFrame);
			m_pBloodBack->setPosition(0,-60);
			this->addChild(m_pBloodBack);

			SpriteFrame* pFrame1 = SpriteFrameCache::getInstance()->getSpriteFrameByName("play_15-2.png");
			auto m_pBlood = Sprite::createWithSpriteFrame(pFrame1);
			m_pBloodBar = ProgressTimer::create(m_pBlood);
			m_pBloodBar->setType(kCCProgressTimerTypeBar);
			m_pBloodBar->setMidpoint(Vec2(0,1));
			m_pBloodBar->setBarChangeRate(Vec2(1,0)); //设置X,Y轴是否随着进度变化
			m_pBloodBar->setPercentage(100);
			m_pBloodBar->setPosition(0,-60);
			this->addChild(m_pBloodBar);
		}
		
		
	}
}

void LXEnemyPlane::Tick(float fDelta)
{
	LXPlane::Tick(fDelta);

}

void LXEnemyPlane::PlayDestroyEffect()
{
	string effect = "p0/" + string(m_pPlaneData->szDeadEffect) + ".p";
	Node *pParent = dynamic_cast<GameScene*>(ObjectManager().GetGameSceneNode())->GetNodeByZOrder(GAME_ZORDER_OBJECT_PARTICLE);
	EffectManager().CreateEffect(pParent, effect, this->getPositionX(), this->getPositionY(), GAME_ZORDER_OBJECT_PARTICLE, false);
}

void LXEnemyPlane::PlayDestroySound()
{
	std::string strEffect = m_pPlaneData->szDeadEffect;
	if (strEffect == "boom_m")
	{
		SoundSystem().playEffect(Mic_boom_m);
	}
	else
	{
		SoundSystem().playEffect(Mic_boom_s);
	}
	
}

void LXEnemyPlane::DestroyLogic()
{
	
	// 水晶掉落
	DropCrystal();

	// 如果不是教学和关卡模式， 或者掉落物品id为1
	bool isCommonMisson = DBManager().IsCommonMission();
	if ( !isCommonMisson || GetDropItemID() == 1 || GetDropItemID() == 2)
	{
		DropItem();
	}	


	// 战机死亡个数+1
	DBManager().AddMissionKilled();

	// 计算玩家获得的分数
	if ( ! DBManager().IsCommonMission() )
	{
		if ( DBManager().GetCurrPlaneID() == 4 )
		{
			DBManager().AddMissionScore( 2*GetScore() );
		}else
		{
			DBManager().AddMissionScore( GetScore() );
		}
	}
}

void LXEnemyPlane::Destroy()
{
	LXPlane::Destroy();

	if (m_bDeadFlag)
	{
		// 特效
		PlayDestroyEffect();

		// 音效
		PlayDestroySound();

		// 死亡逻辑
		DestroyLogic();

		m_bDeadFlag = false;
	}

	m_bCleanFlag = false;
	
}

std::string LXEnemyPlane::GetStartAnimationPath()
{
	return "dijiall";
}

std::string	LXEnemyPlane::GetStartAnimationName()
{
	if(m_pPlaneData)
	{
		return m_pPlaneData->szAnimation;
	}

	return "";
}

void LXEnemyPlane::DropCrystal()
{
	if ( m_pPlaneData )
	{
		if ( GetDieAnimate() != "Null" ) 	 // 有死亡动画	精英怪
		{
			int var1 = GetDropItemNum() / 6;		// 第2种宝石

			int var2 = GetDropItemNum() - var1 * 2; // 第1种宝石

			std::vector<int> vec;
			vec.push_back(var2);
			vec.push_back(var1);

			CreatCrystal( DBManager().IsCommonMission(), vec );

		}else
		{

			std::vector<int> vec;
			vec.push_back( GetDropItemNum() );

			CreatCrystal( DBManager().IsCommonMission(), vec );
		}
	}
}

void	LXEnemyPlane::Hit(int damage)
{
	LXObject::Hit(damage);

	if (m_pBloodBack && m_pBloodBar)
	{
		m_pBloodBar->setPercentage(m_nCurrentHP*100/m_nTotalHP);
	}
}