//#pragma once
//#include "cocos2d.h"
//#include "Bullet.h"
//
//USING_NS_CC;
//
//class MonsterBullet : public Bullet
//{
//public:
//	MonsterBullet(void);
//	~MonsterBullet(void);
//
//	// -- 初始化 --------------------------
//	CREATE_FUNC(MonsterBullet);
//
//	void Init(int OwerID,int x,int y,int offsetY,bool isBoss = false);
//
//	void InitAction();
//
//	void DeleteMe();
//
//	// -- 属性 ----------------------------
//
//
//	void		SetMyTagId(int nTag){m_nTag = nTag;};
//	int			GetMyTagId(){return m_nTag;}
//	
//	void		SetOwnerID(int nOwnerID){m_OwnerID = nOwnerID;}
//	int			GetOwnerID(){return m_OwnerID;}
//
//	Sprite*		GetMyBulletSprite(){return m_pBullet;}
//
//	Rect		GetMonsterBulletRect();
//
//	// -- 行为 ----------------------------
//
//	
//private:
//
//	int			m_nTag;		// 子弹唯一标识
//
//	int			m_OwnerID;	// 归属飞机id
//
//	Sprite*		m_pBullet;
//
//};
