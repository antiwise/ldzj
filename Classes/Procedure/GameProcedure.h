///****************************************\
//*										*
//* 			   游戏流程基类				*
//*										*
//\****************************************/
//
//#pragma once
//#include <vector>
//#include <string>
//#include "cocos2d.h"
//#include "GameDefine.h"
//#include "DataBase/DataBase.h"
//#include "UI/HelloWorldLayer.h"
//#include "Scene/LoadingScene.h"
//#include "Scene/GameScene.h"
//#include "Scene/MissionScene.h"
//
//USING_NS_CC;
//
//class CGameProcedure : public cocos2d::Node
//{
//public:
//	CGameProcedure();
//
//	void		Init(void);
//
//public:
//
//	// 场景跳转
//	void		changeScene(SCENE_ID scene_id);
//
//
//	//// 暂停游戏
//	//void		pause();
//
//protected:
//	void		ChangeLayerLogic(float dt);
//
//public:
//
//	// 数据库管理器
//	static CDataBaseSystem*			s_pDataBaseSystem;
//
//
//private:
//	
//	// 界面
//
//	//* 
//	HelloWorld*			m_pLoadingLayer;
//
//	//*/
//
//	//new
//
//	GameScene*			m_pGameScene;		//战斗场景
//	LoadingScene*		m_pLoadingScene;	//加载场景
//	MissionScene*		m_pMissionScene;	//关卡选择界面
//
//
//};
//
//CGameProcedure& GameProcedure();