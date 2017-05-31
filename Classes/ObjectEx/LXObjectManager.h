#pragma once
#include <vector>
#include "cocos2d.h"
#include "LXObject.h"
#include "Effect/ParticleEffect.h"
#include <map>

USING_NS_CC;
//=============================================================================
//// 对象管理类
// 负责管理创建的所有对象，并不负责具体对象的创建，只负责遍历及特定逻辑处理


enum
{
	object_group_plane,
	object_group_sprite
};

enum
{
	object_type_none			= 0x0000,		
	object_type_dropItem		= 0x0001,		// 掉落物品

	object_type_plane_bomb		= 0x1001,		// 保险飞机
	object_type_plane_boss		= 0x1002,		// AI_BOSS
	object_type_plane_enemy		= 0x1003,		// AI_Enemy
	object_type_plane_player	= 0x1004,		// 玩家
	object_type_plane_wing		= 0x1005,		// 僚机

	object_type_weapon_bomb		= 0x2001,		// 保险飞机武器
	object_type_weapon_common	= 0x2002,		// 常规武器
	object_type_weapon_laser	= 0x2003,		// 激光武器
	object_type_weapon_parent	= 0x2004,		// 跟随武器
	object_type_weapon_speed	= 0x2005,		// 加速武器
	object_type_weapon_track	= 0x2006,		// 追踪武器
};

class LXObjectManager
{
public:
	LXObjectManager();
	~LXObjectManager();

	void				Tick(float fDelta);	
	
	// 设置节点
	void				SetGameSceneNode(Node *pNode) { m_pGameSceneNode = pNode; }
	void				SetMainSceneNode(Node *pNode) { m_pMainSceneNode = pNode; }

	Node*				GetGameSceneNode() { return m_pGameSceneNode; }
	Node*				GetMainSceneNode() { return m_pMainSceneNode; }

public:
	// 创建对象
	void				PreCreateObject(int resID, Node *pParent, int type, bool isPlayer, int num);

	// 创建对象
	LXObject*			CreateObject(int resID, Node *pParent, int type, bool isPlayer);

	//清理所有对象
	void				CleanAllObject();

	// 获取标签
	int					GetTag();

	// 获取怪物数量
	int					GetEnemyNum();

	// 注册玩家
	void				RigisterPlayer(LXObject* pObject) { m_pPlayer = pObject; }
	
	// 获取玩家对象
	LXObject*			GetPlayer() { return m_pPlayer; }

	// 来自玩家子弹的碰撞检测
	bool				PlayerHitLogic(LXObject* pObject);

	// 敌机子弹碰撞
	bool				EnemyHitLogic(LXObject* pObject);
	
	// 必杀碰撞
	bool				BombHitLogic(LXObject* pObject);

	// 来自玩家子弹的碰撞检测
	LXObject*			PlayerLaserHitLogic(LXObject* pObject, float fTime);

	// 来自玩家子弹的碰撞检测
	LXObject*			EnemyLaserHitLogic(LXObject* pObject, float fTime);

	// 获取一个随机攻击对象
	LXObject*			GetRandomObject(bool isPlayer);

	// 敌机子弹变成水晶
	void				EnemyWeaponBecomeCrystal();

	// 玩家飞机受到攻击
	void				PlayerPlaneBeHit();

	//  晃动屏幕动作
	void				InitSwayScreen(float duration);

	// 清理敌机子弹
	void				CleanEnemyWeapon();


private:
	// 获取空闲对象
	LXObject*			getFreeObject(int type, int resID);
	// 增加飞机引用
	void				addPlane(LXObject *pObject);
	// 增加掉落物品引用
	void				addDropItem(LXObject *pObject);
	// 增加武器引用
	void				addWeapon(LXObject *pObject);
	// 回收空闲对象
	void				addToFreePool(LXObject *pObject);
	// 数据计算
	void				calcData();

private:

	// 晃动屏幕相关
	void			swayAllObj(float dt);		// 晃动所有对象

	
	float			m_duration;		// 总时间
	float			m_lasTime;		// 走过的世间
	int				m_xparam;		// x随机
	int				m_yparam;		// y随机
	bool			m_IsSway;		// 标记值
	
private:
	
	// 对象池用于逻辑遍历
	int				PlayerPlaneNum;				
	int				EnemyPlaneNum;
	int				DropItemNum;
	int				PlayerWeaponNum;
	int				EnemyWeaponNum;

	LXObject*		m_PlayerPlaneMap[100];		// 玩家飞机
	LXObject*		m_EnemyPlaneMap[500];		// AI飞机
	LXObject*		m_DropItemMap[1000];			// 掉落物品
	LXObject*		m_PlayerWeaponMap[500];		// 玩家武器
	LXObject*		m_EnemyWeaponMap[1000];		// AI武器
	
	// 对象池用于逻辑遍历
	std::map<int, std::list<LXObject*>>			m_dropItems;
	std::map<int, std::list<LXObject*>>			m_plane_bombs;
	std::map<int, std::list<LXObject*>>			m_plane_bosss;
	std::map<int, std::list<LXObject*>>			m_plane_enemys;
	std::map<int, std::list<LXObject*>>			m_plane_players;
	std::map<int, std::list<LXObject*>>			m_plane_wings;
	std::map<int, std::list<LXObject*>>			m_weapon_bombs;
	std::map<int, std::list<LXObject*>>			m_weapon_commons;
	std::map<int, std::list<LXObject*>>			m_weapon_lasers;
	std::map<int, std::list<LXObject*>>			m_weapon_parents;
	std::map<int, std::list<LXObject*>>			m_weapon_speeds;
	std::map<int, std::list<LXObject*>>			m_weapon_tracks;
	
	LXObject*						m_pPlayer;

	Node							*m_pGameSceneNode;
	Node							*m_pMainSceneNode;
};

LXObjectManager &ObjectManager();