//#include "Gameprocedure.h"
//#include "Animation/AnimationManager.h"
//#include "Scene/GameScene.h"
//#include "Action/ActionManager.h"
//#include "DataBase/DataBase.h"
//#include "Shooter/LXShooterManager.h"
//
//CDataBaseSystem*		CGameProcedure::s_pDataBaseSystem	= nullptr;
//
//CGameProcedure& GameProcedure()
//{
//	static CGameProcedure s;
//	return s;
//}
//
//CGameProcedure::CGameProcedure()
//	: m_pLoadingLayer(NULL),
//	m_pGameScene(NULL),
//	m_pLoadingScene(NULL),
//	m_pMenuScene(NULL)
//{
//}
//
//void CGameProcedure::Init(void)
//{
//	ShooterManager().ParseShootersData();
//	AnimationManager().LoadAnimation("boss");
//	LXActionManager().LoadAction("eScript");
//	
//	//�������ݹ�����
//	s_pDataBaseSystem = new CDataBaseSystem();	
//	s_pDataBaseSystem->Initial(0);
//
//	// create a scene. it's an autorelease object
//	auto scene = Scene::create();
//	
//	//m_pLoadingLayer = HelloWorld::create();
//	//scene->addChild(m_pLoadingLayer);
//
//	//loading����
//
//	DBC_DEFINEHANDLE(s_pEnemyDBC, DBC_UPGRADE);
//	auto m_pEnemyData = (const _DBC_UPGRADE*)s_pEnemyDBC->Search_LineNum_EQU(1);
//
//	DBC_DEFINEHANDLE(s_pEnemyDBC1, DBC_ACHIEVE);
//	auto m_pEnemyData1 = (const _DBC_ACHIEVE*)s_pEnemyDBC1->Search_LineNum_EQU(1);
//
//	DBC_DEFINEHANDLE(s_pEnemyDBC2, DBC_BULLET);
//	auto m_pEnemyData2 = (const _DBC_BULLET*)s_pEnemyDBC2->Search_LineNum_EQU(1);
//	
//	// �ؿ�ѡ�񳡾�
//	//m_pMissionScene = MissionScene::create();
//	//scene->addChild(m_pMissionScene);
//
//	// ������
//	m_pMenuScene = MenuScene::create();
//	scene->addChild(m_pMenuScene);
//
//	////ս������
//	//m_pGameScene = GameScene::create();
//	//scene->addChild(m_pGameScene);
//	//m_pGameScene->setVisible(false);
//
//	// run
//	auto director = Director::getInstance();
//	director->runWithScene(scene);
//
//	///*---------------------------------------------------------------------*/
//
//
//}
//
//
//void CGameProcedure::changeScene(SCENE_ID scene_id)
//{
//	m_pLoadingLayer->setVisible(false);
//
//	//	m_NextProc = proc;
//	//	scheduleOnce(schedule_selector(CGameProcedure::ChangeLayerLogic), 0.01);
//}
