///****************************************\
//*										*
//* 			   ��Ϸ���̻���				*
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
//	// ������ת
//	void		changeScene(SCENE_ID scene_id);
//
//
//	//// ��ͣ��Ϸ
//	//void		pause();
//
//protected:
//	void		ChangeLayerLogic(float dt);
//
//public:
//
//	// ���ݿ������
//	static CDataBaseSystem*			s_pDataBaseSystem;
//
//
//private:
//	
//	// ����
//
//	//* 
//	HelloWorld*			m_pLoadingLayer;
//
//	//*/
//
//	//new
//
//	GameScene*			m_pGameScene;		//ս������
//	LoadingScene*		m_pLoadingScene;	//���س���
//	MissionScene*		m_pMissionScene;	//�ؿ�ѡ�����
//
//
//};
//
//CGameProcedure& GameProcedure();