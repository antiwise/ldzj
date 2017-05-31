#pragma once
#include "LXWeapon.h"

class LXParentWeapon : public LXWeapon
{
public:
	LXParentWeapon();
	~LXParentWeapon();

public:
	// 初始化
	virtual void				Init(int value);

	// 主逻辑循环
	virtual void				Tick(float fDelta);

	// 清理
	virtual void				Destroy();

};
