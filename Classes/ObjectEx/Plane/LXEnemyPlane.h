#pragma once
#include "LXPlane.h"
#include "DataBase/LXDBC_Struct.h"
#include "GameDefine.h"


class LXEnemyPlane : public LXPlane
{
public:
	LXEnemyPlane();
	~LXEnemyPlane();

public:
	// 初始化
	virtual void				Init(int value);
	
	// 重生
	virtual void				Relive();
	
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

	// 水晶掉落逻辑
	virtual void				DropCrystal();

	// 清理
	virtual void				Destroy();
	
	// 获取动画路径
	virtual std::string			GetStartAnimationPath();

	// 获取动画名称
	virtual std::string			GetStartAnimationName();

	// 伤害
	virtual void				Hit(int damage);

protected:
	const _DBC_ENEMY		*m_pEnemyData;	// 怪物数据
	const _DBC_PLANE		*m_pPlaneData;	// 怪物数据

	Sprite					*m_pBloodBack;	// 血条背景
	ProgressTimer			*m_pBloodBar;	// 血条

};
