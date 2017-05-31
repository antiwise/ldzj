#pragma once
#include "LXWeapon.h"

class LXCommonWeapon : public LXWeapon
{
public:
	LXCommonWeapon();
	~LXCommonWeapon();

public:
	// 初始化
	virtual void				Init(int value);

	// 主逻辑循环
	virtual void				Tick(float fDelta);

	// 清理
	virtual void				Destroy();

	// 动作初始化
	virtual void				InitAction();

};
