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
////	NOT_MOVE,				// ����
////	TOP,					// ��
////	BOTTOM,					// ��
////	LEFT,					// ��
////	RIGHT,					// ��
////};
//
//
//class MyPlane : public ::Plane
//{
//public:
//	MyPlane(void);
//	~MyPlane(void);
//
//	CREATE_FUNC(MyPlane);		// ����
//
//	// ��ʼ��
//	void Init();
//
//
//	void Tick(float dt);
//
//	// ����
//	void	SetIsStartMove(bool bIs){m_isStartMove = bIs;}
//	bool	isStartMove(){return m_isStartMove;}
//
//	void	SetIsPlayedAnima(bool bIs){m_isPlayerAnim = bIs;}
//	bool	IsPlayedAnima(){return m_isPlayerAnim;}
//
//	Rect	GetPlaneRect();		// �ɻ�����ײ��С
//
//	Rect	GetBulletRect();	// �ӵ��Ĺ�����Χ
//
//	// ��Ⱦ -----------
//	// ���ж���
//	void	PlayerMoveAnimation();
//	void	AnimationCallback();
//
//	// ��Ϊ --------
//	// �ƶ�
//	void	SetAimX(float x){m_nAimLengthX = x;}
//	void	SetAimY(float y){m_nAimLengthY = y;}
//
//	void	Move(float x,float y);
//
//	// ����
//	void	OpenFire();
//
//	// �ܻ�
//	virtual void BeHit(int damage);
//
//	void	DieCallback();
//
//	// �ص�
//	virtual bool onTouchBegan(Touch *touch, Event *unused_event); 
//	virtual void onTouchMoved(Touch *touch, Event *unused_event); 
//	virtual void onTouchEnded(Touch *touch, Event *unused_event); 
//	virtual void onTouchCancelled(Touch *touch, Event *unused_event);
//
//private:
//
//	Sprite*		m_pMyPlane;			// �ɻ�
//	Animate* 	m_pToLeftAnima;		// ���󶯻�
//	Animate*	m_pToRightAnima;	// ���Ҷ���
//
//	float		m_nAimLengthX;		// Ŀ��X����
//	float		m_nAimLengthY;		// Ŀ��Y����
//
//	bool		m_isStartMove;		// �Ƿ�ʼ�ƶ�
//	bool		m_isPlayerAnim;		// �Ƿ񲥷Ŷ���
//	bool		m_isDeath;			// �Ƿ�����
//
//	//PlaneMoveTo m_eMoveType;		// �ƶ�����
//	Rect		m_BulletRect;		// �ӵ��Ĺ�����Χ
//
//	float		m_AllTime;				// ��ʱ��
//	float		m_nextTime;				// �´ο���ʱ��
//	int			m_nFireBulletNumOnec;	// ���η����ӵ�������
//};
//
