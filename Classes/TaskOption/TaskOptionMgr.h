
/*
==
Purpose: ��������� 

note:
*/

#pragma once
#include "cocos2d.h"
#include "GameDefine.h"

USING_NS_CC;


enum TASK_STATE
{
	TASK_STATE_NORMAL,	//����
	TASK_STATE_REWARD,	//���콱
	TASK_STATE_CLOSE	//����
};


enum REWARD_TYPE
{

	REWARD_DIAMOND			= 0,	// ��ʯ

	REWARD_SHIELD			= 3,	// ����

	REWARD_SKILL			= 5		//  ��ɱ

};




struct TaskOption
{
	int			m_nTaskID;			// ID
	int			m_nRewardNum;		// ��������
	TASK_STATE	m_eTaskState;		// ״̬
	REWARD_TYPE m_eRewardType;		// �������� 

	TaskOption()
	{
		m_nTaskID = -1;
		m_nRewardNum = 0;
		m_eTaskState = TASK_STATE::TASK_STATE_NORMAL;
		m_eRewardType = REWARD_TYPE::REWARD_DIAMOND;
	}
};


class TaskOptionMgr
{
public:
	TaskOptionMgr(void);
	~TaskOptionMgr(void);

	void		CreatTaskOption();
	
	TaskOption*	GetTaskOption(int nId);

	void		DoneTaskOption(int nId);

	void		Reward(int nId);

	int			CalcRewardNum();

	int&		GetRewardNum(){ return m_RewardNum; }

	std::map<int,TaskOption*>&		GetTaskOptionMap(){ return m_TaskOption;}


private:

	std::map<int,TaskOption*> m_TaskOption;

	int		m_RewardNum;			// ����ȡ������

};

TaskOptionMgr& TaskMgr();