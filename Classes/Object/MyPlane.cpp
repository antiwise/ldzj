//#include "MyPlane.h"
//#include "Object/MyBullet.h"
//#include "UI/PlayerLayer.h"
//
//
//MyPlane::MyPlane(void):Plane()
//{
//	m_pMyPlane = nullptr;
//	m_pToLeftAnima = nullptr;
//	m_pToRightAnima = nullptr;
//
//	m_nAimLengthX = 0.0f;
//	m_nAimLengthY = 0.0f;
//
//	m_AllTime = 0.0f;
//	m_nextTime = To_Next_Fire_Time;
//
//	m_isStartMove = false;
//	m_isPlayerAnim = false;
//	m_isDeath = false;
//	
//	m_nFireBulletNumOnec = 0;
//
//	//m_eMoveType = PlaneMoveTo::NOT_MOVE;
//}
//
//
//MyPlane::~MyPlane(void)
//{
//}
//
//void MyPlane::Init()
//{
//	Size visibleSize = Director::getInstance()->getVisibleSize();
//	Vec2 origin = Director::getInstance()->getVisibleOrigin();
//
//	//m_pMyPlane = Sprite::create("Texture/Res/maps/p03-1.png",Rect(118,0,79,87));
//	//m_pMyPlane->setPosition(Vec2( visibleSize.width/2+ origin.x ,m_pMyPlane->getContentSize().height));
//	//this->addChild(m_pMyPlane);
//
//	//SetMaxHp(30);
//	//SetHp(150);
//	//SetAttack(30);
//
//	//// 构造关键帧
//	//Vector<SpriteFrame*> vec;
//	//SpriteFrame *frame;
//	//
//	//// 向左动画
//	//frame = SpriteFrame::create("Texture/Res/maps/p03-1.png",Rect(118,0,79,87));
//	//vec.pushBack(frame);
//
//	//frame = SpriteFrame::create("Texture/Res/maps/p03-1.png",Rect(197,0,65,87));
//	//vec.pushBack(frame);
//
//	//frame = SpriteFrame::create("Texture/Res/maps/p03-1.png",Rect(264,0,57,87));
//	//vec.pushBack(frame);
//
//
//	//Animation* pToLeftAnima = Animation::createWithSpriteFrames(vec,0.05f);
//	//if (pToLeftAnima)
//	//{
//	//	m_pToLeftAnima = Animate::create(pToLeftAnima);
//	//	m_pToLeftAnima->retain();
//	//}
//
//	//// 向右动画
//	//vec.clear();
//	//frame = SpriteFrame::create("Texture/Res/maps/p03-1.png",Rect(118,0,79,87));
//	//vec.pushBack(frame);
//
//	//frame = SpriteFrame::create("Texture/Res/maps/p03-1.png",Rect(53,0,65,87));
//	//vec.pushBack(frame);
//
//	//frame = SpriteFrame::create("Texture/Res/maps/p03-1.png",Rect(0,0,53,87));
//	//vec.pushBack(frame);
//
// //
// //	Animation* pToRightAnima = Animation::createWithSpriteFrames(vec,0.05f);
//	//if (pToRightAnima)
//	//{
//	//	m_pToRightAnima = Animate::create(pToRightAnima);
//	//	m_pToRightAnima->retain();
//	//}
//}
//
//Rect MyPlane::GetPlaneRect()
//{
//	return m_pMyPlane->getBoundingBox();
//}
//
//Rect MyPlane::GetBulletRect()
//{
//	return Rect(m_pMyPlane->getPositionX()-15,m_pMyPlane->getPositionY()+15,30,30);
//}
//
//
//void MyPlane::Move(float x,float y)
//{
//	//PlaneMoveTo eType = PlaneMoveTo::NOT_MOVE;
//
//	//if ( x + 0.02f > 0 )
//	//{
//	//	eType = PlaneMoveTo::RIGHT;
//
//	//}else if ( x + 0.02f < 0 )
//	//{
//	//	eType = PlaneMoveTo::LEFT;
//
//	//}else
//	//{
//	//	if( y + 0.02f > 0 )
//	//	{
//	//		eType = PlaneMoveTo::TOP;
//
//	//	}else if ( y + 0.02f < 0 )
//	//	{
//	//		eType = PlaneMoveTo::BOTTOM;
//
//	//	}else
//	//	{
//	//		eType = PlaneMoveTo::NOT_MOVE;
//	//	}
//	//}
//
//	//if (m_eMoveType != eType)
//	//{
//	//	SetIsPlayedAnima(false);
//	//	m_eMoveType = eType;
//	//}
//
//	//// 计算飞机移动
//	//m_nAimLengthX = x;
//	//m_nAimLengthY = y;
//
//	//// 边界判断
//	//if ( m_pMyPlane->getPositionX() + m_nAimLengthX > visibleSize.width )
//	//{
//	//	m_nAimLengthX = visibleSize.width - m_pMyPlane->getPositionX();
//
//	//}else if ( m_pMyPlane->getPositionX() + m_nAimLengthX < 0 )
//	//{
//	//	m_nAimLengthX = 0 - m_pMyPlane->getPositionX();
//	//}
//
//	//if ( m_pMyPlane->getPositionY() + m_nAimLengthY > visibleSize.height )
//	//{
//	//	m_nAimLengthY = visibleSize.height - m_pMyPlane->getPositionY();
//	//} 
//	//else if ( m_pMyPlane->getPositionY() + m_nAimLengthY < 0 )
//	//{
//	//	m_nAimLengthY = 0 - m_pMyPlane->getPositionY();
//	//}
//
//	//m_pMyPlane->setPositionX(m_pMyPlane->getPositionX() + m_nAimLengthX );
//	//m_pMyPlane->setPositionY(m_pMyPlane->getPositionY() + m_nAimLengthY );
//
//	//PlayerMoveAnimation();
//}
//
//void MyPlane::Tick(float dt)
//{
//	m_AllTime += dt;
//	if (m_AllTime > m_nextTime)
//	{
//		if (m_nFireBulletNumOnec < One_Fire_BulletNum)
//		{
//			OpenFire();
//			m_nextTime = m_AllTime + Bullet_To_Bullet_Time;
//			m_nFireBulletNumOnec ++;
//
//		}else
//		{
//			m_nFireBulletNumOnec = 0;
//			m_nextTime = m_AllTime + To_Next_Fire_Time;
//		}		
//	}
//}
//
//bool MyPlane::onTouchBegan(Touch *touch, Event *unused_event)
//{
//	//int x = touch->getLocation().x;
//	//int y = touch->getLocation().y;
//
//	return true;
//}
//
//void MyPlane::onTouchMoved(Touch *touch, Event *unused_event)
//{	
//	//Vec2 movePos = touch->getLocationInView();					//获得触摸位置  
//	//Vec2 movePos1 = Director::getInstance()->convertToGL(movePos);	//坐标转换
//	//CCLOG("---------------------------------");
//	//CCLOG("movePos ----- X:%f,Y:%f",movePos.x,movePos.y);
//	//CCLOG("movePos1 ----- X:%f,Y:%f",movePos1.x,movePos1.y);
//	//CCLOG("getLocation ----- X:%f,Y:%f",x,y);
//
//	// 获取移动偏移量
//	Vec2 pos = touch->getDelta();
//
//	SetIsStartMove(true);		
//	Move(pos.x,pos.y);
//
//}
//
//void MyPlane::onTouchEnded(Touch *touch, Event *unused_event)
//{
//	//Vec2 endPos = touch->getLocationInView();					//获得触摸位置  
//	//Vec2 endPos1 = Director::getInstance()->convertToGL(endPos);	//坐标转换 
//	//CCLOG("---------------------------------");
//	//CCLOG("endPos ----- X:%f,Y:%f",endPos.x,endPos.y);
//	//CCLOG("endPos1 ----- X:%f,Y:%f",endPos1.x,endPos1.y);
//
//	SetIsStartMove(false);
//	m_nAimLengthX = 0.0f;
//	m_nAimLengthY = 0.0f;
//	AnimationCallback();
//}
//
//void MyPlane::onTouchCancelled(Touch *touch, Event *unused_event)
//{
//	//Vec2 cancellPos = touch->getLocationInView();					//获得触摸位置  
//	//Vec2 cancellPos1 = Director::getInstance()->convertToGL(cancellPos);	//坐标转换 
//	//CCLOG("---------------------------------");
//	//CCLOG("cancellPos ----- X:%f,Y:%f",cancellPos.x,cancellPos.y);
//	//CCLOG("cancellPos1 ----- X:%f,Y:%f",cancellPos1.x,cancellPos1.y);
//
//	SetIsStartMove(false);
//	m_nAimLengthX = 0.0f;
//	m_nAimLengthY = 0.0f;	
//	AnimationCallback();
//}
//
//
//void MyPlane::PlayerMoveAnimation()
//{
//	if (IsPlayedAnima())
//	{
//		return ;
//	}
//	if (!isStartMove())
//	{
//		return ;
//	}
//
//	/*switch (m_eMoveType)
//	{
//	case PlaneMoveTo::NOT_MOVE:
//		break;
//	case PlaneMoveTo::TOP:
//		break;
//	case PlaneMoveTo::BOTTOM:
//		break;
//	case PlaneMoveTo::LEFT:
//		{
//			SetIsPlayedAnima(true);
//
//			m_pMyPlane->runAction(m_pToLeftAnima->clone());
//		}
//		break;
//	case PlaneMoveTo::RIGHT:
//		{
//			SetIsPlayedAnima(true);
//
//			m_pMyPlane->runAction(m_pToRightAnima->clone());
//		}
//		break;
//	default:
//		break;
//	}*/
//}
//
//void MyPlane::AnimationCallback()
//{
//
//	SetIsPlayedAnima(false);
//	/*switch (m_eMoveType)
//	{
//	case PlaneMoveTo::NOT_MOVE:
//		{
//			m_pMyPlane->stopAllActions();			
//		}
//		break;
//	case PlaneMoveTo::TOP:
//		{
//			m_pMyPlane->stopAllActions();
//		}
//		break;
//	case PlaneMoveTo::BOTTOM:
//		{
//			m_pMyPlane->stopAllActions();
//		}
//		break;
//	case PlaneMoveTo::LEFT:
//		{
//			m_pMyPlane->runAction(m_pToLeftAnima->clone()->reverse());
//		}
//		break;
//	case PlaneMoveTo::RIGHT:
//		{
//			m_pMyPlane->runAction(m_pToRightAnima->clone()->reverse());
//		}
//		break;
//	default:
//		{
//			m_pMyPlane->stopAllActions();
//		}
//		break;
//	}*/
//}
//
//void MyPlane::OpenFire()
//{
//	MyBullet* pMyBullet = MyBullet::create();
//	if ( pMyBullet)
//	{
//		pMyBullet->Init(m_pMyPlane->getPositionX(),m_pMyPlane->getPositionY());
//		pMyBullet->SetAttack(GetAttack());
//	}
//}
//
//void MyPlane::BeHit(int damage)
//{
//	if (m_isDeath)
//	{
//		return ;
//	}
//
//	if ( damage > 0 )
//	{
//		SetHp(GetHp() - damage);
//		
//		if ( GetHp() <= 0 )
//		{
//			SetHp(0);
//			m_isDeath = true;
//
//			ActionInterval* blink = Blink::create(0.5,3);
//			ActionInstant* pHide = Hide::create();
//			CallFunc* pCallFunc = CallFunc::create(CC_CALLBACK_0(MyPlane::DieCallback,this));
//			FiniteTimeAction* pAction = Sequence::create(blink,pCallFunc,NULL);
//			m_pMyPlane->runAction(pAction);
//
//		}else
//		{
//			ActionInterval* blink = Blink::create(0.5,3);
//			m_pMyPlane->runAction(blink);
//		}
//	}
//}
//
//void MyPlane::DieCallback()
//{
//	if (m_pMyPlane)
//	{
//		m_pMyPlane->setPosition(Vec2( visibleSize.width/2+ origin.x ,m_pMyPlane->getContentSize().height));
//		
//		ActionInstant *pShow = Show::create();
//		ActionInterval *pFadein = FadeIn::create(1);
//		m_pMyPlane->runAction(pShow);
//
//		m_isDeath = false;
//	}
//}