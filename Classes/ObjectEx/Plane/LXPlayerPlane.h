#pragma once
#include "LXPlane.h"

enum
{
	player_plane_type_airplane = 0,
	player_plane_type_airplane2,
	player_plane_type_airplane3,
	player_plane_type_zuheji,
	player_plane_type_erhaoji
};

class ParticleEffect;
class LXWingPlane;
class LXPlayerPlane : public LXPlane
{
public:
	LXPlayerPlane();
	~LXPlayerPlane();

public:
	// 初始化
	virtual void				Init(int value);
						
	// 数据初始化
	virtual void				InitData();

	// 主逻辑循环				
	virtual void				Tick(float fDelta);
								
	// 清理					
	virtual void				Destroy();
	
	// 伤害
	virtual void				Hit(int damage);

	// 创建僚机
	void						AddWingPlane(LXWingPlane* pWing);

	// 移动
	virtual void				Move(Vec2 delta);
	
	// 初始化射击逻辑
	virtual void				InitShootLogic();

	// 设置武器发射特效
	void						SetWeaponSpecialEffect();

	// 停止弹幕发射逻辑
	virtual void				StopShootLogic();

public:

	// 等级
	void						SetLevel(int value);
	int							GetLevel() { return m_nLevel; }

	// 飞机类型
	void						SetPlaneType(int type);
	int							GetPlaneType(){ return m_PlaneType; }

	// 拾取道具
	void						PickItem(LXObject *pObject);

	// 获取战机状态
	void						SetFireState(bool value);
	bool						GetFireState() { return m_bAutoShoot; }
	
	// 无敌时间
	void						SetInvincibleTime(float ft){ m_fInvincible = ft; }
	float						GetInvincibleTime(){ return m_fInvincible; }

	// 护罩时间
	void						SetProtectTime(float ft){ m_fProtected = ft; }
	float						GetProtectTime() { return m_fProtected;}

	// 设置暴走状态
	void						SetCrazyState(bool isCrazy);


	//  创建护盾
	void						CreatProtectEffect();

	//	护罩使用
	void						ShowProtectEffect();

	// 重置护罩特效
	void						ClearUpProtectEffect();

	// 飞入屏幕
	void						RunIntoScreen();

	// 重生后重新结算护盾
	void						RefreshProtectWithReLife();

	// 设置玩家飞机的显示或者隐藏
	void						ShowPlane(bool bShow);

	//  战斗胜利 并 冲锋
	void						FighterWinAndCharge();
	
	//  战斗胜利动画
	void						FighterWin();

	// 回调
	void						FighterWinAndChargeCallback();

	// 回调
	void						FighterWinCallback();

	// 是否在自动移动状态
	bool						IsAutoMove(){ return m_IsAutoMove; };

	// 激活必杀
	void						ActiveBombPlane();
	bool						IsBombPlaneActive() { return m_bBombActive; }  

protected:
	// 计算玩家数据ID
	int							calcUserDataID();
	
	// 计算玩家武器数据ID
	int							calcShooterID();	

	// 初始化弹幕
	void						initShooter();


protected:
	int					m_nLevel;			// 主机当前等级
	int					m_PlaneType;		// 飞机类型
	int					m_nWeaponLevel;		// 武器等级
	bool				m_bAutoShoot;		// 自动射击
	bool				m_isRenascenceing;	// 是否在重生
	
	bool				m_IsAutoMove;		// 是否在自动移动
	bool				m_bCrazyMode;		// 暴走模式
	float				m_fInvincible;		// 无敌状态
	float				m_fProtected;		// 护盾状态
	float				m_fCrazyTime;		// 暴走时间
	float				m_RenascenceTime;	// 重生时间
	
	int					m_nWingPosParam1;	// 僚机位置参数1
	int					m_nWingPosParam2;	// 僚机位置参数1
	float				m_fWingPosParam3;	// 僚机位置参数1
	float				m_fWingPosParam4;	// 僚机位置参数1

	std::vector<LXWingPlane*>	m_WingPlanes;	// 僚机
	std::vector<int>	m_nWeaponInfo;			// 武器数据

	float m_WingTempXPos[10];					// 僚机跟随偏移
	float m_WingTempYPos[10];					// 僚机跟随偏移

	ParticleEffect*		m_ProtectEffect;		// 护盾粒子特效
	ParticleEffect*		m_EffectY;				// 护盾粒子特效

	unsigned int		m_nShooterEffect;		// 子弹特效
	bool				m_bBombActive;			// 必杀激活
	float				m_fBombTime;			// 必杀激活时长

};
