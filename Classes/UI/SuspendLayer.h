/*
==
Purpose: ��ͣ����

note:
*/

#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

class SuspendLayer : public cocos2d::Layer
{
public:
	SuspendLayer();

	~SuspendLayer();

	virtual bool init();

	CREATE_FUNC(SuspendLayer);
	
public:
	
	void	Init();

	void	Continue();	

	// ������ť�ص�
	void	GoOnTouchEvent(Ref* pSender, Widget::TouchEventType type);

	// ������ť�ص�
	void	AbandonTouchEvent(Ref* pSender, Widget::TouchEventType type);
	
	// ������Ϸ�����������
	void	GoOnAnimationPlay();

private:

	ImageView*		m_LeftPicture;			// ��ͼ
	ImageView*		m_RightPicture;			// ��ͼ
	ImageView*		m_SuspendPicture;		// ��ͣͼ
	ImageView*		m_Small_1;				// С 1
	ImageView*		m_Small_2;				// С 2
	ImageView*		m_Small_3;				// С 3
	ImageView*		m_Big_1;				// �� 1
	ImageView*		m_Big_2;				// �� 2
	ImageView*		m_Big_3;				// �� 3
	ImageView*		m_Hexagon_1;			// ������ 1
	ImageView*		m_Hexagon_2;			// ������ 2
	ImageView*		m_AbandonBack;			// ��������
	ImageView*		m_GoOnBack;				// ��������
	Button*			m_AbandonBtn;			// ������ť
	Button*			m_GoOnBtn;				// ������ť

	Vec2			m_LeftPicPos;			// ��ͼλ��
	Vec2			m_RightPicPos;			// ��ͼλ��
	Vec2			m_AbandonBackPos;		// ��������λ��
	Vec2			m_GoOnBackPos;			// ��������λ��
	Vec2			m_AbandonBtnPos;		// ������ťλ��
	Vec2			m_GoOnBtnPos;			// ������ťλ��

	Size			m_LeftPicSize;			// ��ͼ��С
	Size			m_RightPicSize;			// ��ͼ��С
	Size			m_AbandonBackSize;		// ����������С
	Size			m_GoOnBackSize;			// ����������С
	Size			m_AbandonBtnSize;		// ������ť��С
	Size			m_GoOnBtnSize;			// ������ť��С

	bool			m_IsClickGoOnButton;	// �Ƿ����˼���

};
