#pragma once
#include <vector>
#include "cocos2d.h"
#include "Common/OBB.h"

USING_NS_CC;

#ifndef SHOWOBB
#define SHOWOBB 0  //0 ����ʵ��ײ���� 1��ʾ��ײ����
#endif


class LXAnimation;
class LXAction;
class LXAnimationLogic;
class LXShooterLogic;
class LXShooterGroup;
class ParticleEffect;
// ���ж������
class LXObject : public Node
{
public:
	LXObject();
	~LXObject();

	//=========================================================================
	/// ��Ⱦ��س�Ա
public:
	virtual void		draw(Renderer* renderer, const Mat4 &transform, uint32_t flags);

	// ��ʼ��
	virtual void		Init(int value);
	
	// ����
	virtual void		Relive() {}

	virtual void		InitAction(std::string& eventID, int tag);
	
	// ��ʼ������߼�
	virtual void		InitShootLogic();

	// ֹͣ��Ļ�����߼�
	virtual void		StopShootLogic();

	// ��ȡ��ԴID
	int					GetResID() { return m_nResID; }

	// ��ȡ������С
	Size				GetAnimationSize() { return m_pAnimationSize; } 

	// ���Ŷ���
	void				PlayAnimaton(std::string& path, const std::string& name, std::string nameFix = "", int type = 2, bool flip = false);
	
	// ��һ��Ҫ���ŵĶ���
	void				SetNextAnimation(const std::string& value);

	// ���������ص�
	void				OnAnnimationEnd();

	// ��ʼ����Ļ
	void				InitShooters(LXShooterGroup* pShooters);
	
	// �ƶ�������
	void				MoveShooter(int index, float x, float y);

	// �����ӵ������߼�
	void				InitShootLogicByIndex(std::vector<int> & vec);

	// �����ܻ�����
	void				PlayHitEffect();

	// ��ȡ��ײ��Χ
	Rect				GetObjectBoundBox();
	
	// ������ͣ״̬
	void						SetPause(bool value) { m_bPause = value;} 
	bool						IsPause() { return m_bPause; }

#if SHOWOBB
protected:
    void onDraw(const Mat4 &transform);
#endif

protected:

	CustomCommand		m_CustomCommand;			// �Զ������
	Node				*m_pRenderNode;				// ��Ⱦ�ӽڵ�
	int					m_nResID;					// ��Դ���
	LXAnimationLogic	*m_pAnimationLogic;			// �����߼�����
	Size				m_pAnimationSize;			// ��ǰ������С
	Point				m_pAnchorPoint;				// ��ǰ����ê��
	std::string			m_NextAnimation;			// ��һ��Ҫ���ŵĶ���
	
	//=========================================================================
	/// ������س�Ա
protected:
	LXAction			*m_pAction;			// AI������

	//=========================================================================
	///�߼���س�Ա
public:
	
	// ���߼�ѭ��
	virtual void		Tick(float fDelta);
	
	// ������Ч
	virtual void		PlayEffect(std::string effectName);

	// �����Ч
	virtual void		PlayDestroyEffect();

	// �����Ч
	virtual void		PlayDestroySound() {}

	// ����
	virtual void		Destroy();

	// ������ײ��Ϣ
	void				SetCollideInfo(Rect rect);
	Rect				GetCollideRect() { return m_Rect; }
	::OBB* 				GetCollideInfo();

	// ��������
	void				SetObjectType(int value) { m_nObjectType = value; }
	int					GetObjectType() { return m_nObjectType; }

	// Zֵ
	int					GetZOrder() { return m_nZOrder; }
	void				SetZOrder(int value) { m_nZOrder = value; }

	// �Ƿ����
	void				SetIsPlayer(bool value) { m_bIsPlayer = value; }
	bool				IsPlayer() { return m_bIsPlayer; }

	// Ѫ�����
	void				SetHP(int value) { m_nCurrentHP = value; }
	int					GetHP() { return m_nCurrentHP; }
	void				ChangeHP(int value) { m_nCurrentHP += value; }
	bool				IsAlive() { return m_nCurrentHP > 0; }
	virtual bool		CheckHP(int value) { return m_nCurrentHP <= value; }

	// ������
	void				SetAttack(int value) { m_nAttack = value; }
	int					GetAttack() { return m_nAttack; }
	
	// ������ϵ��
	void				SetAttackFactor(int value) { m_nAttackFactor = value; }
	int					GetAttackFactor() { return m_nAttackFactor; }
	
	// ����
	void				SetDefense(int value) { m_nDefense = value; }
	int					GetDefense() { return m_nDefense; }

	// �ӵ�����Ƕ�
	void				SetBulletDegree(float value) { m_fBulletDegree = value; }
	float				GetBulletDegree() { return m_fBulletDegree; }
	
	// �ӵ������ٶ�
	void				SetBulletSpeed(float value) { m_fBulletSpeed = value; }
	float				GetBulletSpeed() { return m_fBulletSpeed; }
	
	// �ӵ�������
	void				SetBulletDelta(float value) { m_fBulletDelta = value; }
	float				GetBulletDelta() { return m_fBulletDelta; }
	 
	// �ӵ��ٶȼӳ�
	void				SetBulletDeltaSpeed(float value) { m_fBulletDeltaSpeed = value; }
	float				GetBulletDeltaSpeed() { return m_fBulletDeltaSpeed; }

	// �ӵ���С�ٶ�
	void				SetBulletMinSpeed(float value) { m_fBulletMinSpeed = value; }
	float				GetBulletMinSpeed() { return m_fBulletMinSpeed; }

	// �ӵ�������
	void				SetBulletMaxSpeed(float value) { m_fBulletMaxSpeed = value; }
	float				GetBulletMaxSpeed() { return m_fBulletMaxSpeed; }

	// �ӵ�������
	void				SetBulletSpeedChange(bool value) { m_bBulletSpeedChange = value; }
	bool				GetBulletSpeedChange() { return m_bBulletSpeedChange; }
	
	// �ӵ�������
	void				SetBulletMaxDeltaDegree(float value) { m_fBulletMaxDeltaDegree = value; }
	float				GetBulletMaxDeltaDegree() { return m_fBulletMaxDeltaDegree; }

	// ��Դ·��
	std::string			GetPath() { return m_Path; }
	std::string			GetNameFix() { return m_NameFix; }

	// ������
	bool				NeedClean() { return m_bCleanFlag; }
	void				SetCleanUp(bool value) { m_bCleanFlag = value; }

	bool				SetDeadFlag(bool bvalue) { return m_bDeadFlag = bvalue;}
	
	// UI����
	bool				IsUIObject() { return m_bUIObject; }
	void				SetUIObject(bool value) { m_bUIObject = value; }

	// �˺�
	virtual void		Hit(int damage);

protected:
	Vec2				m_pLastPos;			// �ϴ�λ��
	ParticleEffect*		m_pEffect;			// �ҽ���Ч
	int					m_nZOrder;			// Zֵ
	int					m_nObjectType;		// ��������
	bool				m_bCleanFlag;		// ������
	bool				m_bDeadFlag;		// �����߼���־
	int					m_nCurrentHP;		// ��ǰѪ��
	int					m_nTotalHP;			// ��Ѫ��
	bool				m_bIsPlayer;		// �Ƿ����
	bool				m_bUIObject;		// UI��־

	/// �������
	int					m_nAttack;			// ������
	int					m_nAttackFactor;	// ����ϵ��
	int					m_nDefense;			// ����

	// common
	float				m_fBulletDegree;	// �ӵ�����Ƕ�
	float				m_fBulletSpeed;		// �ӵ������ٶ�
	float				m_fBulletDelta;		// �ӵ�������

	// speed
	float				m_fBulletDeltaSpeed;// �ӵ��ٶȼӳ�
	float				m_fBulletMinSpeed;	// �ӵ���С�ٶ�
	float				m_fBulletMaxSpeed;	// �ӵ�����ٶ�
	bool				m_bBulletSpeedChange;// �ӵ��ٶȱ仯

	// track
	float				m_fBulletMaxDeltaDegree;	// ���Ƕȱ仯


	std::string			m_Path;				// �ļ�����
	std::string			m_NameFix;			// ������������

	::OBB*				m_pOBB;				// ��ײ��Χ��
	Rect				m_Rect;
	int					m_nSize;			// ����Χ�гߴ�
	std::vector<LXShooterLogic*>	m_Shooters;		// ��Ļ��Ϣ
	float				m_fLastHitTime;		// ��¼�˺���Ч����ʱ��
	LXShooterGroup*		m_pShooters;
	bool				m_bPause;				// ��ͣ
};
