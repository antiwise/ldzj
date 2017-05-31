//#pragma once
//
//#include "cocos2d.h"
//#include "ui/CocosGUI.h"
//#include "cocostudio/CocoStudio.h"
//#include "Object/MyPlane.h"
//#include "ObjectEx/Plane/LXPlayerPlane.h"
//#include "ObjectEx/LXObjectManager.h"
//
//USING_NS_CC;
//using namespace ui;
//using namespace cocostudio;
//
//
//class PlayerLayer : public cocos2d::Layer
//{
//public:
//	PlayerLayer();
//	~PlayerLayer();
//	static PlayerLayer* GetMe(){return s_PlayerLayer;};
//
//public:
//
//
//	CREATE_FUNC(PlayerLayer);
//	
//	virtual bool init();
//
//	void		 Tick(float dt);
//
//	// 初始化
//	void		Init();
//
//
//	// 属性
//	//LXPlayerPlane*	GetMayPlane(){return m_pPlayer;};
//	void		MovePlane(Vec2 deltaMove);
//
//
//private:
//
//	LXPlayerPlane*			m_pPlayer;
//	LXObjectManager*		m_pObjectManager;
//
//	// other Plane 为以后预留
//	// MyPlane*	m_OtherPlane;
//
//protected:
//	static PlayerLayer* s_PlayerLayer;
//};