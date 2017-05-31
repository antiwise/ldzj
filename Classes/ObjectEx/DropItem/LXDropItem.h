#pragma once
#include "ObjectEx/LXObject.h"

class LXDropItem : public LXObject
{
public:
	LXDropItem();
	~LXDropItem();

public:
	// 初始化
	virtual void				Init(int value);
	
	// 主逻辑循环
	virtual void				Tick(float fDelta);

	// 清理
	virtual void				Destroy();

public:
	// 设置目标锁定状态
	void						SetTarget(bool value) { m_bTarget = value; }

	// 设置锁定目标距离
	void						SetTargetRange(float value) { m_fTargetRange = value; }

	// 设置锁定目标之后的移动速度
	void						SetTargetMoveSpeed(float value) { m_fTargetMoveSpeed = value; }
	
	// 设置锁定目标之后的移动速度
	void						SetTargetDelayTime(float value) { m_fTargetDelay = value; }

public:
	// 初始化道具移动逻辑
	void						InitAction(float fDelay,	// 动作延迟
		float speed, 										// 移动速度
		float maxSpeed, 									// 最大移动速度
		float minSpeed, 									// 最小移动速度
		float degree, 										// 角度
		float addSpeed, 									// 速度加成
		float addDegree, 									// 角度变化
		bool topSpeedLimit,									// 存在最大移动速度上限
		bool needReverse,									// 是否需要反向减速移动
		int x,												// 移动区域x			
		int y,												// 移动区域y
		int w,												// 移动区域w
		int h);												// 移动区域h

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

	bool			m_bTarget;			// 是否锁定目标
	float			m_fTargetRange;		// 锁定目标距离
	float			m_fTargetMoveSpeed;	// 锁定目标之后的移动速度
	float			m_fTargetTime;		// 锁定目标之后需要的移动时间
	float			m_fMoveTime;		// 锁定目标之后已经移动的时间
	float			m_fTargetDelay;		// 锁定延迟移动时间

	bool			m_bArrived;			// 已经到达目标位置

};
