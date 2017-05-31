#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "ObjectEx/Plane/LXPlayerPlane.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

class LXPlayerPlane;
class LXWingPlane;

class MenuLayer : public cocos2d::Layer
{
public:

	MenuLayer();
	~MenuLayer();

	virtual bool init();

	CREATE_FUNC(MenuLayer);

public:
	
	void	Init();

	void	InitData(int _nPage = 0);
	
	void	Tick(float dt);

	// ���ݵ�ǰ����ѡ��״̬���������ȾЧ��
	void calcUIState();

	// ֹͣ���е�Ļ
	void StopAllPlaneShoot();

	// ��ʾ��ǰҳ����ʶ
	void ShowCurrPageIdxImage(const int idx);

	// ��ʾ������
	void ShowAttack(int nCurrVal,int nMaxVal);

	// ��ʾ��ʯת����
	void ShowGemChanegRatio(int nCurrVal,int nMaxVal);

	// �ɻ���Ϣ
	void ShowPlaneInfo(int idx);

	// ��ʾǮ
	void ShowMyCurrCoin(int nNum);

	// �Ƿ���Ҫ����
	bool	IsNeedBuyItem(int _Idx);

	// ˢ�½��水ť״̬��Ϣ
	void ResfreshButtonState();


	// -- callback ---------------------
	// ��ҹ���ص�
	void	BuyCoinTouchEvent(Ref* pSender, Widget::TouchEventType type);

	// ���� �ص�
	void	ToLeftTouchEvent(Ref* pSender, Widget::TouchEventType type);

	// ���� �ص�
	void	ToRightTouchEvent(Ref* pSender, Widget::TouchEventType type);

	// ��ҳ�ص�
	void	PageViewEvent(Ref* pSender,	PageView::EventType type);

	// ���� �ص�
	void	RankingTouchEvent(Ref* pSender, Widget::TouchEventType type);

	// ���� �ص�
	void	SettingTouchEvent(Ref* pSender, Widget::TouchEventType type);

	// Boss �ص�
	void	BossTouchEvent(Ref* pSender, Widget::TouchEventType type);

	// BOSSlock �ص�
	void	BossLockTouchEvent(Ref* pSender, Widget::TouchEventType type);

	// �ؿ� �ص�
	void	ThemeTouchEvent(Ref* pSender, Widget::TouchEventType type);

	// ǿ�� �ص�
	void	PlaneStrengthenTouchEvent(Ref* pSender, Widget::TouchEventType type);

	// ����ɻ��ص�
	void	PlaneBuyEventTouchEvent(Ref* pSender, Widget::TouchEventType type);

	// ����״̬
	void	setState(bool value) { m_IsStopTick = value; }

private:

	// ��ǰ�߹�����ʱ��
	float	m_AllTime;

	// ��ǰѡ�е�ҳ
	int		m_nSelectPage;

	// ��ǰҳ������ʾ�� -- �ɻ�
	std::vector<LXPlayerPlane*>					m_vecPagePlane;

	bool	m_IsStopTick;				// �Ƿ�ֹͣtick

	int		m_TempCoinNum;				// Ǯ�Ҽ�¼��ʱֻ

	int		m_TempRewardNum;			// �������

	bool	m_IsJudgementPlane;			// �Ƿ��жϷɻ�

private:

	// �������

	PageView*		m_PageView;					// �ɻ�ҳ

	ImageView*		m_ToLeftButton;				// ����ť
	ImageView*		m_ToRightButton;			// ���Ұ�ť

	ImageView*		m_PlaneIcon;				// �ɻ�ͼ��
	ImageView*		m_PlaneName;				// �ɻ���

	Text*			m_Attack;					// ����
	Text*			m_GemChangeRatio;			// ��ʯת����

	
	LoadingBar*		m_AttackBar;				// ������
	LoadingBar*		m_GemChangeBar;				// ��ʯת����
	
	Button*			m_PlaneStrengthen;			// ս��ǿ��
	Button*			m_PlaneBuy;					// �ɻ�����
	Button*			m_BossOpenBtn;				// BOSS�ؿ�


	TextAtlas*		m_CurrCoin;					// ��ǰǮ��

	ImageView*		m_RewardTips;				// ������ȡ��ʾ

	std::vector<ImageView*>		m_vecPageIdx;	// ҳ��ָʾ
		
};
