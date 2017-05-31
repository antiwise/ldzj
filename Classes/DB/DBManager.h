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
	// �������ݼ���
	void					LoadDB();
	void					SaveDB();

public:

	int					GetData(int datatype);
	void				SetData(int datatype, int nValue);
	void				AddData(int datatype, int addValue);

	// �Ƿ���bossģʽ
	void				SetBossMission(bool bBossMisson){ m_IsBossMission = bBossMisson; };
	bool				IsBossMission(){ return m_IsBossMission; }

	// ��ս�ؿ�����
	void				NextMissonRise();		//����ս��ͨ��
	void				NextBossMissonRise();	//����սBOSS��

	// ��ǰ�ؿ�
	void				SetCurrMission(int nID);
	int					GetCurrMission(){ return m_CurrMission; }

	// ��ǰ�ؿ����׳̶�
	void				SetCurrMissionLevel(int nLevel){ m_CurrMissionLeve = nLevel; };
	int					GetCurrMissionLevel(){ return m_CurrMissionLeve; }
	
	// �ɻ�ʵʱ�ȼ�	
	void				SetPlaneLevel(int value) { m_nPlaneLevel = value; }
	int					GetPlaneLevel() { return m_nPlaneLevel; }
	
	// �������ȼ�(��ʼ�ȼ�)
	int					GetPlaneWeaponLevel(){ return (1 + std::max(0, std::min(5, m_DBData[DATA_FIRE_LEVEL]))) / 2; };

	// ��������������	
	int					GetPlaneAttack() { return 100 *( 100 + m_nPlaneAttack ) / 100 ; }
	
	// �Ż�����
	int					GetWingNum() { return 2 * ((1 + std::max(0, std::min(5, m_DBData[DATA_WING_LEVEL]))) / 2);}
	
	// �Ż�������
	int					GetWingAttack() { return 50 * (std::max(0,std::min(5,m_DBData[DATA_WING_LEVEL]))); }
	
	// ����ʱ�����
	int					GetCrazyTime() { return 3 + m_DBData[DATA_CRAZY_LEVEL];}

	// ����ʱ�����
	int					GetProtectNum() { return  m_DBData[DATA_SHIELD_LEVEL]; }
	
	// �Ա�ս������ʱ��
	int					GetBombTime() { return 100 + 20 * m_DBData[DATA_BOMB_LEVEL];}


	// �������
	void				SetPlayerName(std::string &name){ m_PlayerName = name; }
	std::string&		GetPlayerName(){return m_PlayerName;}

	// ��ǰѡ��ķɻ���id ( �ɻ����� )
	void				SetCurrPlaneID(int nId){ m_CurrPlaneID = nId; }
	int					GetCurrPlaneID(){ return m_CurrPlaneID; }

	//�ؿ���ʱ����
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

	bool				IsCommonMission();		//  true ��ʾ���� �� ���¹ؿ� �� boss�ؿ���
	bool				SetTaskOptionState(int _ID, int _Type);
	int*				GetTaskOptionState(){ return m_TaskOptionState; };
	int					GetTaskOptionStateByIndex(int _Idx);

	void				InitMissonData();			//��ʼ���ؿ�����
	void				SaveMissonDataToDBData();	//����ؿ�����(����ʤ�������)


	


private:
	// ��ȡ�����Ĺ�����
	void				calcPlayerData();
	
private:

	bool				m_IsBossMission;		// �Ƿ���Boss�ؿ�
	int					m_CurrMission;			// ��ǰ�ؿ�
	int					m_CurrMissionLeve;		// ��ǰ�ؿ������׵ȼ�
	int					m_nPlaneLevel;			// ���������ȼ�	
	int					m_nPlaneAttack;			// ��������������
	int					m_nWingAttack;			// �Ż�������

	// ��ǰѡ��ķɻ�
	int					m_CurrPlaneID;			// ��ǰѡ��ķɻ�id
	int					m_CurrPlaneType;		// ��ǰѡ��ķɻ�����

	std::string			m_PlayerName;			// �����

	//DB����
	int					m_DBData[DATA_MAX];				// �������
	int					m_TaskOptionState[TASK_COUNT];	// ����״̬

	//�ؿ���ʱ����
	int					m_nMissDieTimes;	// ��������
	int					m_nMissKilled;		// ��ɱ����
	int					m_nMissLife;		// ����
	int					m_nMissSheild;		// ����
	int					m_nMissBomb;		// ը��
	int					m_nMissScore;		// ����
	int					m_nMissDiamond;		// ��ʯ
	int					m_nMissDiamondOdd;  // ��ʯ��ͷ
};

LXDBManager &DBManager();

