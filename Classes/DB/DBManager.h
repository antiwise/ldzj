#pragma once
#include <map>
#include "cocos2d.h"
#include "GameDefine.h"

class LXDBManager
{
public:
	LXDBManager(void);
	~LXDBManager(void);

public:
	// 存盘数据加载
	void					LoadDB();
	void					SaveDB();

public:

	int					GetData(int datatype);
	void				SetData(int datatype, int nValue);
	void				AddData(int datatype, int addValue);

	// 是否是boss模式
	void				SetBossMission(bool bBossMisson){ m_IsBossMission = bBossMisson; };
	bool				IsBossMission(){ return m_IsBossMission; }

	// 挑战关卡上升
	void				NextMissonRise();		//待挑战普通关
	void				NextBossMissonRise();	//待挑战BOSS关

	// 当前关卡
	void				SetCurrMission(int nID);
	int					GetCurrMission(){ return m_CurrMission; }

	// 当前关卡难易程度
	void				SetCurrMissionLevel(int nLevel){ m_CurrMissionLeve = nLevel; };
	int					GetCurrMissionLevel(){ return m_CurrMissionLeve; }
	
	// 飞机实时等级	
	void				SetPlaneLevel(int value) { m_nPlaneLevel = value; }
	int					GetPlaneLevel() { return m_nPlaneLevel; }
	
	// 主武器等级(初始等级)
	int					GetPlaneWeaponLevel(){ return (1 + std::max(0, std::min(5, m_DBData[DATA_FIRE_LEVEL]))) / 2; };

	// 主机武器攻击力	
	int					GetPlaneAttack() { return 100 *( 100 + m_nPlaneAttack ) / 100 ; }
	
	// 僚机数量
	int					GetWingNum() { return 2 * ((1 + std::max(0, std::min(5, m_DBData[DATA_WING_LEVEL]))) / 2);}
	
	// 僚机攻击力
	int					GetWingAttack() { return 50 * (std::max(0,std::min(5,m_DBData[DATA_WING_LEVEL]))); }
	
	// 暴走时间计算
	int					GetCrazyTime() { return 3 + m_DBData[DATA_CRAZY_LEVEL];}

	// 护盾时间计算
	int					GetProtectNum() { return  m_DBData[DATA_SHIELD_LEVEL]; }
	
	// 自爆战机持续时间
	int					GetBombTime() { return 100 + 20 * m_DBData[DATA_BOMB_LEVEL];}


	// 玩家姓名
	void				SetPlayerName(std::string &name){ m_PlayerName = name; }
	std::string&		GetPlayerName(){return m_PlayerName;}

	// 当前选择的飞机的id ( 飞机类型 )
	void				SetCurrPlaneID(int nId){ m_CurrPlaneID = nId; }
	int					GetCurrPlaneID(){ return m_CurrPlaneID; }

	//关卡临时数据
	void				SetMissionDiamond(int nNum){ m_nMissDiamond = nNum; }
	int					GetMissionDiamond(){ return m_nMissDiamond; }
	void				SetMissionScore(int nNum){ m_nMissScore = nNum; }
	int					GetMissionScore(){ return m_nMissScore; }
	void				SetMissionKilled(int val){ m_nMissKilled = val; }
	int					GetMissionKilled(){return m_nMissKilled;}
	void				SetMissionShield(int val){ m_nMissSheild = val; };
	int					GetMissionShield(){ return m_nMissSheild;}
	void				SetMissionLife(int val){ m_nMissLife = val; }
	int					GetMissionLife(){ return m_nMissLife; }
	void				SetMissionBomb(int val){ m_nMissBomb = val; }
	int					GetMissionBomb(){ return m_nMissBomb; }

	void				AddDiamondByCrystal(int nScore);

	void				AddMissionKilled();
	void				AddMissionScore(int val);
	void				AddMissionDiamon(int nNum);
	void				AddDieTimes();
	
	void				AddMissionShield(int val = 1);
	void				AddMissionLife(int val = 1);
	void				AddMissionBomb(int val = 1);
	bool				UseMissionShield(int val = 1);
	bool				UseMissionLife(int val = 1);
	bool				UseMissionBomb(int val = 1);


public:

	bool				IsCommonMission();		//  true 表示不是 （ 最新关卡 和 boss关卡）
	bool				SetTaskOptionState(int _ID, int _Type);
	int*				GetTaskOptionState(){ return m_TaskOptionState; };
	int					GetTaskOptionStateByIndex(int _Idx);

	void				InitMissonData();			//初始化关卡数据
	void				SaveMissonDataToDBData();	//保存关卡数据(仅在胜利后结算)


	


private:
	// 获取主机的攻击力
	void				calcPlayerData();
	
private:

	bool				m_IsBossMission;		// 是否是Boss关卡
	int					m_CurrMission;			// 当前关卡
	int					m_CurrMissionLeve;		// 当前关卡的难易等级
	int					m_nPlaneLevel;			// 主机武器等级	
	int					m_nPlaneAttack;			// 主机武器攻击力
	int					m_nWingAttack;			// 僚机攻击力

	// 当前选择的飞机
	int					m_CurrPlaneID;			// 当前选择的飞机id
	int					m_CurrPlaneType;		// 当前选择的飞机类型

	std::string			m_PlayerName;			// 玩家名

	//DB数据
	int					m_DBData[DATA_MAX];				// 玩家数据
	int					m_TaskOptionState[TASK_COUNT];	// 任务状态

	//关卡临时数据
	int					m_nMissDieTimes;	// 死亡次数
	int					m_nMissKilled;		// 击杀数量
	int					m_nMissLife;		// 生命
	int					m_nMissSheild;		// 护盾
	int					m_nMissBomb;		// 炸弹
	int					m_nMissScore;		// 积分
	int					m_nMissDiamond;		// 宝石
	int					m_nMissDiamondOdd;  // 宝石零头
};

LXDBManager &DBManager();

