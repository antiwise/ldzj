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

	void	LoadGuanQiaXuanZeJson();		// 加载关卡选择界面

	void	InitStarInfo();					// 初始化星球信息

	void	InitData(bool bIsBoss = false);	// 初始化

	void	SetMissonInfo(int mission);		// 设置选中关卡

	void	SetBossInfo();					// 渲染boss

	void	ShowStarSelectAnim();			// 新球选择动画

	void	ShowPageIndex();				// 显示页数

	void	RefreshStarState();				// 刷新星球状态

	// 继续按钮回调
	void	GoOnTouchEvent(Ref* pSender, Widget::TouchEventType type);

	// 返回按钮回调
	void	ReturnTouchEvent(Ref* pSender, Widget::TouchEventType type);

	// 选择关卡
	void	StarTouchEvent(Ref* pSender, Widget::TouchEventType type, int idx);

	// 翻页回调
	void	PageViewEvent(Ref* pSender,	PageView::EventType type);

	// 向左回调
	void	ToLeftTouchEvent(Ref* pSender, Widget::TouchEventType type);

	// 向右回调
	void	ToRightTouchEvent(Ref* pSender, Widget::TouchEventType type);

private:
	
	int			m_SelectIndex;					// 选择的关卡ID;
	int			m_NewAttackMission;				// 要攻打的新关卡

	Button*		m_ReturnBtn;					// 返回按钮
	Button*		m_GoToBtn;						// 出击

	ImageView*	m_ToLeftImage;					// 向左
	ImageView*	m_ToRightImage;					// 向右

	Layout*		m_SaoMiaoBack;					// 扫描背景
	ImageView*	m_SaoMiaoImage;					// 扫描图片

	ImageView*  m_BossInfoBack;					// boss显示信息

	PageView*	m_PageView;						// 星球滑动页

	std::vector<Node*>		m_vecRepeatNode;	// 星球下面的圈的父节点

	std::vector<ImageView*>	m_vecStarName;		// 星球名
	std::vector<ImageView*>	m_vecPageNum;		// 页数容器

	std::vector<Node*>					m_AnimParent;		// boss动画的父节点
	std::vector<LXAnimationLogic*>		m_pAnimationLogic;	// boss动画

	Node*					m_BossInfoNode;		// boss信息父节点
	Point					m_pAnchorPoint;		// 当前动画锚点
	Size					m_pAnimationSize;	// 当前动画大小

	ImageView*				m_TiShiInfo;		// 提示信息
	float					m_TishiTime;		// 提示时间

	bool					m_isRefreshStarName;// 刷新

	int						m_MissionCount;		// 关卡计数
};
