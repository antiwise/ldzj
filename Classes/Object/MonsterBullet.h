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
//	// -- ��ʼ�� --------------------------
//	CREATE_FUNC(MonsterBullet);
//
//	void Init(int OwerID,int x,int y,int offsetY,bool isBoss = false);
//
//	void InitAction();
//
//	void DeleteMe();
//
//	// -- ���� ----------------------------
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
//	// -- ��Ϊ ----------------------------
//
//	
//private:
//
//	int			m_nTag;		// �ӵ�Ψһ��ʶ
//
//	int			m_OwnerID;	// �����ɻ�id
//
//	Sprite*		m_pBullet;
//
//};
