#pragma once
#include <vector>
#include "cocos2d.h"
#include "LXObject.h"
#include "Effect/ParticleEffect.h"
#include <map>

USING_NS_CC;
//=============================================================================
//// ���������
// ��������������ж��󣬲�������������Ĵ�����ֻ����������ض��߼�����


enum
{
	object_group_plane,
	object_group_sprite
};

enum
{
	object_type_none			= 0x0000,		
	object_type_dropItem		= 0x0001,		// ������Ʒ

	object_type_plane_bomb		= 0x1001,		// ���շɻ�
	object_type_plane_boss		= 0x1002,		// AI_BOSS
	object_type_plane_enemy		= 0x1003,		// AI_Enemy
	object_type_plane_player	= 0x1004,		// ���
	object_type_plane_wing		= 0x1005,		// �Ż�

	object_type_weapon_bomb		= 0x2001,		// ���շɻ�����
	object_type_weapon_common	= 0x2002,		// ��������
	object_type_weapon_laser	= 0x2003,		// ��������
	object_type_weapon_parent	= 0x2004,		// ��������
	object_type_weapon_speed	= 0x2005,		// ��������
	object_type_weapon_track	= 0x2006,		// ׷������
};

class LXObjectManager
{
public:
	LXObjectManager();
	~LXObjectManager();

	void				Tick(float fDelta);	
	
	// ���ýڵ�
	void				SetGameSceneNode(Node *pNode) { m_pGameSceneNode = pNode; }
	void				SetMainSceneNode(Node *pNode) { m_pMainSceneNode = pNode; }

	Node*				GetGameSceneNode() { return m_pGameSceneNode; }
	Node*				GetMainSceneNode() { return m_pMainSceneNode; }

public:
	// ��������
	void				PreCreateObject(int resID, Node *pParent, int type, bool isPlayer, int num);

	// ��������
	LXObject*			CreateObject(int resID, Node *pParent, int type, bool isPlayer);

	//�������ж���
	void				CleanAllObject();

	// ��ȡ��ǩ
	int					GetTag();

	// ��ȡ��������
	int					GetEnemyNum();

	// ע�����
	void				RigisterPlayer(LXObject* pObject) { m_pPlayer = pObject; }
	
	// ��ȡ��Ҷ���
	LXObject*			GetPlayer() { return m_pPlayer; }

	// ��������ӵ�����ײ���
	bool				PlayerHitLogic(LXObject* pObject);

	// �л��ӵ���ײ
	bool				EnemyHitLogic(LXObject* pObject);
	
	// ��ɱ��ײ
	bool				BombHitLogic(LXObject* pObject);

	// ��������ӵ�����ײ���
	LXObject*			PlayerLaserHitLogic(LXObject* pObject, float fTime);

	// ��������ӵ�����ײ���
	LXObject*			EnemyLaserHitLogic(LXObject* pObject, float fTime);

	// ��ȡһ�������������
	LXObject*			GetRandomObject(bool isPlayer);

	// �л��ӵ����ˮ��
	void				EnemyWeaponBecomeCrystal();

	// ��ҷɻ��ܵ�����
	void				PlayerPlaneBeHit();

	//  �ζ���Ļ����
	void				InitSwayScreen(float duration);

	// ����л��ӵ�
	void				CleanEnemyWeapon();


private:
	// ��ȡ���ж���
	LXObject*			getFreeObject(int type, int resID);
	// ���ӷɻ�����
	void				addPlane(LXObject *pObject);
	// ���ӵ�����Ʒ����
	void				addDropItem(LXObject *pObject);
	// ������������
	void				addWeapon(LXObject *pObject);
	// ���տ��ж���
	void				addToFreePool(LXObject *pObject);
	// ���ݼ���
	void				calcData();

private:

	// �ζ���Ļ���
	void			swayAllObj(float dt);		// �ζ����ж���

	
	float			m_duration;		// ��ʱ��
	float			m_lasTime;		// �߹�������
	int				m_xparam;		// x���
	int				m_yparam;		// y���
	bool			m_IsSway;		// ���ֵ
	
private:
	
	// ����������߼�����
	int				PlayerPlaneNum;				
	int				EnemyPlaneNum;
	int				DropItemNum;
	int				PlayerWeaponNum;
	int				EnemyWeaponNum;

	LXObject*		m_PlayerPlaneMap[100];		// ��ҷɻ�
	LXObject*		m_EnemyPlaneMap[500];		// AI�ɻ�
	LXObject*		m_DropItemMap[1000];			// ������Ʒ
	LXObject*		m_PlayerWeaponMap[500];		// �������
	LXObject*		m_EnemyWeaponMap[1000];		// AI����
	
	// ����������߼�����
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