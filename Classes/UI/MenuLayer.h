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

	// 根据当前界面选中状态计算界面渲染效果
	void calcUIState();

	// 停止所有弹幕
	void StopAllPlaneShoot();

	// 显示当前页数标识
	void ShowCurrPageIdxImage(const int idx);

	// 显示攻击力
	void ShowAttack(int nCurrVal,int nMaxVal);

	// 显示宝石转换率
	void ShowGemChanegRatio(int nCurrVal,int nMaxVal);

	// 飞机信息
	void ShowPlaneInfo(int idx);

	// 显示钱
	void ShowMyCurrCoin(int nNum);

	// 是否需要购买
	bool	IsNeedBuyItem(int _Idx);

	// 刷新界面按钮状态信息
	void ResfreshButtonState();


	// -- callback ---------------------
	// 金币购买回调
	void	BuyCoinTouchEvent(Ref* pSender, Widget::TouchEventType type);

	// 向左 回调
	void	ToLeftTouchEvent(Ref* pSender, Widget::TouchEventType type);

	// 向右 回调
	void	ToRightTouchEvent(Ref* pSender, Widget::TouchEventType type);

	// 翻页回调
	void	PageViewEvent(Ref* pSender,	PageView::EventType type);

	// 排行 回调
	void	RankingTouchEvent(Ref* pSender, Widget::TouchEventType type);

	// 设置 回调
	void	SettingTouchEvent(Ref* pSender, Widget::TouchEventType type);

	// Boss 回调
	void	BossTouchEvent(Ref* pSender, Widget::TouchEventType type);

	// BOSSlock 回调
	void	BossLockTouchEvent(Ref* pSender, Widget::TouchEventType type);

	// 关卡 回调
	void	ThemeTouchEvent(Ref* pSender, Widget::TouchEventType type);

	// 强化 回调
	void	PlaneStrengthenTouchEvent(Ref* pSender, Widget::TouchEventType type);

	// 购买飞机回调
	void	PlaneBuyEventTouchEvent(Ref* pSender, Widget::TouchEventType type);

	// 设置状态
	void	setState(bool value) { m_IsStopTick = value; }

private:

	// 当前走过的总时间
	float	m_AllTime;

	// 当前选中的页
	int		m_nSelectPage;

	// 当前页数中显示项 -- 飞机
	std::vector<LXPlayerPlane*>					m_vecPagePlane;

	bool	m_IsStopTick;				// 是否停止tick

	int		m_TempCoinNum;				// 钱币记录临时只

	int		m_TempRewardNum;			// 任务计数

	bool	m_IsJudgementPlane;			// 是否判断飞机

private:

	// 界面相关

	PageView*		m_PageView;					// 飞机页

	ImageView*		m_ToLeftButton;				// 向左按钮
	ImageView*		m_ToRightButton;			// 向右按钮

	ImageView*		m_PlaneIcon;				// 飞机图标
	ImageView*		m_PlaneName;				// 飞机名

	Text*			m_Attack;					// 攻击
	Text*			m_GemChangeRatio;			// 宝石转化率

	
	LoadingBar*		m_AttackBar;				// 攻击条
	LoadingBar*		m_GemChangeBar;				// 宝石转换条
	
	Button*			m_PlaneStrengthen;			// 战机强化
	Button*			m_PlaneBuy;					// 飞机购买
	Button*			m_BossOpenBtn;				// BOSS关卡


	TextAtlas*		m_CurrCoin;					// 当前钱币

	ImageView*		m_RewardTips;				// 奖励领取提示

	std::vector<ImageView*>		m_vecPageIdx;	// 页数指示
		
};
