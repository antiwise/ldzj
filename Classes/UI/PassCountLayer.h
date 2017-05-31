#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

#define All_Time_Of_Add_Num	1.5f		// �����������

class PassCountLayer : public Layer
{
	PassCountLayer();
	~PassCountLayer();

public:

	CREATE_FUNC(PassCountLayer);

	virtual bool init();
	
	void	Tick(float dt);

	void	Init(int Diamond,int KillNum,int Scour);

	// ������ť�ص�
	void	GoOnTouchEvent(Ref* pSender, Widget::TouchEventType type);

	// ǿ����ť�ص�
	void	IntensifyTouchEvent(Ref* pSender, Widget::TouchEventType type);

	// �رհ�ť�ص�
	void	CloseTouchEvent(Ref* pSender, Widget::TouchEventType type);

private:

	TextAtlas*	m_DiamondAtlas;			// ��ʯ�ؼ�
	TextAtlas*	m_KillAtlas;			// ɱ�пؼ�
	TextAtlas*	m_ScoreAtlas;			// �÷ֿؼ�

	int			m_Diamond;				// ��ʯ��
	int			m_Kill;					// ɱ����
	int			m_Score;				// �÷�

	bool		m_isShowDoneWithNum;	// �����Ƿ���ʾ���
	float		m_AllTime;				// �߹�����ʱ��

};
