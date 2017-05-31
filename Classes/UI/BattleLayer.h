#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

class BattleLayer : public cocos2d::Layer
{
public:

	BattleLayer();
	~BattleLayer();

	virtual bool init();

	void		 Tick(float dt);

	CREATE_FUNC(BattleLayer);
	
public:

	void	PauseEvent(Ref* pSender, Widget::TouchEventType type);

	void	BombEvent(Ref* pSender, Widget::TouchEventType type);

	void	ProtectEvent(Ref* pSender, Widget::TouchEventType type);

	void	SetScore(int socre);

	void	ShowBloodBar(bool bShow);

	void	SetBloodPecent(float percent);

	// ��ʾ�����ɻ�
	void	ShowLifePlane();

	// ���򻤶�
	void	BuyProtect();

	// ����ը��
	void	BuyBomb();


	//========================================
	void	StopClickEventWithButton();				// ֹͣս�������еİ�ť����¼�
	void	StartClickEventWithButton();			// ��ʼս�������еİ�ť����¼�

private:

	Layout*			m_BattleLayout;		// ��¼layout

	ImageView*		m_Pause;			// �ݶ�

	TextAtlas*		m_Score;			// ����
	
	LoadingBar*		m_BloodYellow;		// ��ɫѪ��
	LoadingBar*		m_BloodRed;			// ��ɫѪ��
	
	Button*			m_Bomb;				// ը��
	Button*			m_Protect;			// ������

	TextAtlas*		m_BombNum;			// ը������
	TextAtlas*		m_ProtectNum;		// ����������

	std::list<ImageView*>		m_listPlaneLife;	// �����ɻ�ͼ
	Text*						m_PlaneLifeNum;		// �ɻ���������

	int				m_tempScoreNum;		// �ϴη���
	int				m_tempShieldNum;	// �ϴλ�������
	int				m_tempBombNum;		// �ϴλ�������
	int				m_tempLifeNum;		// �ϴλ�������
};
