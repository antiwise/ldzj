#pragma once
#include "LXPlane.h"

class LXWingPlane : public LXPlane
{
public:
	LXWingPlane();
	~LXWingPlane();

public:
	// 初始化
	virtual void				Init(int value);
	
	// 数据初始化
	virtual void				InitData();

	// 主逻辑循环
	virtual void				Tick(float fDelta);

	// 清理
	virtual void				Destroy();
	
public:
	// 初始化
	void						Init(int resID, bool bFlip);

};
