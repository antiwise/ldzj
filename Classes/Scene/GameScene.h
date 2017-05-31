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

struct BatchInfo   //�л�����������Ϣ
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

struct EnemyInfo //�л�������Ϣ
{
	float		createTime;	//����ʱ��
	int			model;
	std::string action;		//��Ϊ����
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

	// ��Ϸ������ʼ��	ս��ID,�ؿ�����,�ؿ�,�Ѷ�
	void	InitGame(int planeID,bool IsBossMisson, int minssionID, int difficulty);
	//	����
	void	CleanGame();

	// ��ѭ��
	virtual void Tick(float dt);

	// �������� --- ���㴥��
	void		OpenListener();
	void		CloseListener();

	// �����ص�
	virtual bool onTouchBegan(Touch *touch, Event *unused_event); 
	virtual void onTouchMoved(Touch *touch, Event *unused_event); 
	virtual void onTouchEnded(Touch *touch, Event *unused_event); 
	virtual void onTouchCancelled(Touch *touch, Event *unused_event);

	// �����ӳٻص�
	void DelayTimeCallback();

	//�ӿں���
	void		GamePause() ;				// ��ͣ
	void		GameContinue();				// ����

	void		GamePauseAndNoSuspendLayer() ;		// ��ͣ��û����ͣҳ�棩
	void		GameContinueAndNoSuspendLayer();	// ������û����ͣҳ�棩

	void		GameGiveUp();				// �����ؿ�
	void		GameGoToSelectMission();	// ���عؿ�ѡ�����
	void		GameGoToIntensify();		// ����ǿ������


	void		SetBossBloodBar(float persent);
	
	// ��ȡ�ڵ�
	Node*		GetNodeByZOrder(int zorder);

	// Ԥ���س�������
	void		InitGameObject();

	// ������ҷɻ�
	void		ClearPlayerPlane();

	// �������
	void		PlayerPlaneRenascence();

	// �ɻ����ܴ���
	void		PlayerPlaneShieldEffect();

	// �Ƿ���ͣ
	bool		IsPause() { return m_bGamePause; }

	/*---------------------------------------------------------------------*/
	// ������������
	void		OpenRankUpLayer();

	/*---------------------------------------------------------------------*/
	// �ؿ�ͳ�ƽ���
	void		ShowPassCountInfo();
	
	/*---------------------------------------------------------------------*/
	// ս������UI
	void		StopClickEventWithBattleLayerButton();			// ֹͣս�������еİ�ť����¼�
	void		StartClickEventWithBattleLayerButton();			// ��ʼս�������еİ�ť����¼�


private:

	void MovePlane(Vec2& deltaMove);
	
	// ���عؿ��л�������Ϣ
	void	InitBatchInfo(bool IsBossMisson, int minssionID, int difficulty);

	//�л����μ����߼�
	void	EnemyBatchTick(float dt);						

	//���عؿ��л���������
	int		LoadEnemyBatchInfo(const std::string& fileName);

private:
	std::vector<Layer*>		m_LayerList;

	//����
	bool					m_bGamePause;		// ��Ϸ��ͣ
	Vector<Node*>			m_vPauseNodes;		// ��ͣ�б�

	//UI
	MapLayer*				m_pMapLayer;		// ��ͼ
	SuspendLayer*			m_pSuspendLayer;	// ��ͣ
	BattleLayer*			m_pBattleLayer;		// ս��
	PassCountLayer*			m_pPassCountLayer;	// ͨ�ؽ���
	RankUp*					m_pRankUpLayer;		// ������������

	//���
	LXPlayerPlane*			m_pPlayer;			// ս��

	//�л���������
	std::vector<BatchInfo>	m_vBatchInfo;		// �ɻ���������
	std::vector<EnemyInfo>	m_vEnemyInfo;		// �ɻ�������Ϣ
	float					m_fPassTime;		// ��ʧʱ��
	int						m_nBatchCount;		// ��ǰ�л�����
	float					m_fDistance;		// ��ǰ�ƶ��߼�����

	float					m_fPreRunTime;		// ս����ʼǰʱ��
	float					m_fAfterRunTime;	// ս��������ʱ��
	int						m_nMission;			// ��ǰ�ؿ�

	bool					m_IsPlayMissionStartStory;		// �Ƿ񲥷Źؿ���ʼ����
	bool					m_IsPlayMissionEndStory;		// �Ƿ񲥷Źؿ���������

	//LXAnimationLogic		*m_pTestLogic;
};
