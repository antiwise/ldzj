#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

class	LXAnimationLogic;
class	LXObjectManager;
class	LXPlayerPlane;
class	MapLayer;			
class	SuspendLayer;	
class	BattleLayer;
class	PassCountLayer;
class	RankUp;

struct BatchInfo   //敌机出场批次信息
{
	int pos;
	int x;
	int y ;
	int model;
	int num;
	bool waitPrev;
	float delay;
	std::string action;
	int offX;
	int offY;
	float waitTime;

	BatchInfo()
	{
		Clean();
	}
	void Clean()
	{
		pos = 0;
		x = 0;
		y = 0;
		model = 0;
		num = 0;
		waitPrev = false;
		delay = 0.0;
		action = "";
		offX = 0;
		offY = 0;
		waitTime = 0.0;
	}
};

struct EnemyInfo //敌机创建信息
{
	float		createTime;	//创建时间
	int			model;
	std::string action;		//行为动作
	float		posX;		
	float		posY;
	EnemyInfo()
	{
		createTime = 0;
		model = 0;
		action = "";
		posX = 0;
		posY = 0;
	}
};

class GameScene : public Layer
{
public:
	GameScene();
	~GameScene();

public:

	CREATE_FUNC(GameScene);

	virtual bool init();

	// 游戏场景初始化	战机ID,关卡类型,关卡,难度
	void	InitGame(int planeID,bool IsBossMisson, int minssionID, int difficulty);
	//	清理
	void	CleanGame();

	// 主循环
	virtual void Tick(float dt);

	// 触摸监听 --- 单点触摸
	void		OpenListener();
	void		CloseListener();

	// 触摸回调
	virtual bool onTouchBegan(Touch *touch, Event *unused_event); 
	virtual void onTouchMoved(Touch *touch, Event *unused_event); 
	virtual void onTouchEnded(Touch *touch, Event *unused_event); 
	virtual void onTouchCancelled(Touch *touch, Event *unused_event);

	// 新手延迟回调
	void DelayTimeCallback();

	//接口函数
	void		GamePause() ;				// 暂停
	void		GameContinue();				// 继续

	void		GamePauseAndNoSuspendLayer() ;		// 暂停（没有暂停页面）
	void		GameContinueAndNoSuspendLayer();	// 继续（没有暂停页面）

	void		GameGiveUp();				// 放弃关卡
	void		GameGoToSelectMission();	// 返回关卡选择界面
	void		GameGoToIntensify();		// 返回强化界面


	void		SetBossBloodBar(float persent);
	
	// 获取节点
	Node*		GetNodeByZOrder(int zorder);

	// 预加载场景对象
	void		InitGameObject();

	// 清理玩家飞机
	void		ClearPlayerPlane();

	// 玩家重生
	void		PlayerPlaneRenascence();

	// 飞机护盾创建
	void		PlayerPlaneShieldEffect();

	// 是否暂停
	bool		IsPause() { return m_bGamePause; }

	/*---------------------------------------------------------------------*/
	// 排名上升界面
	void		OpenRankUpLayer();

	/*---------------------------------------------------------------------*/
	// 关卡统计界面
	void		ShowPassCountInfo();
	
	/*---------------------------------------------------------------------*/
	// 战斗场景UI
	void		StopClickEventWithBattleLayerButton();			// 停止战斗场景中的按钮点击事件
	void		StartClickEventWithBattleLayerButton();			// 开始战斗场景中的按钮点击事件


private:

	void MovePlane(Vec2& deltaMove);
	
	// 加载关卡敌机批次信息
	void	InitBatchInfo(bool IsBossMisson, int minssionID, int difficulty);

	//敌机批次加载逻辑
	void	EnemyBatchTick(float dt);						

	//加载关卡敌机批次数据
	int		LoadEnemyBatchInfo(const std::string& fileName);

private:
	std::vector<Layer*>		m_LayerList;

	//数据
	bool					m_bGamePause;		// 游戏暂停
	Vector<Node*>			m_vPauseNodes;		// 暂停列表

	//UI
	MapLayer*				m_pMapLayer;		// 地图
	SuspendLayer*			m_pSuspendLayer;	// 暂停
	BattleLayer*			m_pBattleLayer;		// 战斗
	PassCountLayer*			m_pPassCountLayer;	// 通关奖励
	RankUp*					m_pRankUpLayer;		// 排名上升界面

	//玩家
	LXPlayerPlane*			m_pPlayer;			// 战机

	//敌机批次数据
	std::vector<BatchInfo>	m_vBatchInfo;		// 飞机波次数据
	std::vector<EnemyInfo>	m_vEnemyInfo;		// 飞机创建信息
	float					m_fPassTime;		// 流失时间
	int						m_nBatchCount;		// 当前敌机批次
	float					m_fDistance;		// 当前移动逻辑距离

	float					m_fPreRunTime;		// 战斗开始前时间
	float					m_fAfterRunTime;	// 战斗结束后时间
	int						m_nMission;			// 当前关卡

	bool					m_IsPlayMissionStartStory;		// 是否播放关卡开始剧情
	bool					m_IsPlayMissionEndStory;		// 是否播放关卡结束剧情

	//LXAnimationLogic		*m_pTestLogic;
};
