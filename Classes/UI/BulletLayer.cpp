//#include "BulletLayer.h"
//
//long BulletLayer::Bullet_tagId = 0;
//BulletLayer* BulletLayer::s_BulletLayer = nullptr;
//
//
//BulletLayer::BulletLayer()
//{
//	s_BulletLayer = this;
//}
//
//BulletLayer::~BulletLayer()
//{
//
//}
//
//bool BulletLayer::init()
//{
//	// 1. super init first
//	if ( !Layer::init() )
//	{
//		return false;
//	}
//
//	return true;
//}
//
//void BulletLayer::AddMyBullet(MyBullet* pBullet)
//{	
//	if (pBullet)
//	{
//		this->addChild(pBullet);
//
//		m_MyBullerMap[pBullet->GetMyTagId()] = pBullet;
//	}
//}
//
//void BulletLayer::AddMonsterBullet(MonsterBullet* pBullet)
//{	
//	if (pBullet)
//	{
//		this->addChild(pBullet);
//
//		m_MonsterBullerMap[pBullet->GetMyTagId()] = pBullet;
//	}
//}
//
//void BulletLayer::RemoveMyBullet(int tag)
//{
//	m_MyBullerMap.erase(tag);
//}
//
//void BulletLayer::RemoveMonsterBullet(int tag)
//{
//	m_MonsterBullerMap.erase(tag);
//}
//
//void BulletLayer::RemoveMonsterAllBulletWithPlaneId(int PlaneID)
//{
//	auto it = m_MonsterBullerMap.begin();
//
//	while( it !=  m_MonsterBullerMap.end() )
//	{
//		if ( (it->second)->GetOwnerID() == PlaneID )
//		{
//			it->second->getParent()->removeChild(it->second);
//			it = m_MonsterBullerMap.erase(it);
//		}else
//		{
//			it++;
//		}
//	}
//}
//
//
//MyBullet* BulletLayer::GetMyBullet(int tag)
//{
//	return m_MyBullerMap.at(tag);
//}
//
//MonsterBullet* BulletLayer::GetMonsterBullet(int tag)
//{
//	return m_MonsterBullerMap.at(tag);
//}