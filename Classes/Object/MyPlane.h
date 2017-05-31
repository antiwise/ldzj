//#pragma once
//#include "cocos2d.h"
//#include "Plane.h"
//
//USING_NS_CC;
//
//#define One_Fire_BulletNum	8
//#define To_Next_Fire_Time	2.0f
//#define Bullet_To_Bullet_Time 0.5
//
////enum PlaneMoveTo
////{
////	NOT_MOVE,				// 不动
////	TOP,					// 上
////	BOTTOM,					// 下
////	LEFT,					// 左
////	RIGHT,					// 右
////};
//
//
//class MyPlane : public ::Plane
//{
//public:
//	MyPlane(void);
//	~MyPlane(void);
//
//	CREATE_FUNC(MyPlane);		// 创建
//
//	// 初始化
//	void Init();
//
//
//	void Tick(float dt);
//
//	// 属性
//	void	SetIsStartMove(bool bIs){m_isStartMove = bIs;}
//	bool	isStartMove(){return m_isStartMove;}
//
//	void	SetIsPlayedAnima(bool bIs){m_isPlayerAnim = bIs;}
//	bool	IsPlayedAnima(){return m_isPlayerAnim;}
//
//	Rect	GetPlaneRect();		// 飞机的碰撞大小
//
//	Rect	GetBulletRect();	// 子弹的攻击范围
//
//	// 渲染 -----------
//	// 飞行动画
//	void	PlayerMoveAnimation();
//	void	AnimationCallback();
//
//	// 行为 --------
//	// 移动
//	void	SetAimX(float x){m_nAimLengthX = x;}
//	void	SetAimY(float y){m_nAimLengthY = y;}
//
//	void	Move(float x,float y);
//
//	// 开火
//	void	OpenFire();
//
//	// 受击
//	virtual void BeHit(int damage);
//
//	void	DieCallback();
//
//	// 回调
//	virtual bool onTouchBegan(Touch *touch, Event *unused_event); 
//	virtual void onTouchMoved(Touch *touch, Event *unused_event); 
//	virtual void onTouchEnded(Touch *touch, Event *unused_event); 
//	virtual void onTouchCancelled(Touch *touch, Event *unused_event);
//
//private:
//
//	Sprite*		m_pMyPlane;			// 飞机
//	Animate* 	m_pToLeftAnima;		// 向左动画
//	Animate*	m_pToRightAnima;	// 向右动画
//
//	float		m_nAimLengthX;		// 目标X长度
//	float		m_nAimLengthY;		// 目标Y长度
//
//	bool		m_isStartMove;		// 是否开始移动
//	bool		m_isPlayerAnim;		// 是否播放动画
//	bool		m_isDeath;			// 是否死亡
//
//	//PlaneMoveTo m_eMoveType;		// 移动方向
//	Rect		m_BulletRect;		// 子弹的攻击范围
//
//	float		m_AllTime;				// 总时长
//	float		m_nextTime;				// 下次开火时间
//	int			m_nFireBulletNumOnec;	// 本次发出子弹的数量
//};
//
