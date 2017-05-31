#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

class Login : public Layer
{
public:
	Login(void);
	~Login(void);
	
	CREATE_FUNC(Login);

	virtual bool init();

public:


	// 初始化
	void	Init();

	// 添加监听
	void  OpenListener();
	void  CloseListener();

	// 触摸回调
	virtual bool onTouchBegan(Touch *touch, Event *unused_event); 
	virtual void onTouchEnded(Touch *touch, Event *unused_event); 

	// 领取回调
	void	ReceiveTouchEvent(Ref* pSender, Widget::TouchEventType type);

	// 领取回调
	void	SureTouchEvent(Ref* pSender, Widget::TouchEventType type);


private:

	std::vector<ImageView*>		m_vecRewardBack;		// 奖励背景

	std::vector<ImageView*>		m_vecRewardNum;			// 奖励数量

	std::vector<ImageView*>		m_vecRewardEnd;			// 已领取

	ImageView*					m_ImageSureBack;		// 确定界面

	std::vector<ImageView*>		m_vecPrizeImage;		// 奖品

	std::vector<ImageView*>		m_vecPrizeNum;			// 奖品数量

	int							m_CurrIndex;			// 当前索引
	int							m_RandNum;				// 随机的数

};

