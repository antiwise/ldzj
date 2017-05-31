//#pragma once
//#include <vector>
//#include <map>
//#include "cocos2d.h"
//#include "Plane.h"
//
//USING_NS_CC;
//
//enum class MonsterPathType
//{
//	LINE,			// ֱ��
//	ARC,			// ����
//	TRIANGLE,		// ������
//};
//
//
//class MonsterPlane : public ::Plane
//{
//public:
//	MonsterPlane(void);
//	~MonsterPlane(void);
//	
//	// ��ʼ��
//	CREATE_FUNC(MonsterPlane);		// ����
//
//	void				InitObject(std::string texture);		// ��ʼ������
//
//	void				InitAndRunAction();						// ��ʼ�����Ҳ��Ŷ���
//
//	void				RepeatMove();							// ѭ���ƶ�
//
//	void				DeleteMe();
//
//	void				Tick(float dt);
//
//	// ����
//	void				SetMonsterTagId(int nId){m_nID = nId;}
//	int					GetMonsterTagId(){return m_nID;}
//
//	Sprite*				GetMySprite(){return m_pMonster;}
//
//	void				SetSpeed(float fspeed){m_fSpeed = fspeed;}
//	float				GetSpeed(){return m_fSpeed;}
//	
//	void				SetIsAutoHide(bool bIs){m_isAutoHide = bIs;}
//	bool				GetIsAutoHide(){return m_isAutoHide;}
//	
//	void				SetPathType(MonsterPathType eType){ m_ePathType = eType; }
//
//	void				AddPathPos(Vec2 vecPos){ m_VecPos.push_back(vecPos);}
//	std::vector<Vec2>&	GetPathPos(){return m_VecPos;}
//
//	Rect				GetMonsterRect();
//
//	// ��Ϊ ----------------------------
//	// ����
//	void				OnpenFrie();
//
//	// �ܻ�
//	void				BeHit(int damage);
//	
//	// ���˻ص�
//	void				BeHitCallBacK();
//
//	// �����ص�
//	void				DieCallback();
//
//private:
//	
//	int						m_nID;			// ���
//
//	Sprite*					m_pMonster;
//
//	std::vector<Vec2>		m_VecPos;	// �л�·���ϵ���Ҫ����� ��m_BegingPos.at(0) Ĭ��Ϊ��ʼ��,m_BegingPos.at(m_BegingPos.size()-1) Ĭ��Ϊ�����㡿
//
//	MonsterPathType			m_ePathType;		// ·������
//
//	float					m_fSpeed;			// �ٶ�
//
//	bool					m_isAutoHide;		// �Ƿ��Զ�����
//	bool					m_isPlayerAction;	// �Ƿ��ڲ��Ŷ���
//
//	float					m_AllTime;			// ����ʱ��
//	float					m_nextFireTiem;		// �´ο���ʱ��
//
//};
