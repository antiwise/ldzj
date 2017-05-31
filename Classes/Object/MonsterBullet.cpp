//#include "MonsterBullet.h"
//#include "UI/BulletLayer.h"
//
//MonsterBullet::MonsterBullet(void):Bullet()
//{
//	m_nTag = 0;
//	m_OwnerID = 0;
//	m_pBullet = nullptr;
//}
//
//
//MonsterBullet::~MonsterBullet(void)
//{
//}
//
//void MonsterBullet::Init(int OwerID,int x,int y,int offsetY,bool isBoss /* = false */)
//{
//	if (isBoss)
//	{
//		m_pBullet = Sprite::create("Texture/Res/maps/062.png");
//	}else
//	{
//		m_pBullet = Sprite::create("Texture/Res/maps/066.png");
//	}
//	
//	if (!m_pBullet)
//	{
//		return ;
//	}
//
//	SetOwnerID(OwerID);
//
//	m_pBullet->setPosition(x,y - offsetY);
//
//	SetSpeed(350);
//
//	SetRealMoveDurationTime((visibleSize.height + origin.y - offsetY) / GetSpeed());
//
//	int nTagNum = BulletLayer::GetMe()->GetMaxBulletTag();
//
//	SetMyTagId(nTagNum);
//
//	this->addChild(m_pBullet);
//	
//	BulletLayer::GetMe()->AddMonsterBullet(this);
//}
//
//void MonsterBullet::InitAction()
//{
//	if (m_pBullet)
//	{
//		ActionInterval* pMoveTo = MoveTo::create(GetRealMoveDurationTime(),Vec2(m_pBullet->getPositionX(),-100));
//		CallFunc* pCallFunc = CallFunc::create(CC_CALLBACK_0(MonsterBullet::DeleteMe,this));
//		FiniteTimeAction * pAction = Sequence::create(pMoveTo,pCallFunc,NULL);
//		m_pBullet->runAction(pAction);
//	}
//}
//
//void MonsterBullet::DeleteMe()
//{
//	BulletLayer::GetMe()->RemoveMonsterBullet(GetMyTagId());
//	this->getParent()->removeChild(this);
//}
//
//Rect MonsterBullet::GetMonsterBulletRect()
//{
//	if (m_pBullet)
//	{
//		return m_pBullet->getBoundingBox();
//	}
//	return Rect(0,0,0,0);
//}
