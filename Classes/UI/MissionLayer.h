#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

class LXAnimationLogic;

class MissionLayer : public Layer
{
public:

	MissionLayer();
	~MissionLayer();

	virtual bool init();

	CREATE_FUNC(MissionLayer);


public:
	bool	Init();

	void	Tick(float dt);

	void	LoadGuanQiaXuanZeJson();		// ���عؿ�ѡ�����

	void	InitStarInfo();					// ��ʼ��������Ϣ

	void	InitData(bool bIsBoss = false);	// ��ʼ��

	void	SetMissonInfo(int mission);		// ����ѡ�йؿ�

	void	SetBossInfo();					// ��Ⱦboss

	void	ShowStarSelectAnim();			// ����ѡ�񶯻�

	void	ShowPageIndex();				// ��ʾҳ��

	void	RefreshStarState();				// ˢ������״̬

	// ������ť�ص�
	void	GoOnTouchEvent(Ref* pSender, Widget::TouchEventType type);

	// ���ذ�ť�ص�
	void	ReturnTouchEvent(Ref* pSender, Widget::TouchEventType type);

	// ѡ��ؿ�
	void	StarTouchEvent(Ref* pSender, Widget::TouchEventType type, int idx);

	// ��ҳ�ص�
	void	PageViewEvent(Ref* pSender,	PageView::EventType type);

	// ����ص�
	void	ToLeftTouchEvent(Ref* pSender, Widget::TouchEventType type);

	// ���һص�
	void	ToRightTouchEvent(Ref* pSender, Widget::TouchEventType type);

private:
	
	int			m_SelectIndex;					// ѡ��Ĺؿ�ID;
	int			m_NewAttackMission;				// Ҫ������¹ؿ�

	Button*		m_ReturnBtn;					// ���ذ�ť
	Button*		m_GoToBtn;						// ����

	ImageView*	m_ToLeftImage;					// ����
	ImageView*	m_ToRightImage;					// ����

	Layout*		m_SaoMiaoBack;					// ɨ�豳��
	ImageView*	m_SaoMiaoImage;					// ɨ��ͼƬ

	ImageView*  m_BossInfoBack;					// boss��ʾ��Ϣ

	PageView*	m_PageView;						// ���򻬶�ҳ

	std::vector<Node*>		m_vecRepeatNode;	// ���������Ȧ�ĸ��ڵ�

	std::vector<ImageView*>	m_vecStarName;		// ������
	std::vector<ImageView*>	m_vecPageNum;		// ҳ������

	std::vector<Node*>					m_AnimParent;		// boss�����ĸ��ڵ�
	std::vector<LXAnimationLogic*>		m_pAnimationLogic;	// boss����

	Node*					m_BossInfoNode;		// boss��Ϣ���ڵ�
	Point					m_pAnchorPoint;		// ��ǰ����ê��
	Size					m_pAnimationSize;	// ��ǰ������С

	ImageView*				m_TiShiInfo;		// ��ʾ��Ϣ
	float					m_TishiTime;		// ��ʾʱ��

	bool					m_isRefreshStarName;// ˢ��

	int						m_MissionCount;		// �ؿ�����
};
