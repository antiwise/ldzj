//#pragma once
//#include "cocos2d.h"
//#include "Bullet.h"
//
//USING_NS_CC;
//
//class MyBullet : public Bullet
//{
//public:
//	MyBullet(void);
//	~MyBullet(void);
//
//	// -- 初始化 --------------------------
//	CREATE_FUNC(MyBullet);
//
//	bool Init(int x,int y);
//
//	
//	void DelereMe();
//
//	// -- 属性 ----------------------------
//
//	void		SetMyTagId(int nTag){m_nTag = nTag;};
//	int			GetMyTagId(){return m_nTag;}
//
//	Sprite*		GetMyBulletSprite(){return m_pBullet;}
//	
//	Rect		GetMyBulletRect();
//
//	// -- 行为 ----------------------------
//
//	
//private:
//
//	int			m_nTag;
//
//	Sprite*		m_pBullet;
//
//};