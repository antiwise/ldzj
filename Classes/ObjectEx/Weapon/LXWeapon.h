#pragma once
#include "ObjectEx/LXObject.h"

enum 
{
	bullet_type_animiaton = 0,
	bullet_type_particle
};

class LXShooterLogic;
struct _DBC_BULLET;
class LXWeapon : public LXObject
{
public:
	LXWeapon();
	~LXWeapon();

public:
	// 初始化
	virtual void				Init(int value);
	
	// 重生
	virtual void		Relive();

	// 初始化子弹外观
	virtual void				InitRenderInfo(const _DBC_BULLET *pBulletData);

	// 主逻辑循环
	virtual void				Tick(float fDelta);

	// 清理
	virtual void				Destroy();

	// 动作初始化
	virtual void				InitAction() {}

	// 播放音效
	virtual void				PlayShootEffect();

public:
	// 计算武器朝向
	void						CalcBulletDegree();

	// 数据初始化
	virtual void				SetShooterLogic(LXShooterLogic* pLogic) { m_pShooterLogic = pLogic; }
	LXShooterLogic*				GetShooterLogic() { return m_pShooterLogic; }

	// 是否动画
	bool						IsAnimation() { return m_bAnimation; }


protected:
	// 玩家子弹逻辑
	virtual void				playerWeaponLogic();

	// AI子弹逻辑
	virtual void				AIWeaponLogic();


protected:
	LXShooterLogic			*m_pShooterLogic;		// 武器发射逻辑
	bool					m_bResDir;				// 子弹资源是否有朝向信息
	bool					m_bHit;					// 是否已经击中目标
	bool					m_bAnimation;			// 是否动画
	const _DBC_BULLET		*m_pBulletData;
};
