#pragma once
#include <string>
#include <vector>

enum
{
	shooter_type_common = 0,
	shooter_type_track,
	shooter_type_speed,
	shooter_type_parent,
	shooter_type_laser,
	shooter_type_bomb
};

class LXShooter
{
public:
	LXShooter();
	~LXShooter();

public:
	void				SetName(std::string& value) { m_Name = value;}
	std::string			GetName() { return m_Name; }

	void				SetType(int value) { m_nType = value;}
	int					GetType() { return m_nType; }

	void				SetResId(int value) { m_nResId = value;}
	int					GetResId() { return m_nResId; }

	void				SetPath(std::string& value) { m_Path = value;}
	std::string			GetPath() { return m_Path; }

	void				SetOffX(float value) { m_fOffX = value;}
	float				GetOffX() { return m_fOffX; }
	
	void				SetOffY(float value) { m_fOffY = value;}
	float				GetOffY() { return m_fOffY; }
	
	void				SetSpeed(float value) { m_fSpeed = value;}
	float				GetSpeed() { return m_fSpeed; }
	
	void				SetDegree(float value) { m_fDegree = value;}
	float				GetDegree() { return m_fDegree; }
	
	void				SetDuration(float value) { m_fDuration = value;}
	float				GetDuration() { return m_fDuration; }
	
	void				SetDelay(float value) { m_fDelay = value;}
	float				GetDelay() { return m_fDelay; }
	
	void				SetGroup(int value) { m_nGroup = value;}
	int					GetGroup() { return m_nGroup; }

	void				SetNum(int value) { m_nNum = value;}
	int					GetNum() { return m_nNum; }
	
	void				SetWaitTime(float value) { m_fWaitTime = value;}
	float				GetWaitTime() { return m_fWaitTime; }
	
	void				SetGroupWaitTime(float value) { m_fGroupWaitTime = value;}
	float				GetGroupWaitTime() { return m_fGroupWaitTime; }
	
	void				SetDegreeDelta(float value) { m_fDegreeDelta = value;}
	float				GetDegreeDelta() { return m_fDegreeDelta; }
	
	void				SetGroupDegreeDelta(float value) { m_fGroupDegreeDelta = value;}
	float				GetGroupDegreeDelta() { return m_fGroupDegreeDelta; }
	
	void				SetSpeedDelta(float value) { m_fSpeedDelta = value;}
	float				GetSpeedDelta() { return m_fSpeedDelta; }
	
	void				SetCanThrough(bool value) { m_bCanThrough = value;}
	bool				GetCanThrough() { return m_bCanThrough; }
	
	void				SetBurstTime(float value) { m_fBurstTime = value;}
	float				GetBurstTime() { return m_fBurstTime; }
	
	void				SetMinSpeed(float value) { m_fMinSpeed = value;}
	float				GetMinSpeed() { return m_fMinSpeed; }
	
	void				SetMaxSpeed(float value) { m_fMaxSpeed = value;}
	float				GetMaxSpeed() { return m_fMaxSpeed; }
	
	void				SetDeltaSpeed(float value) { m_fDeltaSpeed = value;}
	float				GetDeltaSpeed() { return m_fDeltaSpeed; }
	
	void				SetMaxDeltaDegree(float value) { m_fMaxDeltaDegree = value;}
	float				GetMaxDeltaDegree() { return m_fMaxDeltaDegree; }
	
	void				SetDeltaSpeedChange(bool value) { m_bDeltaSpeedChange = value;}
	bool				GetDeltaSpeedChange() { return m_bDeltaSpeedChange; }
	
private:
	std::string			m_Name;				// ����
	int					m_nType;			// ��������
	int					m_nResId;			// �������
	std::string			m_Path;				// ����·��
	float				m_fOffX;			// ê��offX
	float				m_fOffY;			// ê��offY
	float				m_fSpeed;			// �ٶ�
	float				m_fDegree;			// �Ƕ�
	float				m_fDuration;		// ���
	float				m_fDelay;			// �ӳ�
	int					m_nGroup;			// ����
	int					m_nNum;				// ����
	float				m_fWaitTime;		// ����֮��ȴ�ʱ��
	float				m_fGroupWaitTime;	// ����֮ǰ�ȴ�ʱ��
	float				m_fDegreeDelta;		// ����֮��Ƕȱ���
	float				m_fGroupDegreeDelta;// ����֮��Ƕȼӳ�
	float				m_fSpeedDelta;		// �ٶȼӳ�
	bool				m_bCanThrough;		// �Ƿ��л���Ч��
	float				m_fBurstTime;		// ����ʱ��
	float				m_fMinSpeed;		// ��С�ٶ�
	float				m_fMaxSpeed;		// ����ٶ�
	float				m_fDeltaSpeed;		// ʱ�����??
	float				m_fMaxDeltaDegree;	// ���Ƕȱ���
	bool				m_bDeltaSpeedChange;// �Ƿ�����ٶȱ任

};

class LXShooterLogic;
class LXObject;
class LXShooterGroup
{
	
public:
	LXShooterGroup();
	~LXShooterGroup();

public:
	// �������
	void				AddShooter(LXShooter* pShooter);

	// ��ʼ��
	void				InitShooterLogic(LXObject *pObject, std::vector<LXShooterLogic*> &vec);

private:

	// ������
	std::vector<LXShooter*>		m_Shooters;
};