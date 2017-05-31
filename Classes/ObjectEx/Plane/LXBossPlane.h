#pragma once
#include "LXEnemyPlane.h"

class LXBossPlane : public LXEnemyPlane
{
public:
	LXBossPlane();
	~LXBossPlane();

public:
	// 初始化
	virtual void				Init(int value);

	// 数据初始化
	virtual void				InitData();

	// 主逻辑循环
	virtual void				Tick(float fDelta);
	
	// 损毁特效
	virtual void				PlayDestroyEffect();

	// 损毁音效
	virtual void				PlayDestroySound();

	// 损毁逻辑
	virtual void				DestroyLogic();

	// 清理
	virtual void				Destroy();

	// 水晶掉落逻辑
	virtual void				DropCrystal();

	// 物品掉落逻辑
	virtual void				DropItem();

	// 检测boss血量
	virtual bool				CheckHP(int value);

	// 获取动画路径
	virtual std::string			GetStartAnimationPath();

	// 获取动画名称
	virtual std::string			GetStartAnimationName();

	virtual void				Hit(int damage);

private:
	// 拆分BOSS碰撞信息
	void						SplitBossCollideInfo();

protected:
	const _DBC_BOSS		*m_pBossData;	// 怪物数据
};
