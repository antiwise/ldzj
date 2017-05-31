#include "LXObjectManager.h"
#include "ObjectEx/DropItem/LXDropItem.h"
#include "ObjectEx/Plane/LXBombPlane.h"
#include "ObjectEx/Plane/LXBossPlane.h"
#include "ObjectEx/Plane/LXEnemyPlane.h"
#include "ObjectEx/Plane/LXPlayerPlane.h"
#include "ObjectEx/Plane/LXWingPlane.h"
#include "ObjectEx/Weapon/LXBombWeapon.h"
#include "ObjectEx/Weapon/LXCommonWeapon.h"
#include "ObjectEx/Weapon/LXLaserWeapon.h"
#include "ObjectEx/Weapon/LXParentWeapon.h"
#include "ObjectEx/Weapon/LXSpeedWeapon.h"
#include "ObjectEx/Weapon/LXTrackWeapon.h"
#include "Common/OBB.h"
#include "DB/DBManager.h"
#include "Effect/LXEffectManager.h"
#include "Sound/LXSoundSystem.h"
#include "GameDefine.h"
#include "Scene/MainScene.h"
#include "Scene/GameScene.h"
#include "Common/LXUtils.h"
#include "TaskOption/TaskOptionMgr.h"

LXObjectManager &ObjectManager()
{
	static LXObjectManager s;
	return s;
}

LXObjectManager::LXObjectManager()
	: m_pPlayer(nullptr),
	m_duration(0.0f),
	m_lasTime(0.0f),
	m_xparam(0),
	m_yparam(0),
	m_IsSway(false),
	m_pGameSceneNode(nullptr),
	m_pMainSceneNode(nullptr),
	PlayerPlaneNum(0),				
	EnemyPlaneNum(0),
	DropItemNum(0),
	PlayerWeaponNum(0),
	EnemyWeaponNum(0)
{
	for (int i = 0; i < 100; i++)
	{
		m_PlayerPlaneMap[i] = nullptr;
	}
	for (int i = 0; i < 500; i++)
	{
		m_EnemyPlaneMap[i] = nullptr;
	}
	for (int i = 0; i < 1000; i++)
	{
		m_DropItemMap[i] = nullptr;
	}
	for (int i = 0; i < 500; i++)
	{
		m_PlayerWeaponMap[i] = nullptr;
	}
	for (int i = 0; i < 1000; i++)
	{
		m_EnemyWeaponMap[i] = nullptr;
	}
}

LXObjectManager::~LXObjectManager() 
{
	calcData();

	for (int i = 0; i < PlayerPlaneNum; i++)
	{
		if (m_PlayerPlaneMap[i])
		{
			if (m_PlayerPlaneMap[i]->getParent())
			{
				m_PlayerPlaneMap[i]->removeFromParentAndCleanup(true);
			}
			else
			{
				m_PlayerPlaneMap[i]->release();
			}

			m_PlayerPlaneMap[i] = nullptr;
		}
	}
	PlayerPlaneNum = 0;

	for (int i = 0; i < EnemyPlaneNum; i++)
	{
		if (m_EnemyPlaneMap[i])
		{
			if (m_EnemyPlaneMap[i]->getParent())
			{
				m_EnemyPlaneMap[i]->removeFromParentAndCleanup(true);
			}
			else
			{
				m_EnemyPlaneMap[i]->release();
			}

			m_EnemyPlaneMap[i] = nullptr;
		}
	}
	EnemyPlaneNum = 0;

	for (int i = 0; i < DropItemNum; i++)
	{
		if (m_DropItemMap[i])
		{
			if (m_DropItemMap[i]->getParent())
			{
				m_DropItemMap[i]->removeFromParentAndCleanup(true);
			}
			else
			{
				m_DropItemMap[i]->release();
			}

			m_DropItemMap[i] = nullptr;
		}
	}
	DropItemNum = 0;

	for (int i = 0; i < PlayerWeaponNum; i++)
	{
		if (m_PlayerWeaponMap[i])
		{
			if (m_PlayerWeaponMap[i]->getParent())
			{
				m_PlayerWeaponMap[i]->removeFromParentAndCleanup(true);
			}
			else
			{
				m_PlayerWeaponMap[i]->release();
			}

			m_PlayerWeaponMap[i] = nullptr;
		}
	}
	PlayerWeaponNum = 0;

	for (int i = 0; i < EnemyWeaponNum; i++)
	{
		if (m_EnemyWeaponMap[i])
		{
			if (m_EnemyWeaponMap[i]->getParent())
			{
				m_EnemyWeaponMap[i]->removeFromParentAndCleanup(true);
			}
			else
			{
				m_EnemyWeaponMap[i]->release();
			}

			m_EnemyWeaponMap[i] = nullptr;
		}
	}
	EnemyWeaponNum = 0;

	std::map<int, std::list<LXObject*>>::iterator it = m_dropItems.begin();
	while (it != m_dropItems.end())
	{
		if (it->second.size() > 0)
		{
			std::list<LXObject*>::iterator itor = it->second.begin();
			while (itor != it->second.end())
			{
				if ((*itor)->getParent())
				{
					(*itor)->removeFromParentAndCleanup(true);
				}
				else
				{
					(*itor)->release();
				}
				itor++;
			}
			it->second.clear();
		}
		it++;
	}
	m_dropItems.clear();
	
	it = m_plane_bombs.begin();
	while (it != m_plane_bombs.end())
	{
		if (it->second.size() > 0)
		{
			std::list<LXObject*>::iterator itor = it->second.begin();
			while (itor != it->second.end())
			{
				if ((*itor)->getParent())
				{
					(*itor)->removeFromParentAndCleanup(true);
				}
				else
				{
					(*itor)->release();
				}
				itor++;
			}
			it->second.clear();
		}
		it++;
	}
	m_plane_bombs.clear();
	
	it = m_plane_bosss.begin();
	while (it != m_plane_bosss.end())
	{
		if (it->second.size() > 0)
		{
			std::list<LXObject*>::iterator itor = it->second.begin();
			while (itor != it->second.end())
			{
				if ((*itor)->getParent())
				{
					(*itor)->removeFromParentAndCleanup(true);
				}
				else
				{
					(*itor)->release();
				}
				itor++;
			}
			it->second.clear();
		}
		it++;
	}
	m_plane_bosss.clear();
	
	it = m_plane_enemys.begin();
	while (it != m_plane_enemys.end())
	{
		if (it->second.size() > 0)
		{
			std::list<LXObject*>::iterator itor = it->second.begin();
			while (itor != it->second.end())
			{
				if ((*itor)->getParent())
				{
					(*itor)->removeFromParentAndCleanup(true);
				}
				else
				{
					(*itor)->release();
				}
				itor++;
			}
			it->second.clear();
		}
		it++;
	}
	m_plane_enemys.clear();
	
	it = m_plane_players.begin();
	while (it != m_plane_players.end())
	{
		if (it->second.size() > 0)
		{
			std::list<LXObject*>::iterator itor = it->second.begin();
			while (itor != it->second.end())
			{
				if ((*itor)->getParent())
				{
					(*itor)->removeFromParentAndCleanup(true);
				}
				else
				{
					(*itor)->release();
				}
				itor++;
			}
			it->second.clear();
		}
		it++;
	}
	m_plane_players.clear();
	
	it = m_plane_wings.begin();
	while (it != m_plane_wings.end())
	{
		if (it->second.size() > 0)
		{
			std::list<LXObject*>::iterator itor = it->second.begin();
			while (itor != it->second.end())
			{
				if ((*itor)->getParent())
				{
					(*itor)->removeFromParentAndCleanup(true);
				}
				else
				{
					(*itor)->release();
				}
				itor++;
			}
			it->second.clear();
		}
		it++;
	}
	m_plane_wings.clear();
	
	it = m_weapon_bombs.begin();
	while (it != m_weapon_bombs.end())
	{
		if (it->second.size() > 0)
		{
			std::list<LXObject*>::iterator itor = it->second.begin();
			while (itor != it->second.end())
			{
				if ((*itor)->getParent())
				{
					(*itor)->removeFromParentAndCleanup(true);
				}
				else
				{
					(*itor)->release();
				}
				itor++;
			}
			it->second.clear();
		}
		it++;
	}
	m_weapon_bombs.clear();
	
	it = m_weapon_commons.begin();
	while (it != m_weapon_commons.end())
	{
		if (it->second.size() > 0)
		{
			std::list<LXObject*>::iterator itor = it->second.begin();
			while (itor != it->second.end())
			{
				if ((*itor)->getParent())
				{
					(*itor)->removeFromParentAndCleanup(true);
				}
				else
				{
					(*itor)->release();
				}
				itor++;
			}
			it->second.clear();
		}
		it++;
	}
	m_weapon_commons.clear();
	
	it = m_weapon_lasers.begin();
	while (it != m_weapon_lasers.end())
	{
		if (it->second.size() > 0)
		{
			std::list<LXObject*>::iterator itor = it->second.begin();
			while (itor != it->second.end())
			{
				if ((*itor)->getParent())
				{
					(*itor)->removeFromParentAndCleanup(true);
				}
				else
				{
					(*itor)->release();
				}
				itor++;
			}
			it->second.clear();
		}
		it++;
	}
	m_weapon_lasers.clear();
	
	it = m_weapon_parents.begin();
	while (it != m_weapon_parents.end())
	{
		if (it->second.size() > 0)
		{
			std::list<LXObject*>::iterator itor = it->second.begin();
			while (itor != it->second.end())
			{
				if ((*itor)->getParent())
				{
					(*itor)->removeFromParentAndCleanup(true);
				}
				else
				{
					(*itor)->release();
				}
				itor++;
			}
			it->second.clear();
		}
		it++;
	}
	m_weapon_parents.clear();
	
	it = m_weapon_speeds.begin();
	while (it != m_weapon_speeds.end())
	{
		if (it->second.size() > 0)
		{
			std::list<LXObject*>::iterator itor = it->second.begin();
			while (itor != it->second.end())
			{
				if ((*itor)->getParent())
				{
					(*itor)->removeFromParentAndCleanup(true);
				}
				else
				{
					(*itor)->release();
				}
				itor++;
			}
			it->second.clear();
		}
		it++;
	}
	m_weapon_speeds.clear();
	
	it = m_weapon_tracks.begin();
	while (it != m_weapon_tracks.end())
	{
		if (it->second.size() > 0)
		{
			std::list<LXObject*>::iterator itor = it->second.begin();
			while (itor != it->second.end())
			{
				if ((*itor)->getParent())
				{
					(*itor)->removeFromParentAndCleanup(true);
				}
				else
				{
					(*itor)->release();
				}
				itor++;
			}
			it->second.clear();
		}
		it++;
	}
	m_weapon_tracks.clear();

}

void LXObjectManager::calcData()
{

	std::map<int, int> info;
	for (int i = 0; i < PlayerPlaneNum; i++)
	{
		if(m_PlayerPlaneMap[i])
		{
			int resID = m_PlayerPlaneMap[i]->GetResID();
			if (info.find(resID) != info.end())
			{
				info[resID] += 1;
			}
			else
			{
				info[resID] = 1;
			}
		}
	}

	info.clear();
	for (int i = 0; i < EnemyPlaneNum; i++)
	{
		if (m_EnemyPlaneMap[i])
		{
			int resID = m_EnemyPlaneMap[i]->GetResID();
			if (info.find(resID) != info.end())
			{
				info[resID] += 1;
			}
			else
			{
				info[resID] = 1;
			}
		}
	}

	for (std::map<int, std::list<LXObject*>>::iterator itor = m_plane_enemys.begin(); itor != m_plane_enemys.end(); itor++)
	{
		info[itor->first] = itor->second.size();
	}

	for (std::map<int, std::list<LXObject*>>::iterator itor = m_plane_bosss.begin(); itor != m_plane_bosss.end(); itor++)
	{
		info[itor->first] = itor->second.size();
	}
	/*
	std::stringstream ss1;
	for (std::map<int, int>::iterator it = info.begin(); it != info.end(); it++)
	{
		ss1 << "\"" << it->first << "," << it->second <<  "\"" << ",";
	}
	log("%s", ss1.str().c_str());
*/
	info.clear();
	for (int i = 0; i < DropItemNum; i++)
	{
		if (m_DropItemMap[i])
		{
			int resID = m_DropItemMap[i]->GetResID();
			if (info.find(resID) != info.end())
			{
				info[resID] += 1;
			}
			else
			{
				info[resID] = 1;
			}
		}
	}

	for (std::map<int, std::list<LXObject*>>::iterator itor = m_dropItems.begin(); itor != m_dropItems.end(); itor++)
	{
		info[itor->first] = itor->second.size();
	}
	
	info.clear();

	for (int i = 0; i < PlayerWeaponNum; i++)
	{
		if (m_PlayerWeaponMap[i] && m_PlayerWeaponMap[i]->GetObjectType() != object_type_weapon_laser)
		{
			if(dynamic_cast<LXWeapon*>(m_PlayerWeaponMap[i])->IsAnimation())
			{
				int resID = m_PlayerWeaponMap[i]->GetResID();
				if (info.find(resID) != info.end())
				{
					info[resID] += 1;
				}
				else
				{
					info[resID] = 1;
				}
			}
		}
	}

	info.clear();

	
	for (int i = 0; i < EnemyWeaponNum; i++)
	{
		if (m_EnemyWeaponMap[i] && m_EnemyWeaponMap[i]->GetObjectType() != object_type_weapon_laser)
		{
			if(dynamic_cast<LXWeapon*>(m_EnemyWeaponMap[i])->IsAnimation())
			{
				int resID = m_EnemyWeaponMap[i]->GetResID();
				if (info.find(resID) != info.end())
				{
					info[resID] += 1;
				}
				else
				{
					info[resID] = 1;
				}
			}
		}
	}

	for (std::map<int, std::list<LXObject*>>::iterator itor = m_weapon_bombs.begin(); itor != m_weapon_bombs.end(); itor++)
	{
		for (std::list<LXObject*>::iterator it = itor->second.begin(); it != itor->second.end(); it++)
		{
			if(dynamic_cast<LXWeapon*>(*it)->IsAnimation())
			{
				if (info.find((*it)->GetResID()) != info.end())
				{
					info[(*it)->GetResID()] += 1;
				}
				else
				{
					info[(*it)->GetResID()] = 1;
				}
			}
		}
	}

	for (std::map<int, std::list<LXObject*>>::iterator itor = m_weapon_commons.begin(); itor != m_weapon_commons.end(); itor++)
	{
		for (std::list<LXObject*>::iterator it = itor->second.begin(); it != itor->second.end(); it++)
		{
			if(dynamic_cast<LXWeapon*>(*it)->IsAnimation())
			{
				if (info.find((*it)->GetResID()) != info.end())
				{
					info[(*it)->GetResID()] += 1;
				}
				else
				{
					info[(*it)->GetResID()] = 1;
				}
			}
		}
	}

	for (std::map<int, std::list<LXObject*>>::iterator itor = m_weapon_parents.begin(); itor != m_weapon_parents.end(); itor++)
	{
		for (std::list<LXObject*>::iterator it = itor->second.begin(); it != itor->second.end(); it++)
		{
			if(dynamic_cast<LXWeapon*>(*it)->IsAnimation())
			{
				if (info.find((*it)->GetResID()) != info.end())
				{
					info[(*it)->GetResID()] += 1;
				}
				else
				{
					info[(*it)->GetResID()] = 1;
				}
			}
		}
	}

	for (std::map<int, std::list<LXObject*>>::iterator itor = m_weapon_speeds.begin(); itor != m_weapon_speeds.end(); itor++)
	{
		for (std::list<LXObject*>::iterator it = itor->second.begin(); it != itor->second.end(); it++)
		{
			if(dynamic_cast<LXWeapon*>(*it)->IsAnimation())
			{
				if (info.find((*it)->GetResID()) != info.end())
				{
					info[(*it)->GetResID()] += 1;
				}
				else
				{
					info[(*it)->GetResID()] = 1;
				}
			}
		}
	}

	for (std::map<int, std::list<LXObject*>>::iterator itor = m_weapon_tracks.begin(); itor != m_weapon_tracks.end(); itor++)
	{
		for (std::list<LXObject*>::iterator it = itor->second.begin(); it != itor->second.end(); it++)
		{
			if(dynamic_cast<LXWeapon*>(*it)->IsAnimation())
			{
				if (info.find((*it)->GetResID()) != info.end())
				{
					info[(*it)->GetResID()] += 1;
				}
				else
				{
					info[(*it)->GetResID()] = 1;
				}
			}
		}
	}/*
	std::stringstream ss2;
	for (std::map<int, int>::iterator it = info.begin(); it != info.end(); it++)
	{
		ss2 << "\"" << it->first << "," << it->second <<  "\"" << ", ";
	}
	log("%s", ss2.str().c_str());
*/
	info.clear();
}

LXObject* LXObjectManager::getFreeObject(int type, int resID)
{
	LXObject *pObject = nullptr;
	// 没有找到创建
	switch (type)
	{
	case object_type_dropItem:
		{
			std::map<int, std::list<LXObject*>>::iterator it = m_dropItems.find(resID);
			if (it != m_dropItems.end())
			{
				if (it->second.size() > 0)
				{
					pObject = it->second.front();
					it->second.pop_front();
				}
				break;
			}
		}
		break;
	case object_type_plane_bomb:
		{
			std::map<int, std::list<LXObject*>>::iterator it = m_plane_bombs.find(resID);
			if (it != m_plane_bombs.end())
			{
				if (it->second.size() > 0)
				{
					pObject = it->second.front();
					it->second.pop_front();
				}
				break;
			}
		}
		break;
	case object_type_plane_boss:
		{
			std::map<int, std::list<LXObject*>>::iterator it = m_plane_bosss.find(resID);
			if (it != m_plane_bosss.end())
			{
				if (it->second.size() > 0)
				{
					pObject = it->second.front();
					it->second.pop_front();
				}
				break;
			}
		}
		break;
	case object_type_plane_enemy:
		{
			std::map<int, std::list<LXObject*>>::iterator it = m_plane_enemys.find(resID);
			if (it != m_plane_enemys.end())
			{
				if (it->second.size() > 0)
				{
					pObject = it->second.front();
					it->second.pop_front();
				}
				break;
			}
		}
		break;
	case object_type_plane_player:
		{
			std::map<int, std::list<LXObject*>>::iterator it = m_plane_players.find(resID);
			if (it != m_plane_players.end())
			{
				if (it->second.size() > 0)
				{
					pObject = it->second.front();
					it->second.pop_front();
				}
				break;
			}
		}
		break;
	case object_type_plane_wing:
		{
			std::map<int, std::list<LXObject*>>::iterator it = m_plane_wings.find(resID);
			if (it != m_plane_wings.end())
			{
				if (it->second.size() > 0)
				{
					pObject = it->second.front();
					it->second.pop_front();
				}
				break;
			}
		}
		break;
	case object_type_weapon_bomb:
		{
			std::map<int, std::list<LXObject*>>::iterator it = m_weapon_bombs.find(resID);
			if (it != m_weapon_bombs.end())
			{
				if (it->second.size() > 0)
				{
					pObject = it->second.front();
					it->second.pop_front();
				}
				break;
			}
		}
		break;
	case object_type_weapon_common:
		{
			std::map<int, std::list<LXObject*>>::iterator it = m_weapon_commons.find(resID);
			if (it != m_weapon_commons.end())
			{
				if (it->second.size() > 0)
				{
					pObject = it->second.front();
					it->second.pop_front();
				}
				break;
			}
		}
		break;
	case object_type_weapon_laser:
		{
			std::map<int, std::list<LXObject*>>::iterator it = m_weapon_lasers.find(resID);
			if (it != m_weapon_lasers.end())
			{
				if (it->second.size() > 0)
				{
					pObject = it->second.front();
					it->second.pop_front();
				}
				break;
			}
		}
		break;
	case object_type_weapon_parent:
		{
			std::map<int, std::list<LXObject*>>::iterator it = m_weapon_parents.find(resID);
			if (it != m_weapon_parents.end())
			{
				if (it->second.size() > 0)
				{
					pObject = it->second.front();
					it->second.pop_front();
				}
				break;
			}
		}
		break;
	case object_type_weapon_speed:
		{
			std::map<int, std::list<LXObject*>>::iterator it = m_weapon_speeds.find(resID);
			if (it != m_weapon_speeds.end())
			{
				if (it->second.size() > 0)
				{
					pObject = it->second.front();
					it->second.pop_front();
				}
				break;
			}
		}
		break;
	case object_type_weapon_track:
		{
			std::map<int, std::list<LXObject*>>::iterator it = m_weapon_tracks.find(resID);
			if (it != m_weapon_tracks.end())
			{
				if (it->second.size() > 0)
				{
					pObject = it->second.front();
					it->second.pop_front();
				}
				break;
			}
		}
		break;
	default:
		break;
	}

	if (pObject)
	{
		pObject->setVisible(true);
	}

	return pObject;
}

void LXObjectManager::PreCreateObject(int resID, Node *pParent, int type, bool isPlayer, int num)
{
	LXObject *pObject = nullptr;
	// 首先检测池子是否有空闲对象
	// 没有找到创建
	for(int i = 0; i < num; i++)
	{
		switch (type)
		{
		case object_type_dropItem:
			{
				pObject = new LXDropItem();
				if (m_dropItems.find(resID) != m_dropItems.end())
				{
					m_dropItems[resID].push_back(pObject);
				}
				else
				{
					std::list<LXObject*> m_list;
					m_list.push_back(pObject);
					m_dropItems[resID] = m_list;
				}
			}
			break;
		case object_type_plane_bomb:
			{
				pObject = new LXBombPlane();
				if (m_plane_bombs.find(resID) != m_plane_bombs.end())
				{
					m_plane_bombs[resID].push_back(pObject);
				}
				else
				{
					std::list<LXObject*> m_list;
					m_list.push_back(pObject);
					m_plane_bombs[resID] = m_list;
				}
			}
			break;
		case object_type_plane_boss:
			{
				pObject = new LXBossPlane();
				if (m_plane_bosss.find(resID) != m_plane_bosss.end())
				{
					m_plane_bosss[resID].push_back(pObject);
				}
				else
				{
					std::list<LXObject*> m_list;
					m_list.push_back(pObject);
					m_plane_bosss[resID] = m_list;
				}
			}
			break;
		case object_type_plane_enemy:
			{
				pObject = new LXEnemyPlane();
				if (m_plane_enemys.find(resID) != m_plane_enemys.end())
				{
					m_plane_enemys[resID].push_back(pObject);
				}
				else
				{
					std::list<LXObject*> m_list;
					m_list.push_back(pObject);
					m_plane_enemys[resID] = m_list;
				}
			}
			break;
		case object_type_plane_player:
			{
				pObject = new LXPlayerPlane();
				if (m_plane_players.find(resID) != m_plane_players.end())
				{
					m_plane_players[resID].push_back(pObject);
				}
				else
				{
					std::list<LXObject*> m_list;
					m_list.push_back(pObject);
					m_plane_players[resID] = m_list;
				}
			}
			break;
		case object_type_plane_wing:
			{
				pObject = new LXWingPlane();
				if (m_plane_wings.find(resID) != m_plane_wings.end())
				{
					m_plane_wings[resID].push_back(pObject);
				}
				else
				{
					std::list<LXObject*> m_list;
					m_list.push_back(pObject);
					m_plane_wings[resID] = m_list;
				}
			}
			break;
		case object_type_weapon_bomb:
			{
				pObject = new LXBombWeapon();
				if (m_weapon_bombs.find(resID) != m_weapon_bombs.end())
				{
					m_weapon_bombs[resID].push_back(pObject);
				}
				else
				{
					std::list<LXObject*> m_list;
					m_list.push_back(pObject);
					m_weapon_bombs[resID] = m_list;
				}
			}
			break;
		case object_type_weapon_common:
			{
				pObject = new LXCommonWeapon();
				if (m_weapon_commons.find(resID) != m_weapon_commons.end())
				{
					m_weapon_commons[resID].push_back(pObject);
				}
				else
				{
					std::list<LXObject*> m_list;
					m_list.push_back(pObject);
					m_weapon_commons[resID] = m_list;
				}
			}
			break;
		case object_type_weapon_laser:
			{
				pObject = new LXLaserWeapon();
				if (m_weapon_lasers.find(resID) != m_weapon_lasers.end())
				{
					m_weapon_lasers[resID].push_back(pObject);
				}
				else
				{
					std::list<LXObject*> m_list;
					m_list.push_back(pObject);
					m_weapon_lasers[resID] = m_list;
				}
			}
			break;
		case object_type_weapon_parent:
			{
				pObject = new LXLaserWeapon();
				if (m_weapon_parents.find(resID) != m_weapon_parents.end())
				{
					m_weapon_parents[resID].push_back(pObject);
				}
				else
				{
					std::list<LXObject*> m_list;
					m_list.push_back(pObject);
					m_weapon_parents[resID] = m_list;
				}
			}
			break;
		case object_type_weapon_speed:
			{
				pObject = new LXSpeedWeapon();
				if (m_weapon_speeds.find(resID) != m_weapon_speeds.end())
				{
					m_weapon_speeds[resID].push_back(pObject);
				}
				else
				{
					std::list<LXObject*> m_list;
					m_list.push_back(pObject);
					m_weapon_speeds[resID] = m_list;
				}
			}
			break;
		case object_type_weapon_track:
			{
				pObject = new LXTrackWeapon();
				if (m_weapon_tracks.find(resID) != m_weapon_tracks.end())
				{
					m_weapon_tracks[resID].push_back(pObject);
				}
				else
				{
					std::list<LXObject*> m_list;
					m_list.push_back(pObject);
					m_weapon_tracks[resID] = m_list;
				}
			}
			break;
		default:
			break;
		}
		
		switch (type)
		{
		case object_type_dropItem		:
			{
				pParent->addChild(pObject, GAME_ZORDER_ITEM, GetTag());
			}
			break;
		case object_type_plane_bomb		:
		case object_type_plane_boss		:
		case object_type_plane_enemy	:
		case object_type_plane_player	:
		case object_type_plane_wing		:
			{
				if (isPlayer)
				{
					pParent->addChild(pObject, GAME_ZORDER_PLANE, GetTag());
				}
				else
				{
					pParent->addChild(pObject, GAME_ZORDER_ENEMY, GetTag());
				}
			}
			break;
		case object_type_weapon_bomb	:
		case object_type_weapon_common	:
		case object_type_weapon_laser	:
		case object_type_weapon_parent	:
		case object_type_weapon_speed	:
		case object_type_weapon_track	:
			{
				if (isPlayer)
				{
					pParent->addChild(pObject, GAME_ZORDER_PLAYER_BULLET, GetTag());
				}
				else
				{
					pParent->addChild(pObject, GAME_ZORDER_AI_BULLET, GetTag());
				}
			}
			break;
		default:
			break;
		}

		pObject->Init(resID);
		pObject->removeFromParent();
	}
}

LXObject* LXObjectManager::CreateObject(int resID, Node *pParent, int type, bool isPlayer)
{
	LXObject *pObject = nullptr;
	// 首先检测池子是否有空闲对象
	pObject = getFreeObject(type, resID);
	if(!pObject)
	{
		// 没有找到创建
		switch (type)
		{
		case object_type_dropItem:
			{
				pObject = new LXDropItem();
			}
			break;
		case object_type_plane_bomb:
			{
				pObject = new LXBombPlane();
			}
			break;
		case object_type_plane_boss:
			{
				pObject = new LXBossPlane();
			}
			break;
		case object_type_plane_enemy:
			{
				pObject = new LXEnemyPlane();
			}
			break;
		case object_type_plane_player:
			{
				pObject = new LXPlayerPlane();
			}
			break;
		case object_type_plane_wing:
			{
				pObject = new LXWingPlane();
			}
			break;
		case object_type_weapon_bomb:
			{
				pObject = new LXBombWeapon();
			}
			break;
		case object_type_weapon_common:
			{
				pObject = new LXCommonWeapon();
			}
			break;
		case object_type_weapon_laser:
			{
				pObject = new LXLaserWeapon();
			}
			break;
		case object_type_weapon_parent:
			{
				pObject = new LXLaserWeapon();
			}
			break;
		case object_type_weapon_speed:
			{
				pObject = new LXSpeedWeapon();
			}
			break;
		case object_type_weapon_track:
			{
				pObject = new LXTrackWeapon();
			}
			break;
		default:
			break;
		}
	}

	if(pObject)
	{
		pObject->SetIsPlayer(isPlayer);
		pObject->SetObjectType(type);
		pObject->SetCleanUp(false);

		switch (type)
		{
		case object_type_dropItem		:
			{
				if (pObject->getParent() != pParent)
				{
					pObject->retain();
					pObject->removeFromParent();
					pParent->addChild(pObject, GAME_ZORDER_ITEM, GetTag());
					pObject->release();
				}
				pObject->Init(resID);
				addDropItem(pObject);
			}
			break;
		case object_type_plane_bomb		:
		case object_type_plane_boss		:
		case object_type_plane_enemy	:
		case object_type_plane_player	:
		case object_type_plane_wing		:
			{
				if (isPlayer)
				{
					if (pObject->getParent() != pParent)
					{
						pObject->retain();
						pObject->removeFromParent();
						pParent->addChild(pObject, GAME_ZORDER_PLANE, GetTag());
						pObject->release();
					}
				}
				else
				{
					if (pObject->getParent() != pParent)
					{
						pObject->retain();
						pObject->removeFromParent();
						pParent->addChild(pObject, GAME_ZORDER_ENEMY, GetTag());
						pObject->release();
					}
				}
				addPlane(pObject);
			}
			break;
		case object_type_weapon_bomb	:
		case object_type_weapon_common	:
		case object_type_weapon_laser	:
		case object_type_weapon_parent	:
		case object_type_weapon_speed	:
		case object_type_weapon_track	:
			{
				if (isPlayer)
				{
					if (pObject->getParent() != pParent)
					{
						pObject->retain();
						pObject->removeFromParent();
						pParent->addChild(pObject, GAME_ZORDER_PLAYER_BULLET, GetTag());
						pObject->release();
					}
				}
				else
				{
					if (pObject->getParent() != pParent)
					{
						pObject->retain();
						pObject->removeFromParent();
						pParent->addChild(pObject, GAME_ZORDER_AI_BULLET, GetTag());
						pObject->release();
					}
				}
				addWeapon(pObject);
			}
			break;
		default:
			break;
		}
	}
	return pObject;
}

void LXObjectManager::CleanAllObject()
{
	for (int i = 0; i < PlayerPlaneNum; i++)
	{
		if ( m_PlayerPlaneMap[i] && !m_PlayerPlaneMap[i]->IsUIObject())
		{
			m_PlayerPlaneMap[i]->SetCleanUp(true);
		}
	}
	for (int i = 0; i < EnemyPlaneNum; i++)
	{
		if (m_EnemyPlaneMap[i])
		{
			m_EnemyPlaneMap[i]->SetCleanUp(true);
			m_EnemyPlaneMap[i]->SetDeadFlag(false);
		}
	}
	for (int i = 0; i < DropItemNum; i++)
	{
		if (m_DropItemMap[i])
		{
			m_DropItemMap[i]->SetCleanUp(true);
		}
	}
	for (int i = 0; i < PlayerWeaponNum; i++)
	{
		if (m_PlayerWeaponMap[i])
		{
			m_PlayerWeaponMap[i]->SetCleanUp(true);
		}
	}
	for (int i = 0; i < EnemyWeaponNum; i++)
	{
		if(m_EnemyWeaponMap[i])
		{
			m_EnemyWeaponMap[i]->SetCleanUp(true);
		}
	}

	// 清理BOSS，敌机，敌机子弹
	std::map<int, std::list<LXObject*>>::iterator it = m_plane_bosss.begin();
	while (it != m_plane_bosss.end())
	{
		if (it->second.size() > 0)
		{
			std::list<LXObject*>::iterator itor = it->second.begin();
			while (itor != it->second.end())
			{
				if ((*itor)->getParent())
				{
					(*itor)->removeFromParentAndCleanup(true);
				}
				else
				{
					(*itor)->release();
				}
				itor++;
			}
			it->second.clear();
		}
		it++;
	}
	m_plane_bosss.clear();

	it = m_plane_enemys.begin();
	while (it != m_plane_enemys.end())
	{
		if (it->second.size() > 0)
		{
			std::list<LXObject*>::iterator itor = it->second.begin();
			while (itor != it->second.end())
			{
				if ((*itor)->getParent())
				{
					(*itor)->removeFromParentAndCleanup(true);
				}
				else
				{
					(*itor)->release();
				}
				itor++;
			}
			it->second.clear();
		}
		it++;
	}
	m_plane_enemys.clear();

	it = m_weapon_commons.begin();
	while (it != m_weapon_commons.end())
	{
		if (it->second.size() > 0)
		{
			std::list<LXObject*>::iterator itor = it->second.begin();
			while (itor != it->second.end())
			{
				if ((*itor)->getParent())
				{
					(*itor)->removeFromParentAndCleanup(true);
				}
				else
				{
					(*itor)->release();
				}
				itor++;
			}
			it->second.clear();
		}
		it++;
	}
	m_weapon_commons.clear();

}

void LXObjectManager::addToFreePool(LXObject *pObject)
{
	// 从父节点摘除
	//pObject->removeFromParent();
	pObject->setVisible(false);
	int resID = pObject->GetResID();
	switch (pObject->GetObjectType())
	{
	case object_type_dropItem		:
		{
			if (m_dropItems.find(resID) != m_dropItems.end())
			{
				m_dropItems[resID].push_back(pObject);
			}
			else
			{
				std::list<LXObject*> m_list;
				m_list.push_back(pObject);
				m_dropItems[resID] = m_list;
			}
		}
		break;
	case object_type_plane_bomb		:
		{
			if (m_plane_bombs.find(resID) != m_plane_bombs.end())
			{
				m_plane_bombs[resID].push_back(pObject);
			}
			else
			{
				std::list<LXObject*> m_list;
				m_list.push_back(pObject);
				m_plane_bombs[resID] = m_list;
			}
		}
		break;
	case object_type_plane_boss		:
		{
			if (m_plane_bosss.find(resID) != m_plane_bosss.end())
			{
				m_plane_bosss[resID].push_back(pObject);
			}
			else
			{
				std::list<LXObject*> m_list;
				m_list.push_back(pObject);
				m_plane_bosss[resID] = m_list;
			}
		}
		break;
	case object_type_plane_enemy	:
		{
			if (m_plane_enemys.find(resID) != m_plane_enemys.end())
			{
				m_plane_enemys[resID].push_back(pObject);
			}
			else
			{
				std::list<LXObject*> m_list;
				m_list.push_back(pObject);
				m_plane_enemys[resID] = m_list;
			}
		}
		break;
	case object_type_plane_player	:
		{
			if (m_plane_players.find(resID) != m_plane_players.end())
			{
				m_plane_players[resID].push_back(pObject);
			}
			else
			{
				std::list<LXObject*> m_list;
				m_list.push_back(pObject);
				m_plane_players[resID] = m_list;
			}
		}
		break;
	case object_type_plane_wing		:
		{
			if (m_plane_wings.find(resID) != m_plane_wings.end())
			{
				m_plane_wings[resID].push_back(pObject);
			}
			else
			{
				std::list<LXObject*> m_list;
				m_list.push_back(pObject);
				m_plane_wings[resID] = m_list;
			}
		}
		break;
	case object_type_weapon_bomb	:
		{
			if (m_weapon_bombs.find(resID) != m_weapon_bombs.end())
			{
				m_weapon_bombs[resID].push_back(pObject);
			}
			else
			{
				std::list<LXObject*> m_list;
				m_list.push_back(pObject);
				m_weapon_bombs[resID] = m_list;
			}
		}
		break;
	case object_type_weapon_common	:
		{
			if (m_weapon_commons.find(resID) != m_weapon_commons.end())
			{
				m_weapon_commons[resID].push_back(pObject);
			}
			else
			{
				std::list<LXObject*> m_list;
				m_list.push_back(pObject);
				m_weapon_commons[resID] = m_list;
			}
		}
		break;
	case object_type_weapon_laser	:
		{
			if (m_weapon_lasers.find(resID) != m_weapon_lasers.end())
			{
				m_weapon_lasers[resID].push_back(pObject);
			}
			else
			{
				std::list<LXObject*> m_list;
				m_list.push_back(pObject);
				m_weapon_lasers[resID] = m_list;
			}
		}
		break;
	case object_type_weapon_parent	:
		{
			if (m_weapon_parents.find(resID) != m_weapon_parents.end())
			{
				m_weapon_parents[resID].push_back(pObject);
			}
			else
			{
				std::list<LXObject*> m_list;
				m_list.push_back(pObject);
				m_weapon_parents[resID] = m_list;
			}
		}
		break;
	case object_type_weapon_speed	:
		{
			if (m_weapon_speeds.find(resID) != m_weapon_speeds.end())
			{
				m_weapon_speeds[resID].push_back(pObject);
			}
			else
			{
				std::list<LXObject*> m_list;
				m_list.push_back(pObject);
				m_weapon_speeds[resID] = m_list;
			}
		}
		break;
	case object_type_weapon_track	:
		{
			if (m_weapon_tracks.find(resID) != m_weapon_tracks.end())
			{
				m_weapon_tracks[resID].push_back(pObject);
			}
			else
			{
				std::list<LXObject*> m_list;
				m_list.push_back(pObject);
				m_weapon_tracks[resID] = m_list;
			}
		}
		break;
	default:
		break;
	}
}

void LXObjectManager::Tick(float fDelta)
{
	// 遍历所有的元素
	for (int i = 0; i < PlayerPlaneNum; i++)
	{
		if (m_PlayerPlaneMap[i] != nullptr)
		{
			if (m_PlayerPlaneMap[i]->NeedClean())
			{
				m_PlayerPlaneMap[i]->Destroy();
				addToFreePool(m_PlayerPlaneMap[i]);
				m_PlayerPlaneMap[i] = nullptr;
			}
			else
			{
				m_PlayerPlaneMap[i]->Tick(fDelta);
			}
		}
	}

	for (int i = 0; i < EnemyPlaneNum; i++)
	{
		if (m_EnemyPlaneMap[i] != nullptr)
		{
			if (m_EnemyPlaneMap[i]->NeedClean())
			{
				m_EnemyPlaneMap[i]->Destroy();
				addToFreePool(m_EnemyPlaneMap[i]);
				m_EnemyPlaneMap[i] = nullptr;
			}
			else
			{
				m_EnemyPlaneMap[i]->Tick(fDelta);
			}
		}
	}
	for (int i = 0; i < DropItemNum; i++)
	{
		if (m_DropItemMap[i] != nullptr)
		{
			if (m_DropItemMap[i]->NeedClean())
			{
				m_DropItemMap[i]->Destroy();
				addToFreePool(m_DropItemMap[i]);
				m_DropItemMap[i] = nullptr;
			}
			else
			{
				m_DropItemMap[i]->Tick(fDelta);
			}
		}
	}
	for (int i = 0; i < PlayerWeaponNum; i++)
	{
		if (m_PlayerWeaponMap[i] != nullptr)
		{
			if (m_PlayerWeaponMap[i]->NeedClean())
			{
				m_PlayerWeaponMap[i]->Destroy();
				addToFreePool(m_PlayerWeaponMap[i]);
				m_PlayerWeaponMap[i] = nullptr;
			}
			else
			{
				m_PlayerWeaponMap[i]->Tick(fDelta);
			}
		}
	}
	for (int i = 0; i < EnemyWeaponNum; i++)
	{
		if (m_EnemyWeaponMap[i] != nullptr)
		{
			if (m_EnemyWeaponMap[i]->NeedClean())
			{
				m_EnemyWeaponMap[i]->Destroy();
				addToFreePool(m_EnemyWeaponMap[i]);
				m_EnemyWeaponMap[i] = nullptr;
			}
			else
			{
				m_EnemyWeaponMap[i]->Tick(fDelta);
			}
		}
	}
	
	/*---------------------------------------------------------------------*/
	// 晃动屏幕
	if ( m_IsSway )
	{
		swayAllObj(fDelta);
	}

}

void LXObjectManager::addPlane(LXObject *pObject)
{
	if (!pObject)
	{
		return;
	}
	
	bool bDone = false;
	if (pObject->IsPlayer())
	{
		for (int i = 0; i < PlayerPlaneNum; i++)
		{
			if(m_PlayerPlaneMap[i] == nullptr)
			{
				m_PlayerPlaneMap[i] = pObject;
				bDone = true;
				break;
			}
		}

		if (!bDone)
		{
			m_PlayerPlaneMap[PlayerPlaneNum] = pObject;
			PlayerPlaneNum++;
		}

		if (PlayerPlaneNum > 100)
		{
			PlayerPlaneNum = 100;
		}
	}
	else
	{
		for (int i = 0; i < EnemyPlaneNum; i++)
		{
			if(m_EnemyPlaneMap[i] == nullptr)
			{
				m_EnemyPlaneMap[i] = pObject;
				bDone = true;
				break;
			}
		}

		if (!bDone)
		{
			m_EnemyPlaneMap[EnemyPlaneNum] = pObject;
			EnemyPlaneNum++;
		}
		
		if (EnemyPlaneNum > 500)
		{
			EnemyPlaneNum = 500;
		}
	}
}

void LXObjectManager::addDropItem(LXObject *pObject)
{
	if (!pObject)
	{
		return;
	}

	// 增加引用
	bool bDone = false;
	for (int i = 0; i < DropItemNum; i++)
	{
		if(m_DropItemMap[i] == nullptr)
		{
			m_DropItemMap[i] = pObject;
			bDone = true;
			break;
		}
	}

	if (!bDone)
	{
		m_DropItemMap[DropItemNum] = pObject;
		DropItemNum++;
	}

	if (DropItemNum > 1000)
	{
		DropItemNum = 1000;
	}
}

void LXObjectManager::addWeapon(LXObject *pObject)
{
	if (!pObject)
	{
		return;
	}
	
	for (int i = 0; i < PlayerWeaponNum; i++)
	{
		if(m_PlayerWeaponMap[i] == pObject)
		{
			return;
		}
	}

	// 增加引用
	bool bDone = false;
	if (pObject->IsPlayer())
	{
		for (int i = 0; i < PlayerWeaponNum; i++)
		{
			if(m_PlayerWeaponMap[i] == nullptr)
			{
				m_PlayerWeaponMap[i] = pObject;
				bDone = true;
				break;
			}
		}

		if (!bDone)
		{
			m_PlayerWeaponMap[PlayerWeaponNum] = pObject;
			PlayerWeaponNum++;
			bDone = true;
		}

		if (PlayerWeaponNum > 500)
		{
			PlayerWeaponNum = 500;
		}
	}
	else
	{
		for (int i = 0; i < EnemyWeaponNum; i++)
		{
			if(m_EnemyWeaponMap[i] == nullptr)
			{
				m_EnemyWeaponMap[i] = pObject;
				bDone = true;
				break;
			}
		}

		if (!bDone)
		{
			m_EnemyWeaponMap[EnemyWeaponNum] = pObject;
			EnemyWeaponNum++;
			bDone = true;
		}
		
		if (EnemyWeaponNum > 1000)
		{
			EnemyWeaponNum = 1000;
		}
	}
}

int	LXObjectManager::GetTag()
{
	static int tag = 1;
	return tag++;
}

bool LXObjectManager::PlayerHitLogic(LXObject* pObject)
{
	// 遍历所有敌机检测碰撞
	for (int i = 0; i < EnemyPlaneNum; i++)
	{
		// 首先判断AI是否存活
		if (m_EnemyPlaneMap[i] && m_EnemyPlaneMap[i]->IsAlive())
		{
			// 碰撞检测
			//
			if (pObject->GetObjectBoundBox().intersectsRect(m_EnemyPlaneMap[i]->GetObjectBoundBox()))
			{
				if (pObject->GetCollideInfo()->isCollidWithOBB(m_EnemyPlaneMap[i]->GetCollideInfo()))
				{
					// 播放受击动画
					m_EnemyPlaneMap[i]->PlayHitEffect();

					// 伤害逻辑
					int damage = std::max(0, pObject->GetAttack() - m_EnemyPlaneMap[i]->GetDefense());
					m_EnemyPlaneMap[i]->Hit(damage);

					return true;
				}
			}
		}
	}

	return false;
}

bool LXObjectManager::EnemyHitLogic(LXObject* pObject)
{
	//
	auto player = GetPlayer();
	// 主机死亡
	if (player->IsAlive() == false)
	{
		return false;
	}

	
	if (pObject->GetObjectBoundBox().intersectsRect(player->GetObjectBoundBox()))
	{
		if (pObject->GetCollideInfo()->isCollidWithOBB(player->GetCollideInfo()))
		{
			//受击动画
			player->PlayHitEffect();

			//伤害逻辑
			int damage = std::max(0, pObject->GetAttack() - player->GetDefense());
			player->Hit(damage);
		
			return true;
		}
	}


	return false;
}

bool LXObjectManager::BombHitLogic(LXObject* pObject)
{
	// 遍历所有敌机检测碰撞
	for (int i = 0; i < EnemyWeaponNum; i++)
	{
		// 首先判断AI是否存活
		if (m_EnemyWeaponMap[i] && m_EnemyWeaponMap[i]->IsAlive())
		{
			// 碰撞检测
			//
			if (pObject->GetObjectBoundBox().intersectsRect(m_EnemyWeaponMap[i]->GetObjectBoundBox()))
			{
				if (pObject->GetCollideInfo()->isCollidWithOBB(m_EnemyWeaponMap[i]->GetCollideInfo()))
				{
					m_EnemyWeaponMap[i]->SetCleanUp(true);
				}
			}
		}
	}

	// 遍历所有敌机检测碰撞
	for (int i = 0; i < EnemyPlaneNum; i++)
	{
		// 首先判断AI是否存活
		if (m_EnemyPlaneMap[i] && m_EnemyPlaneMap[i]->IsAlive())
		{
			// 碰撞检测
			//
			if (pObject->GetObjectBoundBox().intersectsRect(m_EnemyPlaneMap[i]->GetObjectBoundBox()))
			{
				if (pObject->GetCollideInfo()->isCollidWithOBB(m_EnemyPlaneMap[i]->GetCollideInfo()))
				{
					// 播放受击动画
					m_EnemyPlaneMap[i]->PlayHitEffect();
					// 伤害逻辑
					int hit = CCUserDefault::sharedUserDefault()->getIntegerForKey("hit");
					m_EnemyPlaneMap[i]->Hit(hit);

					if (m_EnemyPlaneMap[i]->IsAlive())
					{
						std::string effect = "p0/boom_m.p";
						Node *pParent = dynamic_cast<GameScene*>(ObjectManager().GetGameSceneNode())->GetNodeByZOrder(GAME_ZORDER_OBJECT_PARTICLE);
						EffectManager().CreateEffect(pParent, effect, pObject->getPositionX(), pObject->getPositionY(), GAME_ZORDER_OBJECT_PARTICLE, false);
					}

					// 爆炸范围群体伤血
					for (int j = 0; j < EnemyPlaneNum; j++)
					{
						// 首先判断AI是否存活
						if (i != j && m_EnemyPlaneMap[j] && m_EnemyPlaneMap[j]->IsAlive())
						{
							int x = m_EnemyPlaneMap[j]->getPositionX() - pObject->getPositionX();
							int y = m_EnemyPlaneMap[j]->getPositionY() - pObject->getPositionY();
							if(x * x + y * y <= 90000) 
							{
								m_EnemyPlaneMap[j]->Hit(hit);
							}
						}
					}

					return true;
				}
			}
		}
	}

	return false;
}

LXObject* LXObjectManager::PlayerLaserHitLogic(LXObject* pObject, float fTime)
{
	// 遍历所有敌机检测碰撞
	LXLaserWeapon *pWeapon = dynamic_cast<LXLaserWeapon *>(pObject);
	if (pWeapon->GetCanThrough() == false)
	{
		// 需要检测碰撞目标然后才能计算伤害
		LXObject *pTargetObject = nullptr;
		for (int i = 0; i < EnemyPlaneNum; i++)
		{
			// 首先判断AI是否存活
			if (m_EnemyPlaneMap[i] && m_EnemyPlaneMap[i]->IsAlive())
			{
				// 碰撞检测
				bool bRet = pObject->GetCollideInfo()->isCollidWithOBB(m_EnemyPlaneMap[i]->GetCollideInfo());
				if (bRet)
				{
					if (pTargetObject == nullptr)
					{
						pTargetObject = m_EnemyPlaneMap[i];
					}
					else if (pTargetObject->getPosition().y > m_EnemyPlaneMap[i]->getPosition().y)
					{
						pTargetObject = m_EnemyPlaneMap[i];
					}
				}
			}
		}
		
		if (pTargetObject)
		{
			// 伤害逻辑
			int damage = fTime * (pObject->GetAttack() - pTargetObject->GetDefense());
			damage = damage > 0?damage:0;
				
			// 播放受击动画
			pTargetObject->PlayHitEffect();

			// 伤害逻辑
			pTargetObject->Hit(damage);

			return pTargetObject;
		}
	}
	else
	{
		for (int i = 0; i < EnemyPlaneNum; i++)
		{
			// 首先判断AI是否存活
			if (m_EnemyPlaneMap[i] && m_EnemyPlaneMap[i]->IsAlive())
			{
				// 碰撞检测
				bool bRet = pObject->GetCollideInfo()->isCollidWithOBB(m_EnemyPlaneMap[i]->GetCollideInfo());
				if (bRet)
				{
					int damage = fTime * (pObject->GetAttack() - m_EnemyPlaneMap[i]->GetDefense());
					damage = damage > 0?damage:0;
				
					// 播放受击动画
					m_EnemyPlaneMap[i]->PlayHitEffect();

					// 伤害逻辑
					m_EnemyPlaneMap[i]->Hit(damage);
				}
			}
		}
	}
	
	return nullptr;
}

LXObject* LXObjectManager::EnemyLaserHitLogic(LXObject* pObject, float fTime)
{
	auto player = GetPlayer();
	
	// 主机死亡
	if (player->IsAlive() == false)
	{
		return nullptr;
	}


	bool bRet = pObject->GetCollideInfo()->isCollidWithOBB(player->GetCollideInfo());
	if (bRet)
	{
		// 伤害逻辑
		int damage = fTime * (pObject->GetAttack() - player->GetDefense());
		damage = damage > 0?damage:0;
		
		//受击动画
		player->PlayHitEffect();

		//伤害逻辑
		player->Hit(damage);

		return player;
	}

	return nullptr;
}

LXObject* LXObjectManager::GetRandomObject(bool isPlayer)
{
	if (isPlayer)
	{
		return m_pPlayer;
	}
	else
	{
		int num = 0;
		for (int i = 0; i < EnemyPlaneNum; i++)
		{
			if (m_EnemyPlaneMap[i] != nullptr)
			{
				num++;
			}
		}

		if (num == 0)
		{
			return nullptr;
		}
		else if (num == 1)
		{
			for (int i = 0; i < EnemyPlaneNum; i++)
			{
				if (m_EnemyPlaneMap[i] != nullptr)
				{
					return m_EnemyPlaneMap[i];
				}
			}
		}

		int index = rand() % num;
		num = 0;
		for (int i = 0; i < EnemyPlaneNum; i++)
		{
			if (m_EnemyPlaneMap[i] != nullptr)
			{
				num++;
				if (num == index)
				{
					return m_EnemyPlaneMap[i];
				}
			}
		}
	}

	return nullptr;
}

void LXObjectManager::EnemyWeaponBecomeCrystal()
{
	auto glview = Director::getInstance()->getOpenGLView();
	const auto screenSize = glview->getDesignResolutionSize();
	float startX = 0.0f;
	float endX = screenSize.width;
	if (screenSize.width == 600) // 600*800加边框特殊处理
	{
		startX = 30;
		endX = 570;
	}

	for (int i = 0; i < EnemyWeaponNum; i++)
	{
		LXObject *pEnemyObject = m_EnemyWeaponMap[i];
		if (pEnemyObject)
		{
			bool isBecome = false;
			if( pEnemyObject->getPositionX() >= startX && pEnemyObject->getPositionY() < endX && pEnemyObject->getPositionY() >= 0.0f && pEnemyObject->getPositionY() < screenSize.height )
			{
				isBecome = true;
			}

			if ( true )
			{
				Node *pParent = dynamic_cast<GameScene*>(GetGameSceneNode())->GetNodeByZOrder(GAME_ZORDER_ITEM);
				LXDropItem* pObject = dynamic_cast<LXDropItem*>(ObjectManager().CreateObject(6, pParent, object_type_dropItem, false) );
				if (pObject)
				{
					pObject->Init( 6 );
					pObject->SetTarget(true);
					pObject->SetTargetRange(-1);
					pObject->SetTargetMoveSpeed( 1200 );
					pObject->SetTargetDelayTime( 1.5F );
					pObject->InitAction( 0.5f, 30, 30, 30, rand()%360, 0, 0, false, false, startX, 0, endX, screenSize.height );
					pObject->setPosition( pEnemyObject->getPosition() );

					// a("tocrystal", i, var2.getX(), var2.getY(), false, true);

					pEnemyObject-> SetCleanUp( true );
				}
			}
		}
	}
}

void LXObjectManager::PlayerPlaneBeHit()
{
	LXPlayerPlane* pPlayer = dynamic_cast<LXPlayerPlane*>( GetPlayer() );

	if (!pPlayer)
	{
		return ;
	}

	// 晃动屏幕
	InitSwayScreen(0.5f);

	// 播放特效
	//com.sg.raiden.a.c.a.a var9 = p.a("user_hited", p.c(), 240.0F, 424.0F, false);
	//var9.setTransform(true);
	//var9.setScaleY(1.0F * (float)com.sg.raiden.a.a / 800.0F);

	// 屏幕四周出现红色警戒特效
	//std::string strName = "p0/user_hited.p";
	//ParticleEffect *effect = EffectManager().CreateEffect(pPlayer->getParent(), strName, Utils().GetFixedX(240), Utils().GetFixedY(400), GAME_ZORDER_OBJECT_PARTICLE, false);
	//effect->GetParent()->setScaleY( 1.0f * Utils().GetYRate() );

	// 如果声音打开。则震动
	if ( !DBManager().GetData(DATA_EFFECT_ON))
	{
		// 调用震动功能
		// long[] var10 = new long[]{100L, 500L};
		// com.sg.raiden.a.e.f.a(var10);			
	}


	// 如果有护盾
	if ( DBManager().GetMissionShield() > 0 )
	{
		DBManager().UseMissionShield();

		pPlayer->CreatProtectEffect();
		pPlayer->ShowProtectEffect();
		pPlayer->SetInvincibleTime(3.0f);
		pPlayer->SetProtectTime(1.0f);
		SoundSystem().playEffect(Mic_shield);// 声音

	}else
	{

		DBManager().AddDieTimes(); //记录死亡次数
		pPlayer->StopShootLogic();
		pPlayer->ShowPlane(false);		
		pPlayer->CreatItem(3);					// 死亡后掉落 3 号物品
		
		TaskMgr().DoneTaskOption(5);;			// 完成 5 号任务	怒送一血	第一次死亡
		//w.c("boom_m.ogg");

		Node *pParent = dynamic_cast<GameScene*>(ObjectManager().GetGameSceneNode())->GetNodeByZOrder(GAME_ZORDER_OBJECT_PARTICLE);
		std::string strName = "p0/boom_m.p";
		EffectManager().CreateEffect(pParent, strName, pPlayer->getPositionX(), pPlayer->getPositionY(), GAME_ZORDER_OBJECT_PARTICLE, false);
		SoundSystem().playEffect(Mic_boom_m);

		if ( DBManager().GetMissionLife() > 0 )			// 如果生命大于0
		{
			DBManager().UseMissionLife();					// 使用一条生命

			pPlayer->CreatItem(1);					// 掉落 1 号物品
			pPlayer->CreatItem(1);					// 掉落 1 号物品
			pPlayer->RunIntoScreen();				// 重生
			pPlayer->RefreshProtectWithReLife();	// 重新计算护盾

		}else
		{
			std::vector<int> vec;
			vec.push_back(100);
			pPlayer->CreatCrystal(false,vec);
			pPlayer->CreatItem(2);
			pPlayer->CreatItem(1);					// 掉落 1 号物品
			pPlayer->CreatItem(1);					// 掉落 1 号物品
			pPlayer->CreatItem(3);					// 掉落 1 号物品
			pPlayer->CreatItem(3);					// 掉落 1 号物品

			for(int i = 0; i < 30; ++i) {
				pPlayer->CreatItem(12);					// 掉落物品
			}

			// 设置为死亡
			pPlayer->Hit(pPlayer->GetHP());
			pPlayer->SetDeadFlag(true);


			// 弹出式购买
			static_cast<MainScene*>(pPlayer->getParent()->getParent()->getParent())->ShowCounDownFlowBag( BuyType::Buy_Life );
			
			// 这里不清理飞机。因为需要判断玩家是否购买复活道具
	
			// 设置位置
			pPlayer->setPosition( Utils().GetFixedX(PLANE_START_X), PLANE_START_Y);

		}
	}
}

void LXObjectManager::InitSwayScreen(float duration)
{
	m_duration = duration;
	m_IsSway = true;
	m_lasTime = 0.0f;
	m_xparam = 0;
	m_yparam = 0;
}

void LXObjectManager::CleanEnemyWeapon()
{
	for (int i = 0; i < EnemyWeaponNum; i++)
	{
		if (m_EnemyWeaponMap[i])
		{
			m_EnemyWeaponMap[i]->SetCleanUp(true);
			m_EnemyWeaponMap[i]->SetDeadFlag(true);
		}
	}
}

void LXObjectManager::swayAllObj(float dt)
{
	if ( m_lasTime > m_duration )
	{
		m_IsSway = false;

		int tempX = - m_xparam;
		int	tempY = - m_yparam;

		for (int i = 0; i < PlayerPlaneNum; i++)
		{
			if (m_PlayerPlaneMap[i] && !m_PlayerPlaneMap[i]->IsUIObject())
			{
				m_PlayerPlaneMap[i]->setPositionX( m_PlayerPlaneMap[i]->getPositionX() + tempX );
				m_PlayerPlaneMap[i]->setPositionY( m_PlayerPlaneMap[i]->getPositionY() + tempY );
			}
		}
		for (int i = 0; i < EnemyPlaneNum; i++)
		{
			if (m_EnemyPlaneMap[i])
			{
				m_EnemyPlaneMap[i]->setPositionX( m_EnemyPlaneMap[i]->getPositionX() + tempX );
				m_EnemyPlaneMap[i]->setPositionY( m_EnemyPlaneMap[i]->getPositionY() + tempY );
			}
		}
		for (int i = 0; i < DropItemNum; i++)
		{
			if (m_DropItemMap[i])
			{
				m_DropItemMap[i]->setPositionX( m_DropItemMap[i]->getPositionX() + tempX );
				m_DropItemMap[i]->setPositionY( m_DropItemMap[i]->getPositionY() + tempY );
			}
		}
		for (int i = 0; i < PlayerWeaponNum; i++)
		{
			if (m_PlayerWeaponMap[i])
			{
				m_PlayerWeaponMap[i]->setPositionX( m_PlayerWeaponMap[i]->getPositionX() + tempX );
				m_PlayerWeaponMap[i]->setPositionY( m_PlayerWeaponMap[i]->getPositionY() + tempY );
			}
		}
		for (int i = 0; i < EnemyWeaponNum; i++)
		{
			if (m_EnemyWeaponMap[i])
			{
				m_EnemyWeaponMap[i]->setPositionX( m_EnemyWeaponMap[i]->getPositionX() + tempX );
				m_EnemyWeaponMap[i]->setPositionY( m_EnemyWeaponMap[i]->getPositionY() + tempY );
			}
		}
	}else
	{
		m_lasTime += dt;

		int varX = rand()%6 - 3;
		int varY = rand()%6 - 3;

		int tempX = varX - m_xparam;
		int	tempY = varY - m_yparam;

		m_xparam = varX;
		m_yparam = varY;

		for (int i = 0; i < PlayerPlaneNum; i++)
		{
			if (m_PlayerPlaneMap[i] && !m_PlayerPlaneMap[i]->IsUIObject())
			{
				m_PlayerPlaneMap[i]->setPositionX( m_PlayerPlaneMap[i]->getPositionX() + tempX );
				m_PlayerPlaneMap[i]->setPositionY( m_PlayerPlaneMap[i]->getPositionY() + tempY );
			}
		}
		for (int i = 0; i < EnemyPlaneNum; i++)
		{
			if (m_EnemyPlaneMap[i])
			{
				m_EnemyPlaneMap[i]->setPositionX( m_EnemyPlaneMap[i]->getPositionX() + tempX );
				m_EnemyPlaneMap[i]->setPositionY( m_EnemyPlaneMap[i]->getPositionY() + tempY );
			}
		}
		for (int i = 0; i < DropItemNum; i++)
		{
			if (m_DropItemMap[i])
			{
				m_DropItemMap[i]->setPositionX( m_DropItemMap[i]->getPositionX() + tempX );
				m_DropItemMap[i]->setPositionY( m_DropItemMap[i]->getPositionY() + tempY );
			}
		}
		for (int i = 0; i < PlayerWeaponNum; i++)
		{
			if (m_PlayerWeaponMap[i])
			{
				m_PlayerWeaponMap[i]->setPositionX( m_PlayerWeaponMap[i]->getPositionX() + tempX );
				m_PlayerWeaponMap[i]->setPositionY( m_PlayerWeaponMap[i]->getPositionY() + tempY );
			}
		}
		for (int i = 0; i < EnemyWeaponNum; i++)
		{
			if (m_EnemyWeaponMap[i])
			{
				m_EnemyWeaponMap[i]->setPositionX( m_EnemyWeaponMap[i]->getPositionX() + tempX );
				m_EnemyWeaponMap[i]->setPositionY( m_EnemyWeaponMap[i]->getPositionY() + tempY );
			}
		}
	}
}

int	LXObjectManager::GetEnemyNum()
{
	int num = 0;
	for (int i = 0; i < EnemyPlaneNum; i++)
	{
		if (m_EnemyPlaneMap[i])
		{
			num++;
		}
	}

	return num;
}