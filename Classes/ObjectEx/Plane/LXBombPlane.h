#pragma once
#include "LXPlane.h"

class LXBombPlane : public LXPlane
{
public:
	LXBombPlane();
	~LXBombPlane();

public:
	// 初始化
	virtual void				Init(int value);

	// 主逻辑循环
	virtual void				Tick(float fDelta);

	// 清理
	virtual void				Destroy();

public:
	// 设置状态
	void				setBombStatus(bool value) { m_bBomb = value; }


private:
	bool				m_bBomb;
	float				m_fBombTime;			// 必杀激活时长

};
