//#pragma once
//
//#include "cocos2d.h"
//#include "ui/CocosGUI.h"
//#include "cocostudio/CocoStudio.h"
//#include "Object/MonsterPlane.h"
//
//USING_NS_CC;
//using namespace ui;
//using namespace cocostudio;
//
//
//
//class EnemyLayer : public cocos2d::Layer
//{
//public:
//	EnemyLayer();
//	~EnemyLayer();
//
//	static EnemyLayer* GetMe(){return s_EnemyLayer;}
//public:
//
//	CREATE_FUNC(EnemyLayer);
//	
//	virtual bool init();
//
//	void		 Tick(float dt);
//
//	void						AddMonsterPlane(MonsterPlane* pMonster);
//
//	MonsterPlane*				GetMonsterPlaneByTagId(long	tagId);
//
//	void						RemoveMonsterPlane(MonsterPlane* pMonster);
//
//	int							GetMonsterNum(){return m_MapMonster.size();}
//
//	std::vector<int>			GetMonsterVector(){return m_vecMonter;}
//	void						RemoveVector(int num);
//
//	std::map<long,MonsterPlane*>&	GetMonsterPlane(){return m_MapMonster;}
//
//	long GetMaxMonsterTagId(){return ++mp_tagId;}
//
//private:
//
//	std::map<long,MonsterPlane*>		m_MapMonster;
//
//	std::vector<int>					m_vecMonter;
//		
//	static long mp_tagId;
//
//public:
//	static EnemyLayer* s_EnemyLayer;
//
//public:
//	void	SetMissionID(int ID);
//
//
//private:
//
//	//int		LoadEnemyInfo(const std::string fileName);
//
//	//std::vector<BatchInfo>	m_vBatchInfo;		// �ɻ���������
//	//std::vector<EnemyInfo>	m_vEneyInfo;		// �ɻ�������Ϣ
//	float					m_fPassTime;		// ��ʧʱ��
//	int						m_nBatchCount;		// ��ǰ�л�����
//	float					m_fDistance;		// ��ǰ�ƶ�����
//	
//};