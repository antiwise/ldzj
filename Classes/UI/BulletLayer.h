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
//	// -- 添加	-------------
//	// 自己子弹
//	void				AddMyBullet(MyBullet* pBullet);
//	
//	// 敌机子弹
//	void				AddMonsterBullet(MonsterBullet* pBullet);
//
//	// -- 添加	-------------
//	// 自己子弹
//	void				RemoveMyBullet(int tag);
//
//	// 敌机子弹
//	void				RemoveMonsterBullet(int tag);
//	void				RemoveMonsterAllBulletWithPlaneId(int PlaneId);
//
//	// -- 获取 --------------
//	// 自己的子弹
//	MyBullet*			GetMyBullet(int tag);
//
//	// 敌机的子弹
//	MonsterBullet*		GetMonsterBullet(int tag);
//
//	// -- 获取容器 ---------
//	std::map<long,MyBullet*>&		GetMyBulletMap(){ return m_MyBullerMap; } 
//	
//	std::map<long,MonsterBullet*>&	GetMonsterBulletMap(){ return m_MonsterBullerMap; } 
//
//	// -- 索引 -----------
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
