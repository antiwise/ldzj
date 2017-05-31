//#include "PlayerLayer.h"
//
//using namespace cocos2d;
//
//PlayerLayer* PlayerLayer::s_PlayerLayer = nullptr;
//
//PlayerLayer::PlayerLayer():Layer()
//{
//	s_PlayerLayer = this;
//}
//
//PlayerLayer::~PlayerLayer()
//{
//
//}
//
//bool PlayerLayer::init()
//{
//	// 1. super init first
//	if ( !Layer::init() )
//	{
//		return false;
//	}
//
//	Init();
//
//	return true;
//}
//
//void PlayerLayer::Tick(float dt)
//{
//	//m_MyPlane->Tick(dt);
//}
//
//void PlayerLayer::Init()
//{
//	//m_MyPlane = MyPlane::create();
//	//if(m_MyPlane)
//	//{
//	//	m_MyPlane->Init();
//	//	this->addChild(m_MyPlane);
//	//}
//	m_pObjectManager = GetLXObjectManager();
//
//	// 玩家 
//	m_pPlayer = new LXPlayerPlane();
//	m_pPlayer->setPosition(Utils().GetFixedPosion(240,200));
//	addChild(m_pPlayer, 0, m_pObjectManager->GetTag());
//	m_pPlayer->Init(0);
//	m_pObjectManager->AddPlane(m_pPlayer);
//	m_pObjectManager->RigisterPlayer(m_pPlayer);
//}
//
//void PlayerLayer::MovePlane(Vec2 deltaMove)
//{
//	//PlaneMoveTo eType = PlaneMoveTo::NOT_MOVE;
//
//	//if ( deltaMove.x + 0.02f > 0 )
//	//{
//	//	eType = PlaneMoveTo::RIGHT;
//
//	//}else if ( deltaMove.x + 0.02f < 0 )
//	//{
//	//	eType = PlaneMoveTo::LEFT;
//
//	//}else
//	//{
//	//	if( deltaMove.y + 0.02f > 0 )
//	//	{
//	//		eType = PlaneMoveTo::TOP;
//
//	//	}else if ( deltaMove.y + 0.02f < 0 )
//	//	{
//	//		eType = PlaneMoveTo::BOTTOM;
//
//	//	}else
//	//	{
//	//		eType = PlaneMoveTo::NOT_MOVE;
//	//	}
//	//}
//
//
//	//// 计算飞机移动
//	//float m_nAimLengthX = deltaMove.x;
//	//float m_nAimLengthY = deltaMove.y;
//
//	//Size visibleSize = Director::getInstance()->getVisibleSize();
//	//Vec2 origin = Director::getInstance()->getVisibleOrigin();
//
//	//// 边界判断
//	//if ( m_pPlayer->getPositionX() + m_nAimLengthX > visibleSize.width )
//	//{
//	//	m_nAimLengthX = visibleSize.width - m_pPlayer->getPositionX();
//
//	//}else if ( m_pPlayer->getPositionX() + m_nAimLengthX < 0 )
//	//{
//	//	m_nAimLengthX = 0 - m_pPlayer->getPositionX();
//	//}
//
//	//if ( m_pPlayer->getPositionY() + m_nAimLengthY > visibleSize.height )
//	//{
//	//	m_nAimLengthY = visibleSize.height - m_pPlayer->getPositionY();
//	//} 
//	//else if ( m_pPlayer->getPositionY() + m_nAimLengthY < 0 )
//	//{
//	//	m_nAimLengthY = 0 - m_pPlayer->getPositionY();
//	//}
//
//	//m_pPlayer->Move(Vec2(m_nAimLengthX,m_nAimLengthY));
//}