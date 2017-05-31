#pragma once
#include "ObjectEx/LXObject.h"

class LXDropItem : public LXObject
{
public:
	LXDropItem();
	~LXDropItem();

public:
	// ��ʼ��
	virtual void				Init(int value);
	
	// ���߼�ѭ��
	virtual void				Tick(float fDelta);

	// ����
	virtual void				Destroy();

public:
	// ����Ŀ������״̬
	void						SetTarget(bool value) { m_bTarget = value; }

	// ��������Ŀ�����
	void						SetTargetRange(float value) { m_fTargetRange = value; }

	// ��������Ŀ��֮����ƶ��ٶ�
	void						SetTargetMoveSpeed(float value) { m_fTargetMoveSpeed = value; }
	
	// ��������Ŀ��֮����ƶ��ٶ�
	void						SetTargetDelayTime(float value) { m_fTargetDelay = value; }

public:
	// ��ʼ�������ƶ��߼�
	void						InitAction(float fDelay,	// �����ӳ�
		float speed, 										// �ƶ��ٶ�
		float maxSpeed, 									// ����ƶ��ٶ�
		float minSpeed, 									// ��С�ƶ��ٶ�
		float degree, 										// �Ƕ�
		float addSpeed, 									// �ٶȼӳ�
		float addDegree, 									// �Ƕȱ仯
		bool topSpeedLimit,									// ��������ƶ��ٶ�����
		bool needReverse,									// �Ƿ���Ҫ��������ƶ�
		int x,												// �ƶ�����x			
		int y,												// �ƶ�����y
		int w,												// �ƶ�����w
		int h);												// �ƶ�����h

private:
	bool			m_bActionDone;
	float			m_fActionDelay;
	float			m_fSpeed;
	float			m_fMaxSpeed;
	float			m_fMinSpeed;
	float			m_fDegree;
	float			m_fAddSpeed;
	float			m_fAddDegree;
	bool			m_bTopSpeedLimit;
	bool			m_bNeedReverse;
	int				m_nx;
	int				m_ny;
	int				m_nw;
	int				m_nh;

	bool			m_bTarget;			// �Ƿ�����Ŀ��
	float			m_fTargetRange;		// ����Ŀ�����
	float			m_fTargetMoveSpeed;	// ����Ŀ��֮����ƶ��ٶ�
	float			m_fTargetTime;		// ����Ŀ��֮����Ҫ���ƶ�ʱ��
	float			m_fMoveTime;		// ����Ŀ��֮���Ѿ��ƶ���ʱ��
	float			m_fTargetDelay;		// �����ӳ��ƶ�ʱ��

	bool			m_bArrived;			// �Ѿ�����Ŀ��λ��

};
