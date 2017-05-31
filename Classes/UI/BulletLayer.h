//#pragma once
//
//#include "cocos2d.h"
//#include "ui/CocosGUI.h"
//#include "cocostudio/CocoStudio.h"
//#include "Object/MyBullet.h"
//#include "Object/MonsterBullet.h"
//
//USING_NS_CC;
//using namespace ui;
//using namespace cocostudio;
//
//class BulletLayer : public cocos2d::Layer
//{
//public:
//	BulletLayer();
//	~BulletLayer();
//
//	static BulletLayer* GetMe(){return s_BulletLayer;}
//public:
//
//	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
//	virtual bool init();
//
//	CREATE_FUNC(BulletLayer);
//
//
//	// -- ���	-------------
//	// �Լ��ӵ�
//	void				AddMyBullet(MyBullet* pBullet);
//	
//	// �л��ӵ�
//	void				AddMonsterBullet(MonsterBullet* pBullet);
//
//	// -- ���	-------------
//	// �Լ��ӵ�
//	void				RemoveMyBullet(int tag);
//
//	// �л��ӵ�
//	void				RemoveMonsterBullet(int tag);
//	void				RemoveMonsterAllBulletWithPlaneId(int PlaneId);
//
//	// -- ��ȡ --------------
//	// �Լ����ӵ�
//	MyBullet*			GetMyBullet(int tag);
//
//	// �л����ӵ�
//	MonsterBullet*		GetMonsterBullet(int tag);
//
//	// -- ��ȡ���� ---------
//	std::map<long,MyBullet*>&		GetMyBulletMap(){ return m_MyBullerMap; } 
//	
//	std::map<long,MonsterBullet*>&	GetMonsterBulletMap(){ return m_MonsterBullerMap; } 
//
//	// -- ���� -----------
//	long	GetMaxBulletTag(){ return ++ Bullet_tagId; }
//
//private:
//	std::map<long,MyBullet*>		m_MyBullerMap;
//
//	std::map<long,MonsterBullet*>	m_MonsterBullerMap;
//
//	static long	Bullet_tagId;
//
//protected:
//	static BulletLayer* s_BulletLayer;
//};
