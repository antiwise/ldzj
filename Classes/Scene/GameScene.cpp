#include "GameScene.h"
#include "GameDefine.h"
#include "DataBase/DataBase.h"
#include "ObjectEx/Plane/LXBossPlane.h"
#include "ObjectEx/Plane/LXEnemyPlane.h"
#include "ObjectEx/Plane/LXPlayerPlane.h"
#include "ObjectEx/Plane/LXWingPlane.h"
#include "ObjectEx/LXObjectManager.h"
#include "UI/MapLayer.h"
#include "UI/SuspendLayer.h"
#include "UI/BattleLayer.h"
#include "Common/FileUtils.h"
#include "Common/LXUtils.h"
#include "DB/DBManager.h"
#include "Animation/AnimationManager.h"
#include "Animation/AnimationLogic.h"
#include "MainScene.h"
#include "Effect/ParticleEffect.h"
#include "Effect/LXEffectManager.h"
#include "Sound/LXSoundSystem.h"
#include "UI/PassCountLayer.h"
#include "Sound/LXSoundSystem.h"
#include "Common/LXUtils.h"
#include "TaskOption/TaskOptionMgr.h"
#include "SDK/SDKManager.h"
#include "UI/MyAnimation/NiceGirl.h"
#include "UI/RankUp.h"

GameScene::GameScene()
	: Layer()
{
	for(int i = 0; i < GAME_ZORDER_NUM; i++)
	{
		Layer *pLayer = Layer::create();
		this->addChild(pLayer, i);
		m_LayerList.push_back(pLayer);
	}

	//SoundSystem().preloadBackgroundMusic(Mic_bgm_boss);
	SoundSystem().preloadEffect(Mic_entry);
	SoundSystem().preloadEffect(Mic_levelmax1);
	SoundSystem().preloadEffect(Mic_levelmax2);
	SoundSystem().preloadEffect(Mic_bs1);
	SoundSystem().preloadEffect(Mic_bs2);
	SoundSystem().preloadEffect(Mic_powerup);
	SoundSystem().preloadEffect(Mic_shield);
	SoundSystem().preloadEffect(Mic_life);
	SoundSystem().preloadEffect(Mic_bomb);
	SoundSystem().preloadEffect(Mic_crystal);
	SoundSystem().preloadEffect(Mic_transform);
	SoundSystem().preloadEffect(Mic_boom_m);
	SoundSystem().preloadEffect(Mic_boom_s);
	SoundSystem().preloadEffect(Mic_bullet1);
	SoundSystem().preloadEffect(Mic_bullet2);
	SoundSystem().preloadEffect(Mic_warning);

	m_nMission = 0;

	m_IsPlayMissionStartStory = true;
	m_IsPlayMissionEndStory = true;
	
}

GameScene::~GameScene()
{
}


bool GameScene::init()
{
	// 1. super init first
	if ( !Layer::init() )
	{
		return false;
	}

	m_bGamePause = false;
	
	m_fPassTime = 0.0f;
	m_nBatchCount = 0;
	m_fDistance = 0.0f;	
	m_fPreRunTime = 3.0f;
	m_fAfterRunTime = 7.0f;
	
	return true;
}

void GameScene::InitGame(int planeID,bool IsBossMisson, int minssionID, int difficulty)
{
	m_nMission = minssionID;

	m_IsPlayMissionStartStory = true;
	m_IsPlayMissionEndStory = true;

	//初始化数值
	DBManager().InitMissonData();

	// 添加计时器调用
	this->schedule(schedule_selector(GameScene::Tick));

	// 添加触摸监听
	OpenListener();

	// 地图层
	int MapID[12] = {1,2,3,4,5,0,5,4,0,3,1,2};
	int nMapId = minssionID%6;
	if (minssionID <= 12 && minssionID >0)
	{
		nMapId = MapID[minssionID-1];
	}
	log("===MapLayer START===  ");
	m_pMapLayer = MapLayer::create();
	m_pMapLayer->SetMapId(nMapId); //地图编号0-6
	GetNodeByZOrder(GAME_ZORDER_MAP)->addChild(m_pMapLayer,GAME_ZORDER_MAP);
	log("===MapLayer END===  ");
	//暂停UI
	log("===START STOP UI===  ");
	m_pSuspendLayer = SuspendLayer::create();
	GetNodeByZOrder(GAME_ZORDER_SUSPEND)->addChild(m_pSuspendLayer,GAME_ZORDER_SUSPEND);
	m_pSuspendLayer->setVisible(false);
	log("===START STOP UI===  END");
	//战斗UI
	log("===START BATTLE UI===  ");
	m_pBattleLayer = BattleLayer::create();
	GetNodeByZOrder(GAME_ZORDER_BATTLE)->addChild(m_pBattleLayer,GAME_ZORDER_BATTLE);
	log("===START BATTLE UI===  END");
	// 通关奖励 （ 与暂停同一个层 ）
	m_pPassCountLayer = PassCountLayer::create();
	GetNodeByZOrder(GAME_ZORDER_SUSPEND)->addChild(m_pPassCountLayer,GAME_ZORDER_SUSPEND);
	m_pPassCountLayer->setVisible(false);

	// 排名上升界面 （ 与暂停同一个层 ）
	m_pRankUpLayer = RankUp::create();
	GetNodeByZOrder(GAME_ZORDER_SUSPEND)->addChild(m_pRankUpLayer,GAME_ZORDER_SUSPEND);
	m_pRankUpLayer->setVisible(false);

	//加载敌机批次信息
	InitBatchInfo(IsBossMisson, minssionID, difficulty);

	// 加载玩家
	// 玩家 
	m_pPlayer = dynamic_cast<LXPlayerPlane*>(ObjectManager().CreateObject(planeID, GetNodeByZOrder(GAME_ZORDER_PLANE), object_type_plane_player, true));
	if (m_pPlayer)
	{
		// 初始化
		m_pPlayer->Init(planeID);

		// 设置位置
		m_pPlayer->setPosition( Utils().GetFixedX(PLANE_START_X), PLANE_START_Y);

		// 增加引用
		ObjectManager().RigisterPlayer(m_pPlayer);
	}

	// 加载僚机
	int nWingNum = DBManager().GetWingNum();
	for (int i = 0; i < nWingNum; i++)
	{
		// 根据战机类型计算僚机类型
		int nResID = 0;
		if(m_pPlayer->GetResID() == player_plane_type_zuheji)
		{
			nResID = i / 2;
		}
		else
		{
			nResID = m_pPlayer->GetResID();
		}

		LXWingPlane *pWing = dynamic_cast<LXWingPlane*>(ObjectManager().CreateObject(nResID, GetNodeByZOrder(GAME_ZORDER_PLANE), object_type_plane_wing, true));
		if (pWing)
		{
			bool bValue = false;
			if ((nResID == 0 || nResID == 4) && (i % 2 == 0))
			{
				bValue = true;
			}

			pWing->Init(nResID, bValue);

			// 位置
			float xPos = m_pPlayer->getPosition().x + g_WingXPos[i];
			float yPos = m_pPlayer->getPosition().y - g_WingYPos[i];
			pWing->setPosition(ccp(xPos, yPos));
			// 添加僚机引用
			m_pPlayer->AddWingPlane(pWing);
		}
	}
	
	//m_pPlayer->InitShootLogic();
	m_pPlayer->RunIntoScreen();
	m_pPlayer->SetPause(false);

	/*std::stringstream ss;
	ss << m_nMission;
	SDKManager().startLevel(ss.str().c_str());*/


	// 停止战斗界面的点击事件
	StopClickEventWithBattleLayerButton();

}

void GameScene::CleanGame()
{
	//删除计时器调用
	this->unschedule(schedule_selector(GameScene::Tick));
	//关闭触摸监听
	CloseListener();
}

void GameScene::Tick(float dt)
{
	if (m_bGamePause)
	{
		return;
	}

	//
	if (m_fPreRunTime > 0)
	{
		m_fPreRunTime -= dt;
		if (m_fPreRunTime <= 0)
		{

			StartClickEventWithBattleLayerButton();

			//misson特效
			std::string	strName = "p0/mission.p";
			EffectManager().CreateEffect(GetNodeByZOrder(GAME_ZORDER_OBJECT_PARTICLE), strName, Utils().GetFixedX(240), Utils().GetFixedY(400), GAME_ZORDER_OBJECT_PARTICLE, false);

			//关卡数字
			int nMisson = static_cast<MainScene*>(this->getParent())->GetMisson();
			TextAtlas * pMisson = TextAtlas::create(StringUtils::toString(nMisson),"Texture/Res/ui/TextAtlas/a27.png",29,50,"0");
			pMisson->setScale(0.2);
			//pMisson->setColor(Color3B(1,1,1));
			pMisson->setPosition(Utils().GetFixedPosion(Utils().GetFixedX(340),Utils().GetFixedY(400+150)));
			//动作
			ActionInterval* pDelay = DelayTime::create(0.1);
			ActionInterval* pMoveby = MoveTo::create(0.5,Vec2(Utils().GetFixedX(340),Utils().GetFixedY(400)));
			ActionInterval* pFadeIn = FadeIn::create(0.5);
			ActionInterval* pScaleto = ScaleTo::create(0.5,1);
			ActionInterval* pSpawn = Spawn::create(pMoveby,pFadeIn,pScaleto,nullptr);
			ActionInterval* pDelay2 = DelayTime::create(1.1);
			ActionInterval* pMoveBy2 = MoveBy::create(0.5,Vec2(300,0));
			ActionInterval* pSeq = Sequence::create(pDelay,pSpawn,pDelay2,pMoveBy2,nullptr);
			GetNodeByZOrder(GAME_ZORDER_OBJECT_PARTICLE)->addChild(pMisson,GAME_ZORDER_OBJECT_PARTICLE);
			pMisson->runAction(pSeq);

			SoundSystem().playEffect(Mic_mission_start);
			
		}
		
		if( m_fPreRunTime <= 0.1 )
		{
			int nIdx = DBManager().GetCurrMission();
			if ( nIdx < 2 )
			{
				// 新手教学
				if ( DBManager().GetData( DATA_FRESH_MAN ) < 1 )
				{
					if ( NiceGirl::GetFreshManCurrStep() == 4 )
					{
						NiceGirl::SetFreshManCurrStep( 5 );

						dynamic_cast<MainScene*>(this->getParent())->ShowNewPlayer();

					}
				}

			}else
			{
				if ( DBManager().IsBossMission() == false )
				{
					// 关卡剧情
					if ( m_IsPlayMissionStartStory )
					{
						dynamic_cast<MainScene*>(this->getParent())->ShowMissionStartStory(nIdx);

						m_IsPlayMissionStartStory = false;
					}
				}

			}
		}
	}
	else
	{
		//敌机批次逻辑
		EnemyBatchTick(dt);
	}
	

	//m_pTestLogic->Tick(dt);

	m_pMapLayer->Tick(dt);

	m_pBattleLayer->Tick(dt);

	m_pPassCountLayer->Tick(dt);
}

void GameScene::OpenListener()
{
	// 创建监听
	auto listener = EventListenerTouchOneByOne::create();//创建一个触摸监听(单点触摸）  
	listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);//指定触摸的回调函数  
	listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
	listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);  
	listener->onTouchCancelled = CC_CALLBACK_2(GameScene::onTouchCancelled, this);  
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);//将listener放入事件委托中  

}

void GameScene::CloseListener()
{
	_eventDispatcher->removeEventListenersForTarget(this);
}


bool GameScene::onTouchBegan(Touch *touch, Event *unused_event)
{
	//int x = touch->getLocation().x;
	//int y = touch->getLocation().y;

	return true;
}

void GameScene::onTouchMoved(Touch *touch, Event *unused_event)
{	
	// 获取移动偏移量
	if (m_bGamePause)
	{
		return;
	}

	Vec2 pos = touch->getDelta();
	MovePlane(pos);
	//m_pPlayerLayer->MovePlane(pos);

	
}

void GameScene::onTouchEnded(Touch *touch, Event *unused_event)
{

}

void GameScene::onTouchCancelled(Touch *touch, Event *unused_event)
{
	
}

void GameScene::GamePause()
{
	m_vPauseNodes = Director::getInstance()->getActionManager()->pauseAllRunningActions();
	m_pSuspendLayer->Init();
	if (ObjectManager().GetPlayer())
	{
		ObjectManager().GetPlayer()->SetPause(true);
	}
	m_bGamePause = true;
}

void GameScene::GameContinue()
{
	Director::getInstance()->getActionManager()->resumeTargets(m_vPauseNodes);
	if (ObjectManager().GetPlayer())
	{
		ObjectManager().GetPlayer()->SetPause(false);
	}
	m_bGamePause = false;
}

void GameScene::GamePauseAndNoSuspendLayer()
{
	m_vPauseNodes = Director::getInstance()->getActionManager()->pauseAllRunningActions();
	if (ObjectManager().GetPlayer())
	{
		ObjectManager().GetPlayer()->SetPause(true);
	}
	m_bGamePause = true;
}

void GameScene::GameContinueAndNoSuspendLayer()
{
	Director::getInstance()->getActionManager()->resumeTargets(m_vPauseNodes);
	if (ObjectManager().GetPlayer())
	{
		ObjectManager().GetPlayer()->SetPause(false);
	}
	m_bGamePause = false;
}


void GameScene::GameGiveUp()
{
	ObjectManager().CleanAllObject();
	m_pSuspendLayer->setVisible(false);
	m_pBattleLayer->setVisible(false);
	m_pPassCountLayer->setVisible(false);
	if (ObjectManager().GetPlayer())
	{
		ObjectManager().GetPlayer()->SetPause(true);
	}
	static_cast<MainScene*>(this->getParent())->LeaveGameSence();
	Director::getInstance()->getActionManager()->resumeTargets(m_vPauseNodes);
}

void GameScene::GameGoToSelectMission()
{
	ObjectManager().CleanAllObject();
	m_pSuspendLayer->setVisible(false);
	m_pBattleLayer->setVisible(false);
	m_pPassCountLayer->setVisible(false);
	static_cast<MainScene*>(this->getParent())->LeaveGameSence( INTERFACE_ID::INTERFACE_MISSION );
}

void GameScene::GameGoToIntensify()
{
	ObjectManager().CleanAllObject();
	m_pSuspendLayer->setVisible(false);
	m_pBattleLayer->setVisible(false);
	m_pPassCountLayer->setVisible(false);
	static_cast<MainScene*>(this->getParent())->LeaveGameSence( INTERFACE_ID::INTERFACE_INTENSIFY );
}

void GameScene::InitBatchInfo(bool IsBossMisson, int minssionID, int difficulty)
{
	if (difficulty<0 || difficulty>2)
	{
		return;
	}

	string difficultArray[3] = {"easy","normal","hard"};

	m_vEnemyInfo.clear();
	m_vBatchInfo.clear();

	//补0
	string strMission = StringUtils::toString(minssionID);// to_string(minssionID);
	if (minssionID < 10)
	{
		strMission = "0" + strMission;
	}

	std::string fullpath;
	if (IsBossMisson)
	{
		fullpath = strMission + difficultArray[difficulty] + ".map";
	}
	else
	{
		fullpath = strMission + ".map";
	}
	LXFileUtils::GetMapFullPath(fullpath);
	int count = LoadEnemyBatchInfo(fullpath);

	
	LXFileUtils::AddSpriteFrameCache("diji02.plist", "diji02.png");
	LXFileUtils::AddSpriteFrameCache("ui/play.plist","ui/play.png");

	// 预加载场景所需要的所有资源
	InitGameObject();
}

void GameScene::InitGameObject()
{
	if (m_nMission == 1)
	{
		// 对象初始化
		for(int i = 0; i < 22; i++)
		{
			std::string enemyInfo = g_PreEnemy1[i];
			std::vector<std::string> vec;
			Utils().split(enemyInfo, vec, ",");

			if (vec.size() == 2)
			{
				ObjectManager().PreCreateObject(atoi(vec[0].c_str()), this, object_type_plane_enemy, false, atoi(vec[1].c_str()));
			}
		}

		ObjectManager().PreCreateObject(g_PreBoss[m_nMission-1], this, object_type_plane_boss, false, 1);

		// 弹幕初始化
		for(int i = 0; i < 11; i++)
		{
			std::string weaponInfo = g_PreEnemyWeapon1[i];
			std::vector<std::string> vec;
			Utils().split(weaponInfo, vec, ",");

			if (vec.size() == 2)
			{
				ObjectManager().PreCreateObject(atoi(vec[0].c_str()), this, object_type_weapon_common, false, atoi(vec[1].c_str()));
			}
		}
	}
	else if (m_nMission == 2)
	{
		// 对象初始化
		for(int i = 0; i < 17; i++)
		{
			std::string enemyInfo = g_PreEnemy2[i];
			std::vector<std::string> vec;
			Utils().split(enemyInfo, vec, ",");

			if (vec.size() == 2)
			{
				ObjectManager().PreCreateObject(atoi(vec[0].c_str()), this, object_type_plane_enemy, false, atoi(vec[1].c_str()));
			}
		}

		ObjectManager().PreCreateObject(g_PreBoss[m_nMission-1], this, object_type_plane_boss, false, 1);

		// 弹幕初始化
		for(int i = 0; i < 7; i++)
		{
			std::string weaponInfo = g_PreEnemyWeapon2[i];
			std::vector<std::string> vec;
			Utils().split(weaponInfo, vec, ",");

			if (vec.size() == 2)
			{
				ObjectManager().PreCreateObject(atoi(vec[0].c_str()), this, object_type_weapon_common, false, atoi(vec[1].c_str()));
			}
		}
	}
	else if (m_nMission == 3)
	{
		// 对象初始化
		for(int i = 0; i < 26; i++)
		{
			std::string enemyInfo = g_PreEnemy3[i];
			std::vector<std::string> vec;
			Utils().split(enemyInfo, vec, ",");

			if (vec.size() == 2)
			{
				ObjectManager().PreCreateObject(atoi(vec[0].c_str()), this, object_type_plane_enemy, false, atoi(vec[1].c_str()));
			}
		}

		ObjectManager().PreCreateObject(g_PreBoss[m_nMission-1], this, object_type_plane_boss, false, 1);

		// 弹幕初始化
		for(int i = 0; i < 7; i++)
		{
			std::string weaponInfo = g_PreEnemyWeapon3[i];
			std::vector<std::string> vec;
			Utils().split(weaponInfo, vec, ",");

			if (vec.size() == 2)
			{
				ObjectManager().PreCreateObject(atoi(vec[0].c_str()), this, object_type_weapon_common, false, atoi(vec[1].c_str()));
			}
		}
	}
	else if (m_nMission == 4)
	{
		// 对象初始化
		for(int i = 0; i < 20; i++)
		{
			std::string enemyInfo = g_PreEnemy4[i];
			std::vector<std::string> vec;
			Utils().split(enemyInfo, vec, ",");

			if (vec.size() == 2)
			{
				ObjectManager().PreCreateObject(atoi(vec[0].c_str()), this, object_type_plane_enemy, false, atoi(vec[1].c_str()));
			}
		}

		ObjectManager().PreCreateObject(g_PreBoss[m_nMission-1], this, object_type_plane_boss, false, 1);

		// 弹幕初始化
		for(int i = 0; i < 7; i++)
		{
			std::string weaponInfo = g_PreEnemyWeapon4[i];
			std::vector<std::string> vec;
			Utils().split(weaponInfo, vec, ",");

			if (vec.size() == 2)
			{
				ObjectManager().PreCreateObject(atoi(vec[0].c_str()), this, object_type_weapon_common, false, atoi(vec[1].c_str()));
			}
		}
	}
	else if (m_nMission == 5)
	{
		// 对象初始化
		for(int i = 0; i < 25; i++)
		{
			std::string enemyInfo = g_PreEnemy5[i];
			std::vector<std::string> vec;
			Utils().split(enemyInfo, vec, ",");

			if (vec.size() == 2)
			{
				ObjectManager().PreCreateObject(atoi(vec[0].c_str()), this, object_type_plane_enemy, false, atoi(vec[1].c_str()));
			}
		}

		ObjectManager().PreCreateObject(g_PreBoss[m_nMission-1], this, object_type_plane_boss, false, 1);

		// 弹幕初始化
		for(int i = 0; i < 8; i++)
		{
			std::string weaponInfo = g_PreEnemyWeapon5[i];
			std::vector<std::string> vec;
			Utils().split(weaponInfo, vec, ",");

			if (vec.size() == 2)
			{
				ObjectManager().PreCreateObject(atoi(vec[0].c_str()), this, object_type_weapon_common, false, atoi(vec[1].c_str()));
			}
		}
	}
	else if (m_nMission == 6)
	{
		// 对象初始化
		for(int i = 0; i < 17; i++)
		{
			std::string enemyInfo = g_PreEnemy6[i];
			std::vector<std::string> vec;
			Utils().split(enemyInfo, vec, ",");

			if (vec.size() == 2)
			{
				ObjectManager().PreCreateObject(atoi(vec[0].c_str()), this, object_type_plane_enemy, false, atoi(vec[1].c_str()));
			}
		}

		ObjectManager().PreCreateObject(g_PreBoss[m_nMission-1], this, object_type_plane_boss, false, 1);

		// 弹幕初始化
		for(int i = 0; i < 10; i++)
		{
			std::string weaponInfo = g_PreEnemyWeapon6[i];
			std::vector<std::string> vec;
			Utils().split(weaponInfo, vec, ",");

			if (vec.size() == 2)
			{
				ObjectManager().PreCreateObject(atoi(vec[0].c_str()), this, object_type_weapon_common, false, atoi(vec[1].c_str()));
			}
		}
	}
	else if (m_nMission == 7)
	{
		// 对象初始化
		for(int i = 0; i < 22; i++)
		{
			std::string enemyInfo = g_PreEnemy7[i];
			std::vector<std::string> vec;
			Utils().split(enemyInfo, vec, ",");

			if (vec.size() == 2)
			{
				ObjectManager().PreCreateObject(atoi(vec[0].c_str()), this, object_type_plane_enemy, false, atoi(vec[1].c_str()));
			}
		}

		ObjectManager().PreCreateObject(g_PreBoss[m_nMission-1], this, object_type_plane_boss, false, 1);

		// 弹幕初始化
		for(int i = 0; i < 10; i++)
		{
			std::string weaponInfo = g_PreEnemyWeapon7[i];
			std::vector<std::string> vec;
			Utils().split(weaponInfo, vec, ",");

			if (vec.size() == 2)
			{
				ObjectManager().PreCreateObject(atoi(vec[0].c_str()), this, object_type_weapon_common, false, atoi(vec[1].c_str()));
			}
		}
	}
	else if (m_nMission == 8)
	{
		// 对象初始化
		for(int i = 0; i < 28; i++)
		{
			std::string enemyInfo = g_PreEnemy8[i];
			std::vector<std::string> vec;
			Utils().split(enemyInfo, vec, ",");

			if (vec.size() == 2)
			{
				ObjectManager().PreCreateObject(atoi(vec[0].c_str()), this, object_type_plane_enemy, false, atoi(vec[1].c_str()));
			}
		}

		ObjectManager().PreCreateObject(g_PreBoss[m_nMission-1], this, object_type_plane_boss, false, 1);

		// 弹幕初始化
		for(int i = 0; i < 10; i++)
		{
			std::string weaponInfo = g_PreEnemyWeapon8[i];
			std::vector<std::string> vec;
			Utils().split(weaponInfo, vec, ",");

			if (vec.size() == 2)
			{
				ObjectManager().PreCreateObject(atoi(vec[0].c_str()), this, object_type_weapon_common, false, atoi(vec[1].c_str()));
			}
		}
	}
	else if (m_nMission == 9)
	{
		// 对象初始化
		for(int i = 0; i < 18; i++)
		{
			std::string enemyInfo = g_PreEnemy9[i];
			std::vector<std::string> vec;
			Utils().split(enemyInfo, vec, ",");

			if (vec.size() == 2)
			{
				ObjectManager().PreCreateObject(atoi(vec[0].c_str()), this, object_type_plane_enemy, false, atoi(vec[1].c_str()));
			}
		}

		ObjectManager().PreCreateObject(g_PreBoss[m_nMission-1], this, object_type_plane_boss, false, 1);

		// 弹幕初始化
		for(int i = 0; i < 8; i++)
		{
			std::string weaponInfo = g_PreEnemyWeapon9[i];
			std::vector<std::string> vec;
			Utils().split(weaponInfo, vec, ",");

			if (vec.size() == 2)
			{
				ObjectManager().PreCreateObject(atoi(vec[0].c_str()), this, object_type_weapon_common, false, atoi(vec[1].c_str()));
			}
		}
	}
	else if (m_nMission == 10)
	{
		// 对象初始化
		for(int i = 0; i < 20; i++)
		{
			std::string enemyInfo = g_PreEnemy10[i];
			std::vector<std::string> vec;
			Utils().split(enemyInfo, vec, ",");

			if (vec.size() == 2)
			{
				ObjectManager().PreCreateObject(atoi(vec[0].c_str()), this, object_type_plane_enemy, false, atoi(vec[1].c_str()));
			}
		}

		ObjectManager().PreCreateObject(g_PreBoss[m_nMission-1], this, object_type_plane_boss, false, 1);

		// 弹幕初始化
		for(int i = 0; i < 8; i++)
		{
			std::string weaponInfo = g_PreEnemyWeapon10[i];
			std::vector<std::string> vec;
			Utils().split(weaponInfo, vec, ",");

			if (vec.size() == 2)
			{
				ObjectManager().PreCreateObject(atoi(vec[0].c_str()), this, object_type_weapon_common, false, atoi(vec[1].c_str()));
			}
		}
	}
	else if (m_nMission == 11)
	{
		// 对象初始化
		for(int i = 0; i < 12; i++)
		{
			std::string enemyInfo = g_PreEnemy11[i];
			std::vector<std::string> vec;
			Utils().split(enemyInfo, vec, ",");

			if (vec.size() == 2)
			{
				ObjectManager().PreCreateObject(atoi(vec[0].c_str()), this, object_type_plane_enemy, false, atoi(vec[1].c_str()));
			}
		}

		ObjectManager().PreCreateObject(g_PreBoss[m_nMission-1], this, object_type_plane_boss, false, 1);

		// 弹幕初始化
		for(int i = 0; i < 5; i++)
		{
			std::string weaponInfo = g_PreEnemyWeapon11[i];
			std::vector<std::string> vec;
			Utils().split(weaponInfo, vec, ",");

			if (vec.size() == 2)
			{
				ObjectManager().PreCreateObject(atoi(vec[0].c_str()), this, object_type_weapon_common, false, atoi(vec[1].c_str()));
			}
		}
	}
	else if (m_nMission == 12)
	{
		// 对象初始化
		for(int i = 0; i < 7; i++)
		{
			std::string enemyInfo = g_PreEnemy12[i];
			std::vector<std::string> vec;
			Utils().split(enemyInfo, vec, ",");

			if (vec.size() == 2)
			{
				ObjectManager().PreCreateObject(atoi(vec[0].c_str()), this, object_type_plane_enemy, false, atoi(vec[1].c_str()));
			}
		}

		ObjectManager().PreCreateObject(22, this, object_type_plane_boss, false, 1);
		ObjectManager().PreCreateObject(115, this, object_type_plane_boss, false, 1);
		ObjectManager().PreCreateObject(116, this, object_type_plane_boss, false, 1);
		ObjectManager().PreCreateObject(118, this, object_type_plane_boss, false, 1);
		ObjectManager().PreCreateObject(260, this, object_type_plane_boss, false, 1);
		ObjectManager().PreCreateObject(509, this, object_type_plane_boss, false, 1);

		// 弹幕初始化
		for(int i = 0; i < 11; i++)
		{
			std::string weaponInfo = g_PreEnemyWeapon12[i];
			std::vector<std::string> vec;
			Utils().split(weaponInfo, vec, ",");

			if (vec.size() == 2)
			{
				ObjectManager().PreCreateObject(atoi(vec[0].c_str()), this, object_type_weapon_common, false, atoi(vec[1].c_str()));
			}
		}
	}
}

void GameScene::EnemyBatchTick(float dt)
{
	//波次信息加载
	bool bTimePause = false;
	float fDistance = m_fPassTime * 60; //当前移动距离
	while (true)
	{
		//判断批次是否存在,距离是否达到
		if (m_nBatchCount >= m_vBatchInfo.size() || m_vBatchInfo[m_nBatchCount].pos > fDistance)
		{
			bool bTimePause = false;
			break;
		}

		//判断是否是等待前面的敌机全部死亡
		BatchInfo info =  m_vBatchInfo[m_nBatchCount];
		if (info.waitPrev && (ObjectManager().GetEnemyNum() > 0 || m_vEnemyInfo.size() > 0))
		{
			bool bTimePause = true; //飞机逻辑时间停止
			break;
		}

		//添加敌机创建信息
		for (int i = 0; i < info.num; i++)
		{
			EnemyInfo enemy;

			enemy.model = info.model;
			enemy.createTime = m_fPassTime + info.delay + info.waitTime * i;
			enemy.posX = info.x + info.offX * i;
			enemy.posY = info.y + info.offY * i;
			enemy.action = info.action;

			m_vEnemyInfo.push_back(enemy);

			//获取飞机类型22
			DBC_DEFINEHANDLE(s_pEnemyDB, DBC_ENEMY);
			auto pEnemyData = (const _DBC_ENEMY*)s_pEnemyDB->Search_LineNum_EQU(enemy.model);
			if (pEnemyData->nType == 1)
			{
				//显示boss出场动画
				m_pBattleLayer->ShowBloodBar(true);
				std::string strName = "p0/ui_warning.p";
				EffectManager().CreateEffect(GetNodeByZOrder(GAME_ZORDER_OBJECT_PARTICLE), strName, Utils().GetFixedX(240),Utils().GetFixedY(400) , GAME_ZORDER_OBJECT_PARTICLE, false);
				SoundSystem().playEffect(SoundName::Mic_warning);
				SoundSystem().playBackgroundMusic(Mic_bgm_boss);
				if (ObjectManager().GetPlayer())
				{
					dynamic_cast<LXPlayerPlane*>(ObjectManager().GetPlayer())->SetFireState(false);
				}
			}
		}

		m_nBatchCount++;
		
		/*---------------------------------------------------------------------*/
		// 新手教学相关
		if ( DBManager().GetData( DATA_FRESH_MAN ) < 1 )
		{
			if ( m_nBatchCount == 54 )
			{
				if ( NiceGirl::GetFreshManCurrStep() == 6 )
				{
					ActionInterval* _delayTime = DelayTime::create( 3.2f );

					CallFunc* pCallFunc = CallFunc::create( CC_CALLBACK_0(GameScene::DelayTimeCallback,this) );

					ActionInterval* _Sequence = Sequence::create( _delayTime,pCallFunc,NULL );

					this->runAction( _Sequence );
				}

			}else if( m_nBatchCount == 63  )
			{
				if ( NiceGirl::GetFreshManCurrStep() == 9 )
				{
					// 为了有延迟 做的
					ActionInterval* _delayTime = DelayTime::create( 3.5f );

					CallFunc* pCallFunc = CallFunc::create( CC_CALLBACK_0(GameScene::DelayTimeCallback,this) );

					ActionInterval* _Sequence = Sequence::create( _delayTime,pCallFunc,NULL );

					this->runAction( _Sequence );

				}
			}
		}

	}

	if (!bTimePause)
	{
		m_fPassTime += dt;
	}

	const auto screenSize = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();
	//实际添加飞机
	std::vector<EnemyInfo>::iterator it = m_vEnemyInfo.begin();
	while (it != m_vEnemyInfo.end())
	{
		if ((*it).createTime <= m_fPassTime )
		{
			// 加载怪物信息
			DBC_DEFINEHANDLE(s_pEnemyDBC, DBC_ENEMY);
			const _DBC_ENEMY	*m_pEnemyData = (const _DBC_ENEMY*)s_pEnemyDBC->Search_LineNum_EQU((*it).model);

			//判断飞机类型
			LXObject* pObject = nullptr;
		
			if (m_pEnemyData->nType == enemy_type_monster)
			{	
				pObject = ObjectManager().CreateObject((*it).model, GetNodeByZOrder(GAME_ZORDER_ENEMY), object_type_plane_enemy, false);
			}
			else if (m_pEnemyData->nType == enemy_type_boss)
			{
				pObject = ObjectManager().CreateObject((*it).model, GetNodeByZOrder(GAME_ZORDER_ENEMY), object_type_plane_boss, false);
			}

			if (pObject)
			{
				pObject->Init((*it).model);
				pObject->InitAction((*it).action, ai_tag_common);
				pObject->setPosition( Utils().GetFixedPosion((*it).posX, screenSize.height - (*it).posY));
			}

			it = m_vEnemyInfo.erase(it);
		}
		else
		{
			it++;
		}
	}

	//关卡结束
	if (m_nBatchCount >= m_vBatchInfo.size() && ObjectManager().GetEnemyNum() == 0 && m_vEnemyInfo.size() == 0)
	{
		//关卡结束
		if (m_fAfterRunTime > 0)
		{
			m_fAfterRunTime -= dt;
			if (m_fAfterRunTime <= 0)
			{
				//播放关卡结束动画
				std::string strName =  "p0/victory.p";
				EffectManager().CreateEffect(GetNodeByZOrder(GAME_ZORDER_OBJECT_PARTICLE), strName, Utils().GetFixedX(240), Utils().GetFixedY(400), GAME_ZORDER_OBJECT_PARTICLE, false);
				SoundSystem().playEffect(SoundName::Mic_mission_complete);
				m_pPlayer->FighterWin();

				// 停止Battlelayer的点击
				StopClickEventWithBattleLayerButton();

				/*---------------------------------------------------------------------*/
				
				int nIdx = DBManager().GetCurrMission();
				if ( nIdx < 2 )
				{
					// 新手教学相关
					if ( DBManager().GetData( DATA_FRESH_MAN ) < 1 )
					{
						if ( NiceGirl::GetFreshManCurrStep() ==  12 )
						{
							NiceGirl::SetFreshManCurrStep( 13 );

							dynamic_cast<MainScene*>(this->getParent())->ShowNewPlayer();

						}
					}

				}else
				{
					if ( DBManager().IsBossMission() == false )
					{
						// 关卡剧情
						if ( m_IsPlayMissionEndStory )
						{
							dynamic_cast<MainScene*>(this->getParent())->ShowMissionEndStory(nIdx);

							m_IsPlayMissionEndStory = false;
						}
					}
					
				}
			}
		}
		else
		{
			if (m_fAfterRunTime > -3)
			{
				m_fAfterRunTime -= dt;
				if (m_fAfterRunTime<= -3)
				{
					//弹出战斗结果界面
					//隐藏其他界面


					if (DBManager().IsBossMission() == false)
					{
						if ( DBManager().GetData(DATA_NEXT_MISSION) == DBManager().GetCurrMission() )
						{
							switch ( DBManager().GetCurrMission() )
							{
							case 1:
								{
									// 如果没有激活游戏。就让激活
									if ( DBManager().GetData(DATA_ACTIVATION_TYPE) <= 0 )
									{
										//DBManager().SetCurrPlaneID(1);
										dynamic_cast<MainScene*>( this->getParent() )->SetPlaneID(1);
									}

									TaskMgr().DoneTaskOption(1);
									TaskMgr().DoneTaskOption(4);
								}
								break;
							case 2:
								{
									TaskMgr().DoneTaskOption(2);
								}
								break;
							case 6:
								{
									TaskMgr().DoneTaskOption(3);
								}
								break;
							default:
								break;
							}

							DBManager().NextMissonRise();


							// 多送点宝石
							DBC_DEFINEHANDLE(s_pParamDBC, DBC_PARAM_INFO);
							const _DBC_PARAM_INFO *pParamInfo = (const _DBC_PARAM_INFO*)s_pParamDBC->Search_Index_EQU( 22 + DBManager().GetCurrMission() );
							if ( pParamInfo )
							{
								DBManager().AddMissionDiamon( pParamInfo->getIntValue() );
							}

						}
					}
					else
					{
						if ( DBManager().GetData(DATA_NEXT_BOSS) == DBManager().GetCurrMission() )
						{
							switch ( DBManager().GetCurrMission() )
							{
							case 6:
								{
									TaskMgr().DoneTaskOption(25);
								}
								break;
							default:
								break;
							}
							
							DBManager().NextBossMissonRise();
						}
					}

					int nScoreNum = DBManager().GetMissionScore();
					int nLastScoreNum = DBManager().GetData(DATA_TOP_SCORE);

					DBManager().SaveMissonDataToDBData();
					dynamic_cast<MainScene*>(getParent())->SetMissionState(true);
					
					if ( nScoreNum > nLastScoreNum )
					{
						OpenRankUpLayer();

						if ( DBManager().GetData( DATA_FRESH_MAN ) < 1 )
						{
							int _StepNum = NiceGirl::GetFreshManCurrStep();
							DBManager().SetData( DATA_FRESH_STEP, _StepNum );
						}

					}else
					{
						ShowPassCountInfo();
					}
				}
			}
		}
	}
}

int  GameScene::LoadEnemyBatchInfo(const std::string& fileName)
{
	//文件是否存在
	std::istringstream ifs(FileUtils::getInstance()->getStringFromFile(fileName));
	if (!ifs)
	{
		log("Cannot open file [%s]",fileName.c_str());
		return -1;
	}

	int maxchars = 8192;  // Alloc enough size.
	std::vector<char> buf(maxchars);  // Alloc enough size.

	BatchInfo info;
	bool	IsReadingState = false; //是否在读取状态
	while (ifs.peek() != -1)
	{
		ifs.getline(&buf[0], maxchars);

		std::string linebuf(&buf[0]);

		// Trim newline '\r\n' or '\r'
		if (linebuf.size() > 0)
		{
			if (linebuf[linebuf.size()-1] == '\n') linebuf.erase(linebuf.size()-1);
		}
		if (linebuf.size() > 0)
		{
			if (linebuf[linebuf.size()-1] == '\r') linebuf.erase(linebuf.size()-1);
		}

		// Skip if empty line.
		if (linebuf.empty())
		{
			continue;
		}

		// 空行
		if (linebuf.at(0) == '\0')
		{
			continue;
		}

		// 注释行
		if (linebuf.at(0) == '[')
		{
			continue;
		}

		// 结束
		int pos1=linebuf.find("- end -");
		if (pos1 >=0)
		{
			//模块结束
			if (IsReadingState)
			{
				m_vBatchInfo.push_back(info);
			}

			IsReadingState = false;
			continue;
		}

		// 
		int  pos = linebuf.find(' = ');
		if (pos <= 0)
		{
			continue;
		}

		std::string name = linebuf.substr(0,pos);
		std::string val = linebuf.substr(pos+3);
		if (name == "pos")
		{
			//模块开始
			info.Clean();
			IsReadingState = true;

			info.pos = std::atoi(val.c_str());
			continue;
		}

		if (IsReadingState) //模块开启标识
		{
			if (name == "x")
			{
				info.x = std::atoi(val.c_str());
				continue;
			}

			if (name == "y")
			{
				info.y = std::atoi(val.c_str());
				continue;
			}

			if (name == "model")
			{
				info.model = std::atoi(val.c_str());
				continue;
			}

			if (name == "num")
			{
				info.num = std::atoi(val.c_str());
				continue;
			}

			if (name == "waitPrev")
			{
				if (val == "true")
				{
					info.waitPrev = true;
				}
				else
				{
					info.waitPrev = false;
				}
				continue;
			}

			if (name == "delay")
			{
				info.delay = std::atof(val.c_str());
				continue;
			}

			if (name == "action")
			{
				info.action = val;
				continue;
			}

			if (name == "offX")
			{
				info.offX = std::atoi(val.c_str());
				continue;
			}

			if (name == "offY")
			{
				info.offY = std::atoi(val.c_str());
				continue;
			}

			if (name == "waitTime")
			{
				info.waitTime = std::atof(val.c_str());
				continue;
			}
		}

	}

	return m_vBatchInfo.size();
}

void GameScene::MovePlane(Vec2& deltaMove)
{
	PlaneMoveTo eType = PlaneMoveTo::NOT_MOVE;

	if ( deltaMove.x + 0.02f > 0 )
	{
		eType = PlaneMoveTo::RIGHT;

	}else if ( deltaMove.x + 0.02f < 0 )
	{
		eType = PlaneMoveTo::LEFT;

	}else
	{
		if( deltaMove.y + 0.02f > 0 )
		{
			eType = PlaneMoveTo::TOP;

		}else if ( deltaMove.y + 0.02f < 0 )
		{
			eType = PlaneMoveTo::BOTTOM;

		}else
		{
			eType = PlaneMoveTo::NOT_MOVE;
		}
	}


	// 计算飞机移动
	float m_nAimLengthX = deltaMove.x;
	float m_nAimLengthY = deltaMove.y;

	//Size visibleSize = Director::getInstance()->getVisibleSize();
	const Size visibleSize = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();
	float startX = 0;
	float endX = visibleSize.width;
	float startY = 0;
	float endY = visibleSize.height;
	if (visibleSize.width == 600)
	{
		startX = 30;
		endX = 570;
	}
	
	//空出飞机位置
	startX+=20;
	endX -=20;
	startY+=30;
	endY-=30;

	// 边界判断
	if ( m_pPlayer->getPositionX() + m_nAimLengthX > endX )
	{
		m_nAimLengthX = endX - m_pPlayer->getPositionX();

	}else if ( m_pPlayer->getPositionX() + m_nAimLengthX < startX )
	{
		m_nAimLengthX = startX - m_pPlayer->getPositionX();
	}

	if ( m_pPlayer->getPositionY() + m_nAimLengthY > endY )
	{
		m_nAimLengthY = endY - m_pPlayer->getPositionY();
	} 
	else if ( m_pPlayer->getPositionY() + m_nAimLengthY < startY )
	{
		m_nAimLengthY = startY - m_pPlayer->getPositionY();
	}

	m_pPlayer->Move(Vec2(m_nAimLengthX,m_nAimLengthY));
}

void	GameScene::SetBossBloodBar(float persent) 
{ 
	m_pBattleLayer->SetBloodPecent(persent); 
}

Node* GameScene::GetNodeByZOrder(int zorder)
{
	if (zorder >= 0 && zorder < GAME_ZORDER_NUM)
	{
		return m_LayerList[zorder];
	}

	return nullptr;
}

void GameScene::ClearPlayerPlane()
{
	if (m_pPlayer)
	{
		m_pPlayer->SetCleanUp(true);
		ObjectManager().RigisterPlayer(nullptr);
	}
}

void GameScene::PlayerPlaneRenascence()
{
	if( m_pPlayer )
	{
		DBManager().UseMissionLife();			// 使用一条生命
		m_pPlayer->RunIntoScreen();				// 重生
		m_pPlayer->RefreshProtectWithReLife();	// 重新计算护盾
	}
}

void GameScene::PlayerPlaneShieldEffect()
{
	if ( m_pPlayer )
	{
		m_pPlayer->CreatProtectEffect();
	}
}

void GameScene::DelayTimeCallback()
{
	if ( DBManager().GetData( DATA_FRESH_MAN ) >= 1 )
	{
		return ;
	}

	if ( NiceGirl::GetFreshManCurrStep() ==  6 )
	{
		NiceGirl::SetFreshManCurrStep( 7 );
		dynamic_cast<MainScene*>(this->getParent())->ShowNewPlayer();

	}else if ( NiceGirl::GetFreshManCurrStep() ==  9 )
	{
		NiceGirl::SetFreshManCurrStep( 10 );

		dynamic_cast<MainScene*>(this->getParent())->ShowNewPlayer();
	}
}

void GameScene::OpenRankUpLayer()
{
	if (m_pRankUpLayer)
	{
		m_pRankUpLayer->InitRankUpInfo();
	}
}

void GameScene::ShowPassCountInfo()
{
	if ( m_pPassCountLayer )
	{
		m_pPassCountLayer->Init( DBManager().GetMissionDiamond(), DBManager().GetData(DATA_KILLED_NUM), DBManager().GetData(DATA_TOP_SCORE) );
	}
}

void GameScene::StopClickEventWithBattleLayerButton()
{
	if (m_pBattleLayer)
	{
		m_pBattleLayer->StopClickEventWithButton();
	}
}

void GameScene::StartClickEventWithBattleLayerButton()
{
	if (m_pBattleLayer)
	{
		m_pBattleLayer->StartClickEventWithButton();
	}
}