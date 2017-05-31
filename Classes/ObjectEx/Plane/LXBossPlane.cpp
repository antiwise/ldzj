#include "LXBossPlane.h"
#include "DataBase/LXDBC_Struct.h"
#include "DataBase/DataBase.h"
#include "Common/LXUtils.h"
#include "Animation/AnimationLogic.h"
#include "DB/DBManager.h"
#include "Scene/MainScene.h"
#include "Scene/GameScene.h"
#include "Sound/LXSoundSystem.h"
#include "GameDefine.h"
#include "Effect/LXEffectManager.h"
#include "ObjectEx/LXObjectManager.h"

LXBossPlane::LXBossPlane()
	: LXEnemyPlane(),
	m_pBossData(nullptr)
{
}

LXBossPlane::~LXBossPlane() 
{
}

void LXBossPlane::Init(int value)
{
	LXEnemyPlane::Init(value);

	// 这里是否需要向关卡注册BOSS信息?? 
	 
	// Do something other??
}

void LXBossPlane::InitData()
{
	if (m_pEnemyData && m_pEnemyData->nType == enemy_type_boss)
	{
		int bossID = m_pEnemyData->nResID;

		DBC_DEFINEHANDLE(s_pBossDBC, DBC_BOSS);
		m_pBossData = (const _DBC_BOSS*)s_pBossDBC->Search_LineNum_EQU(bossID);
		
		SplitBossCollideInfo();
		
		SetDropItemID( m_pEnemyData->nItemID );
		SetDropItemNum( m_pEnemyData->nDiamondNum );
		SetScore( m_pEnemyData->nScore );
		SetDieAnimate( m_pBossData->szDeadEffect );
	}
}

void LXBossPlane::Tick(float fDelta)
{
	LXEnemyPlane::Tick(fDelta);
}

void LXBossPlane::PlayDestroyEffect()
{
	Node *pParent = dynamic_cast<GameScene*>(ObjectManager().GetGameSceneNode())->GetNodeByZOrder(GAME_ZORDER_OBJECT_PARTICLE);
	std::string strName = "p0/bossdeath.p";
	EffectManager().CreateEffect(pParent,strName,this->getPositionX(),this->getPositionY(),GAME_ZORDER_OBJECT_PARTICLE,false);
}

void LXBossPlane::PlayDestroySound()
{
	SoundSystem().playEffect(Mic_boom_l);
}

void LXBossPlane::DestroyLogic()
{
	LXEnemyPlane::DestroyLogic();
}

void LXBossPlane::Destroy()
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

std::string	LXBossPlane::GetStartAnimationPath()
{
	if(m_pBossData)
	{
		return m_pBossData->szFileName;
	}

	return "";
}

std::string	LXBossPlane::GetStartAnimationName()
{
	if(m_pBossData)
	{
		return m_pBossData->szAnimation;
	}

	return "";
}

void LXBossPlane::SplitBossCollideInfo()
{
	if(m_pBossData)
	{
		std::string colInfo = m_pBossData->szRect2;
		// 去左右"
		colInfo = colInfo.substr(1, colInfo.length() - 2);

		// 拆分
		std::vector<std::string> rectVec;
		Utils().split(colInfo, rectVec, "/");
		for (int i = 0; i < rectVec.size(); i++)
		{
			// 拆分子项
			std::vector<std::string> pointVec;
			Utils().split(rectVec[i], pointVec, ",");
			if (pointVec.size() == 4)
			{
				int a = atoi(pointVec[0].c_str());
				int b = atoi(pointVec[1].c_str());
				int c = atoi(pointVec[2].c_str());
				int d = atoi(pointVec[3].c_str());

				if (i == 0)
				{
					SetCollideInfo(Rect(a, b, c, d));
				}
			}
		}
	}
}


void LXBossPlane::DropCrystal()
{
	int var1 = GetDropItemNum() / 12;		// 16
	int var2 = GetDropItemNum() / 8;		// 25
	int var3 = GetDropItemNum() - var2 * 2 - var1 * 3;		// 102

	std::vector<int> vec;
	vec.push_back(var3);
	vec.push_back(var2);
	vec.push_back(var1);

	CreatCrystal( DBManager().IsCommonMission(), vec );

}

void LXBossPlane::DropItem()
{
	if( DBManager().IsBossMission() )		// boss
	{
		int var1 = rand()%99;
		if( DBManager().GetCurrMissionLevel() == 1 && var1 < 20 || DBManager().GetCurrMissionLevel() == 2 && var1 < 30) 
		{
			
			int var2 = 3;
			if ( rand() % 2 == 0 )
			{
				var2 = 4;
			}

			std::vector<int> vec;
			vec.push_back(var2);

			CreatCrystal(false,vec);
		}
	}


}

bool LXBossPlane::CheckHP(int value)
{
	int weekNum = DBManager().GetData(DATA_GAME_WEEK);
	
	if ( weekNum == 2 )
	{
		value = value * 3 / 2;

	}
	else if ( weekNum > 2 )
	{
		value *= 2; 
	}

	bool bValue = LXObject::CheckHP( value );

	if ( bValue )
	{
        //w.c("boom_m.ogg");											// 死亡音效
		Node *pParent = dynamic_cast<GameScene*>(ObjectManager().GetGameSceneNode())->GetNodeByZOrder(GAME_ZORDER_OBJECT_PARTICLE);
		std::string strName = "p0/boom_m.p";
		EffectManager().CreateEffect(pParent,strName,this->getPositionX(),this->getPositionY(),GAME_ZORDER_OBJECT_PARTICLE,false);
		SoundSystem().playEffect(Mic_boom_m);
		CreatItem(1);

		// 子弹清理
		ObjectManager().CleanEnemyWeapon();
	}

	return bValue;
}

void	LXBossPlane::Hit(int damage)
{
	//每伤血1/7播放受伤动画
	for(int i = 1; i <= 5; ++i) 
	{
		int bloodStep = i * m_nTotalHP / 7;			// 1/7 - 5/7 总血量

		// 如果血量小于指定数值 跳过
		if(m_nCurrentHP < bloodStep) {
			break;
		}

		if(m_nCurrentHP >= bloodStep && m_nCurrentHP - damage < bloodStep) 
		{
			int x = rand()%100 - 50;
			int y = rand()%100 - 50;
			std::string strName = "p0/hurt.p";
			EffectManager().CreateEffect(this,strName, x,  y,GAME_ZORDER_OBJECT_PARTICLE,false);
		}
	}
	
	LXObject::Hit(damage);
	
	//设置血条百分比

	auto director = Director::getInstance();
	auto pSence = director->getRunningScene();
	auto mainlayer = pSence->getChildByName("mainSence");
	
	static_cast<MainScene*>(mainlayer)->GetGameScene()->SetBossBloodBar(m_nCurrentHP*100/m_nTotalHP);
}