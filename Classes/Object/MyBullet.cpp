//#include "MyBullet.h"
//#include "UI/BulletLayer.h"
//
//MyBullet::MyBullet(void):Bullet()
//{
//	m_nTag = 0;
//	m_pBullet = nullptr;
//}
//
//
//MyBullet::~MyBullet(void)
//{
//	
//}
//
//bool MyBullet::Init(int x,int y)
//{
//	m_pBullet = Sprite::create("Texture/Res/maps/p-f01.png",Rect(2,0,18,50));
//	if (!m_pBullet)
//	{
//		return false;
//	}
//
//	m_pBullet->setPosition(x,y + m_pBullet->getContentSize().height);
//
//	SetSpeed(150);
//
//	SetRealMoveDurationTime((visibleSize.height + origin.y + 100) / GetSpeed());
//
//	int nTagNum = BulletLayer::GetMe()->GetMaxBulletTag();
//
//	SetMyTagId(nTagNum);
//
//	ActionInterval* pMoveAction = MoveTo::create(GetRealMoveDurationTime(),Vec2(x,visibleSize.height + origin.y + 100));
//
//	CallFunc *pCallFunc = CallFunc::create(CC_CALLBACK_0(MyBullet::DelereMe,this));
//
//	FiniteTimeAction* pAcTion = Sequence::create(pMoveAction,pCallFunc,NULL);
//
//	m_pBullet->runAction(pAcTion);
//
//	this->addChild(m_pBullet);
//
//	BulletLayer::GetMe()->AddMyBullet(this);
//	
//	return true;
//}
//
//void MyBullet::DelereMe()
//{
//	BulletLayer::GetMe()->RemoveMyBullet(GetMyTagId());
//	if (this->getParent())
//	{
//		this->getParent()->removeChild(this);
//	}
//}
//
//Rect MyBullet::GetMyBulletRect()
//{
//	return m_pBullet->getBoundingBox();
//}