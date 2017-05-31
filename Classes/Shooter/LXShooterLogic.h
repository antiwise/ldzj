#pragma once
#include <string>
#include "cocos2d.h"

USING_NS_CC;
class LXObject;
class LXWeapon;
class LXShooter;
class LXShooterLogic
{
public:
	LXShooterLogic();
	~LXShooterLogic();

public:
	// 设置父对象子弹的发射者
	void					SetParent(LXObject* pObject) { m_pParent = pObject; } 
	LXObject*				GetParent() { return m_pParent; }

	// 设置武器数据信息
	void					SetShooter(LXShooter* pShooters) { m_pShooter = pShooters; }
	LXShooter*				GetShooter() { return m_pShooter; }

	// 设置锚点信息
	void					SetAnchorPointX(float x) { m_fOffX = x; }
	void					SetAnchorPointY(float y) { m_fOffY = y; }
	float					GetAnchorPointX() { return m_fOffX; }
	float					GetAnchorPointY() { return m_fOffY; }
	void					ChangeAnchorPoint(float x, float y);

	// 设置发射特效
	void					SetEffect(std::string& effect) { m_Effect = effect; }

public:
	// 初始化武器行为
	void					InitAction();

	// 停止动作
	void					StopAction();

	// 开火
	void					Fire(float fDegree, float fSpeed);

	// 计算发射角度
	float					CalcDegree();

	// 播放特效
	void					PlayEffect();

private:
	int						m_nTag;

	LXObject*				m_pParent;		// 父对象
	LXWeapon*				m_pWeapon;		// 武器
	LXShooter*				m_pShooter;		// 武器数据

	float					m_fOffX;		// 锚点X
	float					m_fOffY;		// 锚点Y
	std::string				m_Effect;		// 子弹发射特效
	float					m_fEffectDelat; // 子弹特效播放间隔
};
