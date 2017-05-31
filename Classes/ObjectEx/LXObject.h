#pragma once
#include <vector>
#include "cocos2d.h"
#include "Common/OBB.h"

USING_NS_CC;

#ifndef SHOWOBB
#define SHOWOBB 0  //0 不现实碰撞区域 1显示碰撞区域
#endif


class LXAnimation;
class LXAction;
class LXAnimationLogic;
class LXShooterLogic;
class LXShooterGroup;
class ParticleEffect;
// 所有对象基类
class LXObject : public Node
{
public:
	LXObject();
	~LXObject();

	//=========================================================================
	/// 渲染相关成员
public:
	virtual void		draw(Renderer* renderer, const Mat4 &transform, uint32_t flags);

	// 初始化
	virtual void		Init(int value);
	
	// 重生
	virtual void		Relive() {}

	virtual void		InitAction(std::string& eventID, int tag);
	
	// 初始化射击逻辑
	virtual void		InitShootLogic();

	// 停止弹幕发射逻辑
	virtual void		StopShootLogic();

	// 获取资源ID
	int					GetResID() { return m_nResID; }

	// 获取动画大小
	Size				GetAnimationSize() { return m_pAnimationSize; } 

	// 播放动画
	void				PlayAnimaton(std::string& path, const std::string& name, std::string nameFix = "", int type = 2, bool flip = false);
	
	// 下一个要播放的动作
	void				SetNextAnimation(const std::string& value);

	// 动画结束回调
	void				OnAnnimationEnd();

	// 初始化弹幕
	void				InitShooters(LXShooterGroup* pShooters);
	
	// 移动发射器
	void				MoveShooter(int index, float x, float y);

	// 启动子弹发射逻辑
	void				InitShootLogicByIndex(std::vector<int> & vec);

	// 播放受击动画
	void				PlayHitEffect();

	// 获取碰撞范围
	Rect				GetObjectBoundBox();
	
	// 设置暂停状态
	void						SetPause(bool value) { m_bPause = value;} 
	bool						IsPause() { return m_bPause; }

#if SHOWOBB
protected:
    void onDraw(const Mat4 &transform);
#endif

protected:

	CustomCommand		m_CustomCommand;			// 自定义绘制
	Node				*m_pRenderNode;				// 渲染子节点
	int					m_nResID;					// 资源编号
	LXAnimationLogic	*m_pAnimationLogic;			// 动画逻辑测试
	Size				m_pAnimationSize;			// 当前动画大小
	Point				m_pAnchorPoint;				// 当前动画锚点
	std::string			m_NextAnimation;			// 下一个要播放的动作
	
	//=========================================================================
	/// 动作相关成员
protected:
	LXAction			*m_pAction;			// AI管理器

	//=========================================================================
	///逻辑相关成员
public:
	
	// 主逻辑循环
	virtual void		Tick(float fDelta);
	
	// 播放特效
	virtual void		PlayEffect(std::string effectName);

	// 损毁特效
	virtual void		PlayDestroyEffect();

	// 损毁音效
	virtual void		PlayDestroySound() {}

	// 清理
	virtual void		Destroy();

	// 增加碰撞信息
	void				SetCollideInfo(Rect rect);
	Rect				GetCollideRect() { return m_Rect; }
	::OBB* 				GetCollideInfo();

	// 对象类型
	void				SetObjectType(int value) { m_nObjectType = value; }
	int					GetObjectType() { return m_nObjectType; }

	// Z值
	int					GetZOrder() { return m_nZOrder; }
	void				SetZOrder(int value) { m_nZOrder = value; }

	// 是否玩家
	void				SetIsPlayer(bool value) { m_bIsPlayer = value; }
	bool				IsPlayer() { return m_bIsPlayer; }

	// 血量相关
	void				SetHP(int value) { m_nCurrentHP = value; }
	int					GetHP() { return m_nCurrentHP; }
	void				ChangeHP(int value) { m_nCurrentHP += value; }
	bool				IsAlive() { return m_nCurrentHP > 0; }
	virtual bool		CheckHP(int value) { return m_nCurrentHP <= value; }

	// 攻击力
	void				SetAttack(int value) { m_nAttack = value; }
	int					GetAttack() { return m_nAttack; }
	
	// 攻击力系数
	void				SetAttackFactor(int value) { m_nAttackFactor = value; }
	int					GetAttackFactor() { return m_nAttackFactor; }
	
	// 防御
	void				SetDefense(int value) { m_nDefense = value; }
	int					GetDefense() { return m_nDefense; }

	// 子弹发射角度
	void				SetBulletDegree(float value) { m_fBulletDegree = value; }
	float				GetBulletDegree() { return m_fBulletDegree; }
	
	// 子弹发射速度
	void				SetBulletSpeed(float value) { m_fBulletSpeed = value; }
	float				GetBulletSpeed() { return m_fBulletSpeed; }
	
	// 子弹发射间隔
	void				SetBulletDelta(float value) { m_fBulletDelta = value; }
	float				GetBulletDelta() { return m_fBulletDelta; }
	 
	// 子弹速度加成
	void				SetBulletDeltaSpeed(float value) { m_fBulletDeltaSpeed = value; }
	float				GetBulletDeltaSpeed() { return m_fBulletDeltaSpeed; }

	// 子弹最小速度
	void				SetBulletMinSpeed(float value) { m_fBulletMinSpeed = value; }
	float				GetBulletMinSpeed() { return m_fBulletMinSpeed; }

	// 子弹发射间隔
	void				SetBulletMaxSpeed(float value) { m_fBulletMaxSpeed = value; }
	float				GetBulletMaxSpeed() { return m_fBulletMaxSpeed; }

	// 子弹发射间隔
	void				SetBulletSpeedChange(bool value) { m_bBulletSpeedChange = value; }
	bool				GetBulletSpeedChange() { return m_bBulletSpeedChange; }
	
	// 子弹发射间隔
	void				SetBulletMaxDeltaDegree(float value) { m_fBulletMaxDeltaDegree = value; }
	float				GetBulletMaxDeltaDegree() { return m_fBulletMaxDeltaDegree; }

	// 资源路径
	std::string			GetPath() { return m_Path; }
	std::string			GetNameFix() { return m_NameFix; }

	// 清理标记
	bool				NeedClean() { return m_bCleanFlag; }
	void				SetCleanUp(bool value) { m_bCleanFlag = value; }

	bool				SetDeadFlag(bool bvalue) { return m_bDeadFlag = bvalue;}
	
	// UI对象
	bool				IsUIObject() { return m_bUIObject; }
	void				SetUIObject(bool value) { m_bUIObject = value; }

	// 伤害
	virtual void		Hit(int damage);

protected:
	Vec2				m_pLastPos;			// 上次位置
	ParticleEffect*		m_pEffect;			// 挂接特效
	int					m_nZOrder;			// Z值
	int					m_nObjectType;		// 对象类型
	bool				m_bCleanFlag;		// 清理标记
	bool				m_bDeadFlag;		// 死亡逻辑标志
	int					m_nCurrentHP;		// 当前血量
	int					m_nTotalHP;			// 总血量
	bool				m_bIsPlayer;		// 是否玩家
	bool				m_bUIObject;		// UI标志

	/// 攻击相关
	int					m_nAttack;			// 攻击力
	int					m_nAttackFactor;	// 攻击系数
	int					m_nDefense;			// 防御

	// common
	float				m_fBulletDegree;	// 子弹发射角度
	float				m_fBulletSpeed;		// 子弹发射速度
	float				m_fBulletDelta;		// 子弹发射间隔

	// speed
	float				m_fBulletDeltaSpeed;// 子弹速度加成
	float				m_fBulletMinSpeed;	// 子弹最小速度
	float				m_fBulletMaxSpeed;	// 子弹最大速度
	bool				m_bBulletSpeedChange;// 子弹速度变化

	// track
	float				m_fBulletMaxDeltaDegree;	// 最大角度变化


	std::string			m_Path;				// 文件名称
	std::string			m_NameFix;			// 动画名称修正

	::OBB*				m_pOBB;				// 碰撞包围盒
	Rect				m_Rect;
	int					m_nSize;			// 最大包围盒尺寸
	std::vector<LXShooterLogic*>	m_Shooters;		// 弹幕信息
	float				m_fLastHitTime;		// 记录伤害特效播放时间
	LXShooterGroup*		m_pShooters;
	bool				m_bPause;				// 暂停
};
