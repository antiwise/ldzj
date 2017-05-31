#include "TaskOptionMgr.h"
#include "DB/DBManager.h"
#include "DataBase/DataBase.h"
#include "SDK/SDKManager.h"
#include "Scene/MainScene.h"
#include "cocos2d.h"

USING_NS_CC;

TaskOptionMgr& TaskMgr()
{
	static TaskOptionMgr mgr;
	return mgr;
}

TaskOptionMgr::TaskOptionMgr(void)
{
	m_RewardNum = 0;
}


TaskOptionMgr::~TaskOptionMgr(void)
{
	for ( auto it : m_TaskOption )
	{
		delete it.second;
	}
	m_TaskOption.clear();
}

void TaskOptionMgr::CreatTaskOption()
{

	DBC_DEFINEHANDLE(s_pAchieveDBC, DBC_ACHIEVE);

	for (int i = 0; i < TASK_COUNT; i++)
	{
		int temp  = DBManager().GetTaskOptionStateByIndex(i);

		if ( temp == -1 )
		{
			return ;
		}

		TASK_STATE _eState = static_cast<TASK_STATE>( temp );

		if ( _eState == TASK_STATE::TASK_STATE_CLOSE )
		{
			continue;
		}

		const _DBC_ACHIEVE *pAchieve = (const _DBC_ACHIEVE*)s_pAchieveDBC->Search_LineNum_EQU(i);
		if (!pAchieve)
		{
			return ;
		}

		TaskOption* _Task = new TaskOption();
		if ( _Task )
		{
			_Task->m_nTaskID = pAchieve->nID;
			_Task->m_eTaskState = _eState;
			_Task->m_eRewardType = static_cast<REWARD_TYPE>(pAchieve->nType);
			_Task->m_nRewardNum = pAchieve->nNum;
		}

		m_TaskOption[ pAchieve->nID ] = _Task;
	}

	// 计算可领取的数量
	m_RewardNum = CalcRewardNum();
}

void TaskOptionMgr::DoneTaskOption(int nId)
{
	if ( m_TaskOption.find(nId) == m_TaskOption.end() )
	{
		return ;
	}

	if ( m_TaskOption[nId] && m_TaskOption[nId]->m_eTaskState == TASK_STATE::TASK_STATE_NORMAL )
	{
		m_TaskOption[nId]->m_eTaskState = TASK_STATE::TASK_STATE_REWARD;
		DBManager().SetTaskOptionState( nId-1, static_cast<int>(TASK_STATE::TASK_STATE_REWARD) );
		
		// 刷新提示
		DBC_DEFINEHANDLE(s_pAchieveDBC, DBC_ACHIEVE);
		const _DBC_ACHIEVE *pAchieve = (const _DBC_ACHIEVE*)s_pAchieveDBC->Search_Index_EQU( nId );
		if ( pAchieve )
		{
			dynamic_cast<MainScene*>( Director::getInstance()->getRunningScene()->getChildByName("mainSence") )->TaskTipsShow( pAchieve->szName );
		}

		// 计算可领取的数量
		m_RewardNum = CalcRewardNum();
	}
}

void TaskOptionMgr::Reward(int nId)
{
	if ( m_TaskOption[nId] )
	{
		switch ( m_TaskOption[nId]->m_eRewardType )
		{
		case REWARD_TYPE::REWARD_DIAMOND:
			{
				//SDKManager().bonus(m_TaskOption[nId]->m_nRewardNum, bonus_coin_mission);
				DBManager().AddData(DATA_DIAMOND_NUM,m_TaskOption[nId]->m_nRewardNum);
				DBManager().AddData(DATA_DIAMOND_TOTAL,m_TaskOption[nId]->m_nRewardNum);
			}
			break;
		case REWARD_TYPE::REWARD_SHIELD:
			{
				DBManager().AddData(DATA_SHIELD_NUM,m_TaskOption[nId]->m_nRewardNum);
			}
			break;
		case REWARD_TYPE::REWARD_SKILL:
			{
				DBManager().AddData(DATA_BOMB_NUM,m_TaskOption[nId]->m_nRewardNum);
			}
			break;
		default:
			{
				log(" this taskOption reward type:%d error of id:%d \n",static_cast<int>( m_TaskOption[nId]->m_eRewardType), nId );
				return ;
			}
			break;
		}

		// 设置状态
		m_TaskOption[nId]->m_eTaskState = TASK_STATE::TASK_STATE_CLOSE;
		DBManager().SetTaskOptionState( nId-1, static_cast<int>(TASK_STATE::TASK_STATE_CLOSE) );

		// 计算可领取的数量
		m_RewardNum = CalcRewardNum();
	}
}

int TaskOptionMgr::CalcRewardNum()
{
	int nNum = 0;

	for ( auto it : m_TaskOption )
	{
		if ( it.second && it.second->m_eTaskState == TASK_STATE::TASK_STATE_REWARD )
		{
			++ nNum ;
		}
	}

	return nNum;
}

TaskOption* TaskOptionMgr::GetTaskOption(int nId)
{
	return m_TaskOption.at( nId );
}