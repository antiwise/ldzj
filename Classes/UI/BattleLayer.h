#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

class BattleLayer : public cocos2d::Layer
{
public:

	BattleLayer();
	~BattleLayer();

	virtual bool init();

	void		 Tick(float dt);

	CREATE_FUNC(BattleLayer);
	
public:

	void	PauseEvent(Ref* pSender, Widget::TouchEventType type);

	void	BombEvent(Ref* pSender, Widget::TouchEventType type);

	void	ProtectEvent(Ref* pSender, Widget::TouchEventType type);

	void	SetScore(int socre);

	void	ShowBloodBar(bool bShow);

	void	SetBloodPecent(float percent);

	// 显示生命飞机
	void	ShowLifePlane();

	// 购买护盾
	void	BuyProtect();

	// 购买炸弹
	void	BuyBomb();


	//========================================
	void	StopClickEventWithButton();				// 停止战斗场景中的按钮点击事件
	void	StartClickEventWithButton();			// 开始战斗场景中的按钮点击事件

private:

	Layout*			m_BattleLayout;		// 记录layout

	ImageView*		m_Pause;			// 暂定

	TextAtlas*		m_Score;			// 分数
	
	LoadingBar*		m_BloodYellow;		// 黄色血条
	LoadingBar*		m_BloodRed;			// 红色血条
	
	Button*			m_Bomb;				// 炸弹
	Button*			m_Protect;			// 保护罩

	TextAtlas*		m_BombNum;			// 炸弹数量
	TextAtlas*		m_ProtectNum;		// 保护罩数量

	std::list<ImageView*>		m_listPlaneLife;	// 生命飞机图
	Text*						m_PlaneLifeNum;		// 飞机生命数量

	int				m_tempScoreNum;		// 上次分数
	int				m_tempShieldNum;	// 上次护盾数量
	int				m_tempBombNum;		// 上次护盾数量
	int				m_tempLifeNum;		// 上次活力数量
};
