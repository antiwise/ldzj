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
	std::string			m_Name;				// 名称
	int					m_nType;			// 武器类型
	int					m_nResId;			// 武器外观
	std::string			m_Path;				// 武器路径
	float				m_fOffX;			// 锚点offX
	float				m_fOffY;			// 锚点offY
	float				m_fSpeed;			// 速度
	float				m_fDegree;			// 角度
	float				m_fDuration;		// 间隔
	float				m_fDelay;			// 延迟
	int					m_nGroup;			// 波次
	int					m_nNum;				// 数量
	float				m_fWaitTime;		// 数量之间等待时间
	float				m_fGroupWaitTime;	// 波次之前等待时间
	float				m_fDegreeDelta;		// 数量之间角度变量
	float				m_fGroupDegreeDelta;// 波次之间角度加成
	float				m_fSpeedDelta;		// 速度加成
	bool				m_bCanThrough;		// 是否有击穿效果
	float				m_fBurstTime;		// 爆发时间
	float				m_fMinSpeed;		// 最小速度
	float				m_fMaxSpeed;		// 最大速度
	float				m_fDeltaSpeed;		// 时间变量??
	float				m_fMaxDeltaDegree;	// 最大角度变量
	bool				m_bDeltaSpeedChange;// 是否进行速度变换

};

class LXShooterLogic;
class LXObject;
class LXShooterGroup
{
	
public:
	LXShooterGroup();
	~LXShooterGroup();

public:
	// 添加武器
	void				AddShooter(LXShooter* pShooter);

	// 初始化
	void				InitShooterLogic(LXObject *pObject, std::vector<LXShooterLogic*> &vec);

private:

	// 武器组
	std::vector<LXShooter*>		m_Shooters;
};