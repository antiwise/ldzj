#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "GameDefine.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;


class SettingLayer : public Layer
{
public:
	SettingLayer(void);
	~SettingLayer(void);
	
	CREATE_FUNC(SettingLayer);

	virtual bool init();

public:

	void	Init();

	void	CloseEvent(Ref* pSender, Widget::TouchEventType type);
	void	HelpEvent(Ref* pSender, Widget::TouchEventType type);
	void	AboutEvent(Ref* pSender, Widget::TouchEventType type);
	void	BgMusicEvent(Ref* pSender, Widget::TouchEventType type);
	void	EffectEvent(Ref* pSender, Widget::TouchEventType type);	

	// �����ر�
	void	EjectCloseTouchEvent(Ref* pSender, Widget::TouchEventType type);	

private:

	Button*			m_btn_Close;		//�ر�
	Button*			m_btn_Help;			//����
	Button*			m_btn_About;		//����

	CheckBox*		m_cb_BgMusic;		//��������
	CheckBox*		m_cb_Effect;		//��Ч

	ImageView*		m_EjectBackImage;	// ����ͼƬ���� 
	ImageView*		m_EjectImage;		// ����ͼƬ
	Button*			m_EjectClose;		// �����رհ�ť
};
