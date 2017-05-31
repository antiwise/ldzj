
/*
==
Purpose: 任务管理器 

note:
*/

#pragma once
#include "cocos2d.h"
#include "GameDefine.h"

USING_NS_CC;


enum TASK_STATE
{
	TASK_STATE_NORMAL,	//正常
	TASK_STATE_REWARD,	//可领奖
	TASK_STATE_CLOSE	//结束
};


enum REWARD_TYPE
{

	REWARD_DIAMOND			= 0,	// 宝石

	REWARD_SHIELD			= 3,	// 护盾

	REWARD_SKILL			= 5		//  必杀

};




struct TaskOption
{
	int			m_nTaskID;			// ID
	int			m_nRewardNum;		// 奖励数量
	TASK_STATE	m_eTaskState;		// 状态
	REWARD_TYPE m_eRewardType;		// 奖励类型 

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

	int		m_RewardNum;			// 可领取的数量

};

TaskOptionMgr& TaskMgr();