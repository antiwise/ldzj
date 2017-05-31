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

	// 弹出关闭
	void	EjectCloseTouchEvent(Ref* pSender, Widget::TouchEventType type);	

private:

	Button*			m_btn_Close;		//关闭
	Button*			m_btn_Help;			//帮助
	Button*			m_btn_About;		//关于

	CheckBox*		m_cb_BgMusic;		//背景音乐
	CheckBox*		m_cb_Effect;		//音效

	ImageView*		m_EjectBackImage;	// 弹出图片背景 
	ImageView*		m_EjectImage;		// 弹出图片
	Button*			m_EjectClose;		// 弹出关闭按钮
};
