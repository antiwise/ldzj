#pragma once
#include "LXWeapon.h"

class LXLaserWeapon : public LXWeapon
{
public:
	LXLaserWeapon();
	~LXLaserWeapon();

public:
	// 初始化
	virtual void				Init(int value);
	
	// 初始化子弹外观
	virtual void				InitRenderInfo(const _DBC_BULLET *pBulletData) {}

	// 主逻辑循环
	virtual void				Tick(float fDelta);

	// 清理
	virtual void				Destroy();
	
	// 动作初始化
	virtual void				InitAction();
	
	// 播放音效
	virtual void				PlayShootEffect();

	// 数据初始化
	virtual void				SetShooterLogic(LXShooterLogic* pLogic);

	void						SetPos(Vec2 value) { m_Pos = value; }

public:
	void				SetCanThrough(bool value) { m_bCanThrough = value;}
	bool				GetCanThrough() { return m_bCanThrough; }
	
protected:
	// 玩家子弹逻辑
	virtual void				playerWeaponLogic();

	// AI子弹逻辑
	virtual void				AIWeaponLogic();
	
	// 获取动画元素
	Sprite*						GetRenderElement(int index);

private:
	std::vector<Sprite*> m_SpritePool;	// 动画元素缓存池
	
	Vec2				m_Pos;				// 武器位置
	bool				m_bCanThrough;		// 是否有击穿效果
	float				m_fLastTick;		// 运行时间
	int					m_nPos;				// 位置
	int					m_nPosOffset;		// 位置变化
	int					m_nTickNum;			// 用户播放受击效果
	LXObject*			m_pObject;			// 击中的目标
	float				m_fHeight;			// 激光长度
	Rect				m_Rect;				// 激光图块大小
	std::string			m_ResPath;			// 资源路径

	ParticleEffect*		m_Effect_Fire;		// 开火粒子特效
	ParticleEffect*		m_Effect_Hited;		// 击中粒子特效
	float				m_fScale;			// 缩放

};
