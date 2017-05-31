#pragma once
#include "LXWeapon.h"

class LXBombWeapon : public LXWeapon
{
public:
	LXBombWeapon();
	~LXBombWeapon();

public:
	// 初始化
	virtual void				Init(int value);

	// 主逻辑循环
	virtual void				Tick(float fDelta);

	// 清理
	virtual void				Destroy();
};
