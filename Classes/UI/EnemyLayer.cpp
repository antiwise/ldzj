//#include "EnemyLayer.h"
//#include "Common/FileUtils.h"
//#include "Common/LXUtils.h"
//#include "GameDefine.h"
//#include "DataBase/DataBase.h"
//#include "ObjectEx/Plane/LXEnemyPlane.h"
//#include "ObjectEx/Plane/LXBossPlane.h"
//#include "ObjectEx/LXObjectManager.h"
//
//long EnemyLayer::mp_tagId = 0;
//
//EnemyLayer* EnemyLayer::s_EnemyLayer = nullptr;
//
//EnemyLayer::EnemyLayer()
//	:m_fPassTime(0),
//	m_nBatchCount(0),
//	m_fDistance(0.0)
//{
//	m_vecMonter.push_back(10);
//	m_vecMonter.push_back(15);
//	m_vecMonter.push_back(20);
//	m_vecMonter.push_back(25);
//	m_vecMonter.push_back(30);
//
//
//	s_EnemyLayer = this;
//}
//
//EnemyLayer::~EnemyLayer()
//{
//
//}
//
//bool EnemyLayer::init()
//{
//	// 1. super init first
//	if ( !Layer::init() )
//	{
//		return false;
//	}
//
//
//	return true;
//}
//
//void EnemyLayer::Tick(float dt)
//{
//	//for (auto it : m_MapMonster)
//	//{
//	//	it.second -> Tick(dt);
//	//}
//
//	//static int precount = 0;
//
//	////波次信息加载
//	//bool bTimePause = false;
//	//float fDistance = m_fPassTime * 60; //当前移动距离
//	//while (true)
//	//{
//	//	//判断批次是否存在,距离是否达到
//	//	if (m_nBatchCount >= m_vBatchInfo.size() || m_vBatchInfo[m_nBatchCount].pos > fDistance)
//	//	{
//	//		bool bTimePause = false;
//	//		break;
//	//	}
//
//	//	//判断是否是等待前面的敌机全部死亡
//	//	BatchInfo info =  m_vBatchInfo[m_nBatchCount];
//	//	if (info.waitPrev && GetLXObjectManager()->GetEnemyNum() > 0)
//	//	{
//	//		bool bTimePause = true; //飞机逻辑时间停止
//	//		break;
//	//	}
//
//	//	//添加敌机创建信息
//	//	for (int i = 0; i < info.num; i++)
//	//	{
//	//		precount++;
//	//		EnemyInfo enemy;
//
//	//		enemy.model = info.model;
//	//		enemy.createTime = m_fPassTime + info.delay + info.waitTime * i;
//	//		enemy.posX = info.x + info.offX * i;
//	//		enemy.posY = info.y + info.offY * i;
//	//		enemy.action = info.action;
//
//	//		m_vEneyInfo.push_back(enemy);
//	//	}
//
//	//	m_nBatchCount++;
//	//}
//	//log("precount = %d",precount);
//
//	//if (!bTimePause)
//	//{
//	//	m_fPassTime += dt;
//	//}
//	////static int planecount = 0;
//	////实际添加飞机
//	//for (auto ch:m_vEneyInfo)
//	//{
//	//	if (ch.createTime <= m_fPassTime )
//	//	{
//	//		//planecount ++;
//	//		//log("planecount = %d",planecount);
//	//		// 加载怪物信息
//	//		DBC_DEFINEHANDLE(s_pEnemyDBC, DBC_ENEMY);
//	//		const _DBC_ENEMY	*m_pEnemyData = (const _DBC_ENEMY*)s_pEnemyDBC->Search_LineNum_EQU(ch.model);
//	//		
//	//		//判断飞机类型
//	//		if (m_pEnemyData->nType == enemy_type_monster)
//	//		{	
//	//			auto pObject = new LXEnemyPlane();
//	//			pObject->Init(ch.model);
//	//			pObject->InitAction(ch.action);
//
//	//			pObject->setPosition(ch.posX-100,DesignHight - ch.posY);
//
//	//			this->addChild(pObject,1000,GetLXObjectManager()->GetTag());
//
//	//			GetLXObjectManager()->AddPlane(pObject);
//
//	//		}
//	//		else if (m_pEnemyData->nType == enemy_type_boss)
//	//		{
//	//			auto pObject = new LXBossPlane();
//	//			pObject->Init(ch.model);
//	//			pObject->InitAction(ch.action);
//
//	//			pObject->setPosition(ch.posX,DesignHight - ch.posY);
//	//			GetLXObjectManager()->AddPlane(pObject);
//	//			this->addChild(pObject);
//	//		}
//	//		
//	//	}
//	//}
//	//bool goon = true;
//	//while (goon)
//	//{
//	//	goon = false;
//	//	for (auto it = m_vEneyInfo.begin(); it != m_vEneyInfo.end(); it++)
//	//	{
//	//		if (it->createTime <= m_fPassTime)
//	//		{
//	//			m_vEneyInfo.erase(it);
//	//			goon = true;
//	//		}
//	//			
//	//		break;
//	//	}
//
//	//}
//
//	
//}
//
//void EnemyLayer::AddMonsterPlane(MonsterPlane* pMonster)
//{
//	//if (pMonster)
//	//{
//	//	this->addChild(pMonster);
//	//	m_MapMonster[pMonster->GetMonsterTagId()] = pMonster;
//	//}
//}
//
//MonsterPlane* EnemyLayer::GetMonsterPlaneByTagId(long tagId)
//{
//	auto it = m_MapMonster.find(tagId);
//	if (it != m_MapMonster.end())
//	{
//		return it->second;
//	}
//
//	return nullptr;
//}
//
//void EnemyLayer::RemoveMonsterPlane(MonsterPlane* pMonster)
//{
//	auto it = m_MapMonster.find(pMonster->GetMonsterTagId());
//	if (it != m_MapMonster.end())
//	{
//		m_MapMonster.erase(it);
//	}
//}
//
//void EnemyLayer::RemoveVector(int num)
//{
//	auto it = m_vecMonter.begin();
//	for (it;it!=m_vecMonter.end();)
//	{
//		if ( (*it) == num)
//		{
//			it = m_vecMonter.erase(it);
//
//		}else
//		{
//			it++;
//		}
//	}
//}
//
//void	EnemyLayer::SetMissionID(int ID)
//{
//	//std::string fullpath = LXFileUtils::GetMapFullPath("01.map");
//	//int count = LoadEnemyInfo(fullpath);
//	//log("%d",count);
//}
//
////int  EnemyLayer::LoadEnemyInfo(const std::string fileName)
////{
////	////文件是否存在
////	//std::istringstream ifs(FileUtils::getInstance()->getStringFromFile(fileName));
////	//if (!ifs)
////	//{
////	//	log("Cannot open file [%s]",fileName.c_str());
////	//	return -1;
////	//}
////
////	//int maxchars = 8192;  // Alloc enough size.
////	//std::vector<char> buf(maxchars);  // Alloc enough size.
////
////	//BatchInfo info;
////	//bool	IsReadingState = false; //是否在读取状态
////	//while (ifs.peek() != -1)
////	//{
////	//	ifs.getline(&buf[0], maxchars);
////
////	//	std::string linebuf(&buf[0]);
////
////	//	// Trim newline '\r\n' or '\r'
////	//	if (linebuf.size() > 0)
////	//	{
////	//		if (linebuf[linebuf.size()-1] == '\n') linebuf.erase(linebuf.size()-1);
////	//	}
////	//	if (linebuf.size() > 0)
////	//	{
////	//		if (linebuf[linebuf.size()-1] == '\r') linebuf.erase(linebuf.size()-1);
////	//	}
////
////	//	// Skip if empty line.
////	//	if (linebuf.empty())
////	//	{
////	//		continue;
////	//	}
////
////	//	// 空行
////	//	if (linebuf.at(0) == '\0')
////	//	{
////	//		continue;
////	//	}
////	//	
////	//	// 注释行
////	//	if (linebuf.at(0) == '[')
////	//	{
////	//		continue;
////	//	}
////
////	//	// 结束
////	//	int pos1=linebuf.find("- end -");
////	//	if (pos1 >=0)
////	//	{
////	//		//模块结束
////	//		if (IsReadingState)
////	//		{
////	//			m_vBatchInfo.push_back(info);
////	//		}
////
////	//		IsReadingState = false;
////	//		continue;
////	//	}
////
////	//	// 
////	//	int  pos = linebuf.find(' = ');
////	//	if (pos <= 0)
////	//	{
////	//		continue;
////	//	}
////
////	//	std::string name = linebuf.substr(0,pos);
////	//	std::string val = linebuf.substr(pos+3);
////	//	if (name == "pos")
////	//	{
////	//		//模块开始
////	//		info.Clean();
////	//		IsReadingState = true;
////
////	//		info.pos = std::atoi(val.c_str());
////	//		continue;
////	//	}
////
////	//	if (IsReadingState) //模块开启标识
////	//	{
////	//		if (name == "x")
////	//		{
////	//			info.x = std::atoi(val.c_str());
////	//			continue;
////	//		}
////
////	//		if (name == "y")
////	//		{
////	//			info.y = std::atoi(val.c_str());
////	//			continue;
////	//		}
////
////	//		if (name == "model")
////	//		{
////	//			info.model = std::atoi(val.c_str());
////	//			continue;
////	//		}
////
////	//		if (name == "num")
////	//		{
////	//			info.num = std::atoi(val.c_str());
////	//			continue;
////	//		}
////
////	//		if (name == "waitPrev")
////	//		{
////	//			if (val == "true")
////	//			{
////	//				info.waitPrev = true;
////	//			}
////	//			else
////	//			{
////	//				info.waitPrev = false;
////	//			}
////	//			continue;
////	//		}
////
////	//		if (name == "delay")
////	//		{
////	//			info.delay = std::atof(val.c_str());
////	//			continue;
////	//		}
////
////	//		if (name == "action")
////	//		{
////	//			info.action = val;
////	//			continue;
////	//		}
////
////	//		if (name == "offX")
////	//		{
////	//			info.offX = std::atoi(val.c_str());
////	//			continue;
////	//		}
////
////	//		if (name == "offY")
////	//		{
////	//			info.offY = std::atoi(val.c_str());
////	//			continue;
////	//		}
////
////	//		if (name == "waitTime")
////	//		{
////	//			info.waitTime = std::atof(val.c_str());
////	//			continue;
////	//		}
////	//	}
////
////	//}
////
////	//return m_vBatchInfo.size();
////}