#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "GameDefine.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

enum
{
	proc_loadui,
	proc_loading,
	proc_story,
	proc_storying,
	proc_main,
	proc_end
};

enum
{
	ui_load_MainLayer,
	ui_load_UpgradeLayer,
	ui_load_RankingLayer,
	ui_load_MissionLayer,
	ui_load_BuyBoxLayer,
	ui_load_SettingLayer,
	ui_load_Prop,
	ui_load_NiceGirl,
	ui_load_TaskTips,
	ui_load_Login
};

class GameScene;
class CDataBaseSystem;
class MenuLayer;
class UpgradeLayer;
class LXObjectManager;
class RankingLayer;
class MissionLayer;
class BuyBoxLayer;
class SettingLayer;
class Login;
class NiceGirl;
class LoadingLayer;
class KeyBoardLayer;
class TaskTips;

class MainScene : public cocos2d::Layer
{
public:

	MainScene();
	~MainScene();

	virtual bool init();
	void	LoadUI();
	CREATE_FUNC(MainScene);

	// 触摸回调
	virtual bool onTouchBegan(Touch *touch, Event *unused_event); 
	virtual void onTouchMoved(Touch *touch, Event *unused_event); 
	virtual void onTouchEnded(Touch *touch, Event *unused_event); 
	virtual void onTouchCancelled(Touch *touch, Event *unused_event);

public:

	void	Tick(float dt);

	//设置关卡模式
	void	SetMissonType(bool isbossmisson);

	//设置关卡编号
	void	SetMisson(int missionID);
	int		GetMisson() { return m_nMission;}

	// 设置关卡挑战状态
	void	SetMissionState(bool value) { m_bWin = value; } 
	bool    GetMissionState() { return m_bWin; }

	//接口函数
	void	TurntoGameSence();											// 进入战斗
	void	LeaveGameSence( INTERFACE_ID eType = INTERFACE_MAIN );		// 离开战斗

	void	ShowUpgradeLayer();					// 显示强化界面
	void	HideUpgradeLayer();					// 隐藏强化界面
	void	HideUpgradeLayerWithNewPlayer();	// 隐藏强化界面(新手教学结束时)
	
	void	OpenRankingLayer();		// 打开排行界面
	void	CloseRankingLayer();	// 关闭排行界面

	void	OpenSettingLayer();		// 打开设置界面
	void	CloseSettingLayer();	// 关闭设置界面

	void	ShowMissonLayer(bool isBossMisson);	// 显示关卡界面
	void	HideMissonLayer();	// 隐藏关卡界面

	void	ShowBuyBoxLayer(BuyType eType, bool isPlayAnima = true );							// 显示购买界面
	void	ShowOverflowBag(BuyType eType = BuyType::Buy_Diamond,bool isPlayAnima = true );		// 显示超值礼包界面
	void	ShowCounDownFlowBag( BuyType eType = BuyType::Buy_Life,int duration = 5,bool isPlayAnima = true );  // 显示超值礼包界面,带倒计时
	void	HideBuyBoxLayer();											// 隐藏购买界面

	void	CleanGameScene();	// 清理战斗场景
	GameScene* GetGameScene()	{ return m_pGameScene;}

	void	PayContinue(); //付费成功后继续UI逻辑
	
	// 获取节点
	Node*		GetNodeByZOrder(int zorder);

	/*------- 主界面相关 --------------------------------------------------------------*/
	// 刷新主界面状态
	void	ResfreshMenuLayerState();
	void	ShowMenuLayerAndBetterPlane();			// 显示主界面，同时显示最好的飞机


	/*------- 强化界面相关 --------------------------------------------------------------*/
	// 刷新强化界面状态
	bool	IsShowUpgradeLayer();
	void	RefreshUpgradeLayerState();

	/*------- 战斗场景相关 --------------------------------------------------------------*/
	void	GamePause();					// 暂停
	bool	IsGamePause();					// 暂停
	void	GameContinue();					// 继续
	void	PlaneRenascence();				// 飞机重生
	void	ClearGameScenePlayerPlane();	// 清理战斗场景中玩家的飞机
	void	PlaneCreatShieldEffect();		// 飞机护盾创建

	/*------- 显示美女 --------------------------------------------------------------*/
	void	ShowNiceGirl();			// 显示美女
	void	ShowNewPlayer();		// 显示新手引导
	void	GoToNextStep();			// 继续下一步

	void	ShowMissionStartStory(int nIdx);		// 关卡开始剧情
	void	ShowMissionEndStory(int nIdx);			// 关卡结束剧情

	/*---------------------------------------------------------------------*/
	// 排行界面
	void	ReLoadRankingInfo();

	/*---------------------------------------------------------------------*/
	// 提示界面
	void	TaskTipsShow(std::string _str);

public:

	void	SetPlaneID(int idx);
	int		GetPlaneID(){ return m_nDefaultPlane; }

	void	PlayStory();
	void	HideStoryPage1();
	void	CleanStory();
	void	ShowSkipButton();
	void	SkipStory();

public:
	void	End();
	bool	IsLoadOver() {	return m_bLoadOver && m_pLoadingLayer == nullptr; } 
	void	SetLoadingLayerVisible(bool value);
	void	ParseThread(MainScene *pScene);
	void	SetStory(int proc) { m_nProc = proc; }
	void	SetTargetProgress(int value);
	void	SetProgressDelta(float value) { m_fProgressDelta = value; }


private:
	std::vector<Layer*>		m_LayerList;
	
	LoadingLayer*			m_pLoadingLayer;	// 加载界面
	GameScene*				m_pGameScene;		// 游戏场景
	MenuLayer*				m_pMenuLayer;		// 主界面
	UpgradeLayer*			m_pUpgradeLayer;	// 装备强化界面
	RankingLayer*			m_pRankingLayer;	// 排行界面
	MissionLayer*			m_pMissionLayer;	// 关卡选择界面
	BuyBoxLayer*			m_pBuyBoxLayer;		// 购买界面
	SettingLayer*			m_pSettingLayer;	// 设置界面
	TaskTips*				m_pTaskTipsLayer;	// 提示界面

	Login*					m_pLogin;			// 每日登录
	NiceGirl*				m_pNiceGirl;		// 美女
	KeyBoardLayer*			m_pKeyBoardLayer;	// 按键监听层(必须是最上层)

	bool					m_bCleanGameSence;		// 战斗场景清理标识
	INTERFACE_ID			m_GoToInterfaceType;	// 是否转到关卡选择界面

	//数据
	bool					m_bIsBossMisson;	// 是否BOSS挑战模式
	int						m_nMission;			// 关卡编号
	int						m_nDifficulty;		// 关卡难度 0:easy 1:normal 2:hard
	int						m_nDefaultPlane;	// 默认飞机
	bool					m_bWin;				// 是否成功挑战
	

	//背景
	Sprite*					pBg_1;
	Sprite*					pBg_2;
	Sprite*					pBg_3;
	Sprite*					pBg_4;
	Sprite*					pBg_5;

	//动画
	Sprite*					pAnimate_1_1;
	Sprite*					pAnimate_1_2;
	Sprite*					pAnimate_2_1;
	Sprite*					pAnimate_4_1;
	Sprite*					pAnimate_5_1;
	Sprite*					pAnimate_5_2;

	//文字
	ProgressTimer*			pTimer;
	ProgressTimer*			pTimer21;
	ProgressTimer*			pTimer22;
	ProgressTimer*			pTimer31;
	ProgressTimer*			pTimer32;
	ProgressTimer*			pTimer4;
	ProgressTimer*			pTimer5;

	//跳过
	Sprite*					pSkipText;
	Sprite*					pSkipBtn;
	
	int						m_nProc;
	int						m_nUILoadingProc;
	int						m_nProgress;
	int						m_nTargetProgress;
	float					m_fProgressDelta;
	bool					m_bLoadOver;
	std::thread				*m_pPreLoad;
};

class KeyBoardLayer : public cocos2d::Layer
{
public:
	KeyBoardLayer();
	~KeyBoardLayer();

	virtual bool init();
	CREATE_FUNC(KeyBoardLayer);
public:

	virtual void	onKeyReleased(EventKeyboard::KeyCode keyCode,Event * pEvent); //按键响应

private:

};
