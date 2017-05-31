/*
==
Purpose: 暂停界面

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

	// 继续按钮回调
	void	GoOnTouchEvent(Ref* pSender, Widget::TouchEventType type);

	// 放弃按钮回调
	void	AbandonTouchEvent(Ref* pSender, Widget::TouchEventType type);
	
	// 继续游戏动画播放完毕
	void	GoOnAnimationPlay();

private:

	ImageView*		m_LeftPicture;			// 左图
	ImageView*		m_RightPicture;			// 右图
	ImageView*		m_SuspendPicture;		// 暂停图
	ImageView*		m_Small_1;				// 小 1
	ImageView*		m_Small_2;				// 小 2
	ImageView*		m_Small_3;				// 小 3
	ImageView*		m_Big_1;				// 大 1
	ImageView*		m_Big_2;				// 大 2
	ImageView*		m_Big_3;				// 大 3
	ImageView*		m_Hexagon_1;			// 六边形 1
	ImageView*		m_Hexagon_2;			// 六边形 2
	ImageView*		m_AbandonBack;			// 放弃背景
	ImageView*		m_GoOnBack;				// 继续背景
	Button*			m_AbandonBtn;			// 放弃按钮
	Button*			m_GoOnBtn;				// 继续按钮

	Vec2			m_LeftPicPos;			// 左图位置
	Vec2			m_RightPicPos;			// 右图位置
	Vec2			m_AbandonBackPos;		// 放弃背景位置
	Vec2			m_GoOnBackPos;			// 继续背景位置
	Vec2			m_AbandonBtnPos;		// 放弃按钮位置
	Vec2			m_GoOnBtnPos;			// 继续按钮位置

	Size			m_LeftPicSize;			// 左图大小
	Size			m_RightPicSize;			// 右图大小
	Size			m_AbandonBackSize;		// 放弃背景大小
	Size			m_GoOnBackSize;			// 继续背景大小
	Size			m_AbandonBtnSize;		// 放弃按钮大小
	Size			m_GoOnBtnSize;			// 继续按钮大小

	bool			m_IsClickGoOnButton;	// 是否点击了继续

};
