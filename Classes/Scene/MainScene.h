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

	// �����ص�
	virtual bool onTouchBegan(Touch *touch, Event *unused_event); 
	virtual void onTouchMoved(Touch *touch, Event *unused_event); 
	virtual void onTouchEnded(Touch *touch, Event *unused_event); 
	virtual void onTouchCancelled(Touch *touch, Event *unused_event);

public:

	void	Tick(float dt);

	//���ùؿ�ģʽ
	void	SetMissonType(bool isbossmisson);

	//���ùؿ����
	void	SetMisson(int missionID);
	int		GetMisson() { return m_nMission;}

	// ���ùؿ���ս״̬
	void	SetMissionState(bool value) { m_bWin = value; } 
	bool    GetMissionState() { return m_bWin; }

	//�ӿں���
	void	TurntoGameSence();											// ����ս��
	void	LeaveGameSence( INTERFACE_ID eType = INTERFACE_MAIN );		// �뿪ս��

	void	ShowUpgradeLayer();					// ��ʾǿ������
	void	HideUpgradeLayer();					// ����ǿ������
	void	HideUpgradeLayerWithNewPlayer();	// ����ǿ������(���ֽ�ѧ����ʱ)
	
	void	OpenRankingLayer();		// �����н���
	void	CloseRankingLayer();	// �ر����н���

	void	OpenSettingLayer();		// �����ý���
	void	CloseSettingLayer();	// �ر����ý���

	void	ShowMissonLayer(bool isBossMisson);	// ��ʾ�ؿ�����
	void	HideMissonLayer();	// ���عؿ�����

	void	ShowBuyBoxLayer(BuyType eType, bool isPlayAnima = true );							// ��ʾ�������
	void	ShowOverflowBag(BuyType eType = BuyType::Buy_Diamond,bool isPlayAnima = true );		// ��ʾ��ֵ�������
	void	ShowCounDownFlowBag( BuyType eType = BuyType::Buy_Life,int duration = 5,bool isPlayAnima = true );  // ��ʾ��ֵ�������,������ʱ
	void	HideBuyBoxLayer();											// ���ع������

	void	CleanGameScene();	// ����ս������
	GameScene* GetGameScene()	{ return m_pGameScene;}

	void	PayContinue(); //���ѳɹ������UI�߼�
	
	// ��ȡ�ڵ�
	Node*		GetNodeByZOrder(int zorder);

	/*------- ��������� --------------------------------------------------------------*/
	// ˢ��������״̬
	void	ResfreshMenuLayerState();
	void	ShowMenuLayerAndBetterPlane();			// ��ʾ�����棬ͬʱ��ʾ��õķɻ�


	/*------- ǿ��������� --------------------------------------------------------------*/
	// ˢ��ǿ������״̬
	bool	IsShowUpgradeLayer();
	void	RefreshUpgradeLayerState();

	/*------- ս��������� --------------------------------------------------------------*/
	void	GamePause();					// ��ͣ
	bool	IsGamePause();					// ��ͣ
	void	GameContinue();					// ����
	void	PlaneRenascence();				// �ɻ�����
	void	ClearGameScenePlayerPlane();	// ����ս����������ҵķɻ�
	void	PlaneCreatShieldEffect();		// �ɻ����ܴ���

	/*------- ��ʾ��Ů --------------------------------------------------------------*/
	void	ShowNiceGirl();			// ��ʾ��Ů
	void	ShowNewPlayer();		// ��ʾ��������
	void	GoToNextStep();			// ������һ��

	void	ShowMissionStartStory(int nIdx);		// �ؿ���ʼ����
	void	ShowMissionEndStory(int nIdx);			// �ؿ���������

	/*---------------------------------------------------------------------*/
	// ���н���
	void	ReLoadRankingInfo();

	/*---------------------------------------------------------------------*/
	// ��ʾ����
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
	
	LoadingLayer*			m_pLoadingLayer;	// ���ؽ���
	GameScene*				m_pGameScene;		// ��Ϸ����
	MenuLayer*				m_pMenuLayer;		// ������
	UpgradeLayer*			m_pUpgradeLayer;	// װ��ǿ������
	RankingLayer*			m_pRankingLayer;	// ���н���
	MissionLayer*			m_pMissionLayer;	// �ؿ�ѡ�����
	BuyBoxLayer*			m_pBuyBoxLayer;		// �������
	SettingLayer*			m_pSettingLayer;	// ���ý���
	TaskTips*				m_pTaskTipsLayer;	// ��ʾ����

	Login*					m_pLogin;			// ÿ�յ�¼
	NiceGirl*				m_pNiceGirl;		// ��Ů
	KeyBoardLayer*			m_pKeyBoardLayer;	// ����������(���������ϲ�)

	bool					m_bCleanGameSence;		// ս�����������ʶ
	INTERFACE_ID			m_GoToInterfaceType;	// �Ƿ�ת���ؿ�ѡ�����

	//����
	bool					m_bIsBossMisson;	// �Ƿ�BOSS��սģʽ
	int						m_nMission;			// �ؿ����
	int						m_nDifficulty;		// �ؿ��Ѷ� 0:easy 1:normal 2:hard
	int						m_nDefaultPlane;	// Ĭ�Ϸɻ�
	bool					m_bWin;				// �Ƿ�ɹ���ս
	

	//����
	Sprite*					pBg_1;
	Sprite*					pBg_2;
	Sprite*					pBg_3;
	Sprite*					pBg_4;
	Sprite*					pBg_5;

	//����
	Sprite*					pAnimate_1_1;
	Sprite*					pAnimate_1_2;
	Sprite*					pAnimate_2_1;
	Sprite*					pAnimate_4_1;
	Sprite*					pAnimate_5_1;
	Sprite*					pAnimate_5_2;

	//����
	ProgressTimer*			pTimer;
	ProgressTimer*			pTimer21;
	ProgressTimer*			pTimer22;
	ProgressTimer*			pTimer31;
	ProgressTimer*			pTimer32;
	ProgressTimer*			pTimer4;
	ProgressTimer*			pTimer5;

	//����
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

	virtual void	onKeyReleased(EventKeyboard::KeyCode keyCode,Event * pEvent); //������Ӧ

private:

};
