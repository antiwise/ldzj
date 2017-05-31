#pragma once
#include "LXWeapon.h"

class LXMoveBy;
class LXTrackWeapon : public LXWeapon
{
public:
	LXTrackWeapon();
	~LXTrackWeapon();

public:
	// 初始化
	virtual void				Init(int value);

	// 主逻辑循环
	virtual void				Tick(float fDelta);

	// 清理
	virtual void				Destroy();
	
	// 动作初始化
	virtual void				InitAction();

	// 数据初始化
	virtual void				SetShooterLogic(LXShooterLogic* pLogic);

private:
	LXMoveBy			*m_pTrackAction;		// 跟踪动作

};
