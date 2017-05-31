#include "DBManager.h"
#include <string>
#include "Common/FileUtils.h"
#include "TaskOption/TaskOptionMgr.h"
#include "Common/FileUtils.h"
#include "Sound/LXSoundSystem.h"
#include "SDK/SDKManager.h"
#include "DataBase/DataBase.h"
#include "deprecated/CCString.h"

USING_NS_CC;

const std::string strDBName = "lx.db";
const int	DATA_FORMAT = 1;

LXDBManager &DBManager()
{
	static LXDBManager s;
	return s;
}

LXDBManager::LXDBManager(void)
	:m_IsBossMission(false),
	m_CurrMission(1),
	m_CurrMissionLeve(1),
	m_nPlaneLevel(0),		
	m_nPlaneAttack(0),
	m_nWingAttack(0),
	m_CurrPlaneID(0),
	m_CurrPlaneType(0)
{
	m_PlayerName = "Player";

	memset(m_TaskOptionState, 0, TASK_COUNT);
	//默认数据
	for (int i=0; i<DATA_MAX; i++)
	{
		m_DBData[i] = 0;
	}

	m_DBData[DATA_LIFE_NUM] = 2;
	m_DBData[DATA_NEXT_MISSION] = 1;
	m_DBData[DATA_WORLD_RANK] = 5001;
	m_DBData[DATA_GAME_WEEK] = 1;
	m_DBData[DATA_SHIELD_NUM] = 2;
	m_DBData[DATA_BOMB_NUM] = 2;
	m_DBData[DATA_NEXT_BOSS] = 1;
	m_DBData[DATA_SHOW_STORY] = 1;
	m_DBData[DATA_MUSIC_ON] = 1;
	m_DBData[DATA_EFFECT_ON] = 1;

	//m_DBData[DATA_DIAMOND_LEVEL]	= 5;
	//m_DBData[DATA_WING_LEVEL]		= 5;
	//m_DBData[DATA_FIRE_LEVEL]		= 5;
	//m_DBData[DATA_CRAZY_LEVEL]		= 5;
	//m_DBData[DATA_SHIELD_LEVEL]		= 5;
	//m_DBData[DATA_BOMB_LEVEL]		= 5;

	//测试功能
	//m_DBData[DATA_BOMB_NUM] = 2;
	//m_DBData[DATA_SHIELD_NUM] = 2;
	//m_DBData[DATA_NEXT_MISSION] = 12;
	//m_DBData[DATA_NEXT_BOSS] = 12;

}

LXDBManager::~LXDBManager(void)
{
}

void LXDBManager::LoadDB()
{
	auto pFileUtils = FileUtils::getInstance();
	//if (pFileUtils->isFileExist(strDBName))
	std::string strDB = LXFileUtils::GetFileByName(strDBName);
	if(strDB.length()>0)
	{
		//使用文件数据初始化
		//std::string strDB = FileUtils::getInstance()->getStringFromFile(strDBName);
		
		std::vector<std::string> vData;
		int num = LXFileUtils::ConvertStringToVector(strDB.c_str(),vData,":");
		
		const int dataformat = std::atoi(vData[0].c_str());//std::stoi(vData[0]);

		if (dataformat == 1) //默认格式
		{
			//基础数据
			for (int i = 0; i < DATA_MAX; i++)
			{
				m_DBData[i] = std::atoi(vData[i + 1].c_str()); //std::stoi(vData[i + 1]);
			}

			//成就
			for (int i = 0; i < TASK_COUNT; i++)
			{
				m_TaskOptionState[i] = std::atoi(vData[DATA_MAX + i + 1].c_str()); //std::stoi(vData[DATA_MAX + i + 1]);
			}
			
			//名称
			m_PlayerName = vData[DATA_MAX + TASK_COUNT + 1];
		}
		else
		{
			SaveDB();
		}
	}
	else
	{
		//使用默认数据初始化,并创建文件
		SaveDB();
	}
	
	
	calcPlayerData();

	//初始化音乐设置
	SoundSystem().setBackgroundMusicState(m_DBData[DATA_MUSIC_ON]);
	SoundSystem().setEffectState(m_DBData[DATA_EFFECT_ON]);
}

void LXDBManager::SaveDB()
{
	std::string strContent = StringUtils::toString(DATA_FORMAT) + ":";// StringUtils::toString(DATA_FORMAT) + ":"; //默认数据读取格式

	//游戏数据
	for (int i = 0; i < DATA_MAX; i++)
	{
		strContent += StringUtils::toString(m_DBData[i]) + ":";// StringUtils::toString(m_DBData[i]) + ":";
	}

	//任务数据
	for (int i = 0; i < TASK_COUNT; i++)
	{
		strContent += StringUtils::toString(m_TaskOptionState[i]) + ":";//StringUtils::toString(m_TaskOptionState[i]) + ":";
	}

	//玩家名称
	strContent += m_PlayerName ;

	//
	LXFileUtils::SaveFile(strContent.c_str(),strDBName);
}

int	LXDBManager::GetData(int datatype)
{
	if (datatype >= 0 && datatype < DATA_MAX)
	{
		return m_DBData[datatype];
	}

	return -1;
}

void LXDBManager::SetData(int datatype, int nValue)
{
	if (datatype >= 0 && datatype < DATA_MAX)
	{
		m_DBData[datatype] = nValue;

		//成就判断
		switch (datatype)
		{
		case DATA_DIAMOND_LEVEL:
			if ( m_DBData[DATA_DIAMOND_LEVEL] >= 5 )
			{
				TaskMgr().DoneTaskOption(18);
			}
			break;

		case DATA_WING_LEVEL:
			if (  m_DBData[DATA_WING_LEVEL] >= 5  )
			{
				TaskMgr().DoneTaskOption(19);
			}
			break;
		case DATA_FIRE_LEVEL:
			if ( m_DBData[DATA_FIRE_LEVEL] >= 5  )
			{
				TaskMgr().DoneTaskOption(20);
			}
			break;
		case DATA_CRAZY_LEVEL:
			if (  m_DBData[DATA_CRAZY_LEVEL] >= 5  )
			{
				TaskMgr().DoneTaskOption(21);
			}
			break;
		case DATA_SHIELD_LEVEL:
			if ( m_DBData[DATA_SHIELD_LEVEL] >= 5  )
			{
				TaskMgr().DoneTaskOption(22);
			}
			break;
		case DATA_BOMB_LEVEL:
			if ( m_DBData[DATA_BOMB_LEVEL] >= 5  )
			{
				TaskMgr().DoneTaskOption(23);
			}
			break;
		case DATA_WORLD_RANK:
			{
				const int nRank = m_DBData[DATA_WORLD_RANK];
				if ( nRank < 5000)
				{
					TaskMgr().DoneTaskOption(26);
				}
				if ( nRank < 4000)
				{
					TaskMgr().DoneTaskOption(27);
				}
				if ( nRank < 3000)
				{
					TaskMgr().DoneTaskOption(28);
				}
				if ( nRank < 2000)
				{
					TaskMgr().DoneTaskOption(29);
				}
				if ( nRank < 1000)
				{
					TaskMgr().DoneTaskOption(30);
				}
				if ( nRank < 500)
				{
					TaskMgr().DoneTaskOption(31);
				}
				if ( nRank < 300)
				{
					TaskMgr().DoneTaskOption(32);
				}
				if ( nRank < 200)
				{
					TaskMgr().DoneTaskOption(33);
				}
				if ( nRank < 100)
				{
					TaskMgr().DoneTaskOption(34);
				}
				if ( nRank < 50)
				{
					TaskMgr().DoneTaskOption(35);
				}
				if ( nRank < 30)
				{
					TaskMgr().DoneTaskOption(36);
				}
				if ( nRank < 10)
				{
					TaskMgr().DoneTaskOption(37);
				}
				if ( nRank < 3)
				{
					TaskMgr().DoneTaskOption(38);
				}
			}
			break;
		case DATA_KILLED_NUM:
			{
				const int killNum = m_DBData[DATA_KILLED_NUM];
				// 这里进行任务逻辑判断
				if ( killNum >= 500 )
				{
					TaskMgr().DoneTaskOption(8);
				}
				if ( killNum >= 1000 )
				{
					TaskMgr().DoneTaskOption(9);
				}
				if ( killNum >= 2000 )
				{
					TaskMgr().DoneTaskOption(10);
				}
			}
			break;
		case DATA_DIAMOND_TOTAL:
			{
				const int maxDiamond= m_DBData[DATA_DIAMOND_TOTAL];
				if ( maxDiamond >= 10000 )
				{
					TaskMgr().DoneTaskOption(11);
				}
				if ( maxDiamond >= 50000 )
				{
					TaskMgr().DoneTaskOption(12);
				}
				if ( maxDiamond >= 100000 )
				{
					TaskMgr().DoneTaskOption(13);
				}
			}
		case DATA_TOP_SCORE:
			{
				const int maxscore = m_DBData[DATA_TOP_SCORE];
				// 这里进行任务逻辑判断
				if ( maxscore >= 100000 )
				{
					TaskMgr().DoneTaskOption(14);
				}
				if ( maxscore >= 200000 )
				{
					TaskMgr().DoneTaskOption(15);
				}
				if ( maxscore >= 500000 )
				{
					TaskMgr().DoneTaskOption(16);
				}
			}
			break;
		default:
			break;
		}

		if ( m_DBData[DATA_BOMB_LEVEL] >= 5 && m_DBData[DATA_SHIELD_LEVEL] >= 5 && m_DBData[DATA_CRAZY_LEVEL] >= 5
			&& m_DBData[DATA_FIRE_LEVEL] >= 5 && m_DBData[DATA_WING_LEVEL] >= 5 && m_DBData[DATA_DIAMOND_LEVEL] >= 5 )
		{
			TaskMgr().DoneTaskOption(24);
		}


		//保存数据
		SaveDB();
	}
}

void LXDBManager::AddData(int datatype, int addValue)
{
	if (datatype >= 0 && datatype < DATA_MAX)
	{
		SetData(datatype, GetData(datatype) + addValue);
	}
}

void LXDBManager::calcPlayerData()
{
	m_nPlaneAttack = 50 * (std::max(0, std::min(5, m_DBData[DATA_FIRE_LEVEL])) / 2);
}

void LXDBManager::NextMissonRise()
{
	DBC_DEFINEHANDLE(s_pParamDBC, DBC_PARAM_INFO);
	const _DBC_PARAM_INFO *pParamInfo = (const _DBC_PARAM_INFO*)s_pParamDBC->Search_Index_EQU(MISSION_COUNT_INDEX);

	int nTempNum = 6;
	if (pParamInfo)
	{
		nTempNum = pParamInfo->getIntValue();
	}

	if ( m_DBData[DATA_NEXT_MISSION] < nTempNum )
	{
		m_DBData[DATA_NEXT_MISSION] += 1;
	}
}

void LXDBManager::NextBossMissonRise()
{
	DBC_DEFINEHANDLE(s_pParamDBC, DBC_PARAM_INFO);
	const _DBC_PARAM_INFO *pParamInfo = (const _DBC_PARAM_INFO*)s_pParamDBC->Search_Index_EQU(MISSION_COUNT_INDEX);

	int nTempNum = 6;
	if (pParamInfo)
	{
		nTempNum = pParamInfo->getIntValue();
	}

	if ( m_DBData[DATA_NEXT_BOSS] < nTempNum )
	{
		m_DBData[DATA_NEXT_BOSS] += 1;
	}
}

bool LXDBManager::IsCommonMission()
{
	return ( ( m_DBData[DATA_NEXT_MISSION] != m_CurrMission ) && ( !m_IsBossMission ) );
}

void LXDBManager::SetCurrMission(int nID)
{
	m_CurrMission = nID;
}

void LXDBManager::AddMissionKilled()
{
	m_nMissKilled ++;
}

void LXDBManager::AddMissionScore(int val)
{
	m_nMissScore += val;
}

void LXDBManager::AddMissionDiamon(int nNum)
{
	m_nMissDiamond += nNum;
}

void LXDBManager::AddDieTimes()
{
	m_nMissDieTimes++;
}

void LXDBManager::AddDiamondByCrystal(int nScore)
{
	int var1 = 10 * nScore * m_DBData[DATA_DIAMOND_LEVEL];
	int nDiamond = nScore + var1/1000;
	m_nMissDiamondOdd +=  var1%1000;		// 保存零头
	if ( m_nMissDiamondOdd >= 1000 )
	{
		++nDiamond;
		m_nMissDiamondOdd -= 1000;
	}

	AddMissionDiamon(nDiamond);
}

void LXDBManager::AddMissionShield(int val /* = 1 */)
{
	m_nMissSheild += val;
}

void LXDBManager::AddMissionLife(int val /* = 1 */)
{
	m_nMissLife += val;
}

void LXDBManager::AddMissionBomb(int val /* = 1 */)
{
	m_nMissBomb += val;
}

bool LXDBManager::UseMissionShield(int val /* = 1 */)
{
	if ( val < 1 )
	{
		return false;
	}
	
	if ( val > m_nMissSheild)
	{
		return false;
	}
	
	m_nMissSheild -= val;

	return true;
}

bool LXDBManager::UseMissionLife(int val /* = 1 */)
{
	if ( val < 1 )
	{
		return false;
	}

	if ( val > m_nMissLife)
	{
		return false;
	}

	m_nMissLife -= val;

	return true;

}

bool LXDBManager::UseMissionBomb(int val /* = 1 */)
{
	if ( val < 1 )
	{
		return false;
	}

	if ( val > m_nMissBomb)
	{
		return false;
	}

	m_nMissBomb -= val;

	return true;
}

bool LXDBManager::SetTaskOptionState(int _ID, int _Type)
{
	if ( _ID >= 0 && _ID < TASK_COUNT )
	{
		m_TaskOptionState[_ID] = _Type;
		SaveDB();
		return true;
	}
	return false;
}

void LXDBManager::InitMissonData()
{
	m_nMissDieTimes		 = 0;
	m_nMissKilled		 = GetData(DATA_KILLED_NUM);
	m_nMissLife			 = GetData(DATA_LIFE_NUM);
	m_nMissSheild		 = GetData(DATA_SHIELD_NUM);
	m_nMissBomb			 = GetData(DATA_BOMB_NUM);
	m_nMissScore		 = GetData(DATA_TOP_SCORE);
	m_nMissDiamond		 = GetData(DATA_DIAMOND_NUM);
	m_nMissDiamondOdd	 = GetData(DATA_DIAMOND_ODD);
}

void LXDBManager::SaveMissonDataToDBData()
{
	// 计算时间
	/*if (GetData(DATA_LIFE_NUM) - m_nMissLife > 0)
	{
	SDKManager().use("Buy_Life", GetData(DATA_LIFE_NUM) - m_nMissLife, 0);
	}
	if (GetData(DATA_SHIELD_NUM) - m_nMissSheild > 0)
	{
	SDKManager().use("Buy_Shield", GetData(DATA_SHIELD_NUM) - m_nMissSheild, 0);
	}
	if (GetData(DATA_BOMB_NUM) - m_nMissBomb > 0)
	{
	SDKManager().use("Buy_Bomb", GetData(DATA_BOMB_NUM) - m_nMissBomb, 0);
	}*/
	//if (GetData(DATA_DIAMOND_NUM) - m_nMissDiamond > 0)
	//{
	//	//SDKManager().bonus(GetData(DATA_DIAMOND_NUM) - m_nMissDiamond, bonus_coin_monster);
	//}

	SetData(DATA_KILLED_NUM, m_nMissKilled);
	SetData(DATA_LIFE_NUM, m_nMissLife);
	SetData(DATA_SHIELD_NUM, m_nMissSheild);
	SetData(DATA_BOMB_NUM, m_nMissBomb);
	SetData(DATA_TOP_SCORE, m_nMissScore);
	SetData(DATA_DIAMOND_NUM,m_nMissDiamond);
	AddData(DATA_DIAMOND_TOTAL,m_nMissDiamond);
	SetData(DATA_DIAMOND_ODD, m_nMissDiamondOdd);

	SaveDB();
}

int LXDBManager::GetTaskOptionStateByIndex(int _Idx)
{
	if (_Idx < 0 || _Idx >= TASK_COUNT )
	{
		return -1;
	}
	
	return m_TaskOptionState[_Idx];
}