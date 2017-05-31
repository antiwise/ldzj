//#pragma once
//
//#include "cocos2d.h"
//
//USING_NS_CC;
//
//class Bullet : public Node
//{
//public:
//	Bullet(void);
//	~Bullet(void);
//
//	// -- 属性 ----------------------------
//	
//	// 子弹攻击力
//	void		SetAttack(int nAttack){m_nAttack = nAttack;}
//	int			GetAttack(){return m_nAttack;}
//
//	// 子弹速度
//	void		SetSpeed(float fSpeed){ m_fSpeed = fSpeed; }
//	float		GetSpeed(){ return m_fSpeed; }
//	
//	// 子弹有效持续时间
//	void		SetRealMoveDurationTime(float fTime){ m_fRealMoveDurationTime = fTime; }
//	float		GetRealMoveDurationTime(){ return m_fRealMoveDurationTime; }
//
//	// -- 行为 ----------------------------
//
//public:
//
//	// 屏幕尺寸记录  这个是为了方便使用
//	Size visibleSize;
//	Vec2 origin;
//
//private:
//
//	int			m_nAttack;					// 攻击力
//	float		m_fSpeed;					// 速度
//	float		m_fRealMoveDurationTime;	// 持续时间
//};
//
