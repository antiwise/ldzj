#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

#define All_Time_Of_Add_Num	1.5f		// 多少秒加载完

class PassCountLayer : public Layer
{
	PassCountLayer();
	~PassCountLayer();

public:

	CREATE_FUNC(PassCountLayer);

	virtual bool init();
	
	void	Tick(float dt);

	void	Init(int Diamond,int KillNum,int Scour);

	// 继续按钮回调
	void	GoOnTouchEvent(Ref* pSender, Widget::TouchEventType type);

	// 强化按钮回调
	void	IntensifyTouchEvent(Ref* pSender, Widget::TouchEventType type);

	// 关闭按钮回调
	void	CloseTouchEvent(Ref* pSender, Widget::TouchEventType type);

private:

	TextAtlas*	m_DiamondAtlas;			// 宝石控件
	TextAtlas*	m_KillAtlas;			// 杀敌控件
	TextAtlas*	m_ScoreAtlas;			// 得分控件

	int			m_Diamond;				// 宝石数
	int			m_Kill;					// 杀敌数
	int			m_Score;				// 得分

	bool		m_isShowDoneWithNum;	// 数字是否显示完毕
	float		m_AllTime;				// 走过的总时间

};
