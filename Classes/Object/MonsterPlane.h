//#pragma once
//#include <vector>
//#include <map>
//#include "cocos2d.h"
//#include "Plane.h"
//
//USING_NS_CC;
//
//enum class MonsterPathType
//{
//	LINE,			// 直线
//	ARC,			// 弧线
//	TRIANGLE,		// 三角形
//};
//
//
//class MonsterPlane : public ::Plane
//{
//public:
//	MonsterPlane(void);
//	~MonsterPlane(void);
//	
//	// 初始化
//	CREATE_FUNC(MonsterPlane);		// 创建
//
//	void				InitObject(std::string texture);		// 初始化精灵
//
//	void				InitAndRunAction();						// 初始化并且播放动作
//
//	void				RepeatMove();							// 循环移动
//
//	void				DeleteMe();
//
//	void				Tick(float dt);
//
//	// 属性
//	void				SetMonsterTagId(int nId){m_nID = nId;}
//	int					GetMonsterTagId(){return m_nID;}
//
//	Sprite*				GetMySprite(){return m_pMonster;}
//
//	void				SetSpeed(float fspeed){m_fSpeed = fspeed;}
//	float				GetSpeed(){return m_fSpeed;}
//	
//	void				SetIsAutoHide(bool bIs){m_isAutoHide = bIs;}
//	bool				GetIsAutoHide(){return m_isAutoHide;}
//	
//	void				SetPathType(MonsterPathType eType){ m_ePathType = eType; }
//
//	void				AddPathPos(Vec2 vecPos){ m_VecPos.push_back(vecPos);}
//	std::vector<Vec2>&	GetPathPos(){return m_VecPos;}
//
//	Rect				GetMonsterRect();
//
//	// 行为 ----------------------------
//	// 开火
//	void				OnpenFrie();
//
//	// 受击
//	void				BeHit(int damage);
//	
//	// 受伤回调
//	void				BeHitCallBacK();
//
//	// 死亡回调
//	void				DieCallback();
//
//private:
//	
//	int						m_nID;			// 编号
//
//	Sprite*					m_pMonster;
//
//	std::vector<Vec2>		m_VecPos;	// 敌机路径上的重要坐标点 【m_BegingPos.at(0) 默认为开始点,m_BegingPos.at(m_BegingPos.size()-1) 默认为结束点】
//
//	MonsterPathType			m_ePathType;		// 路线类型
//
//	float					m_fSpeed;			// 速度
//
//	bool					m_isAutoHide;		// 是否自动消逝
//	bool					m_isPlayerAction;	// 是否在播放动画
//
//	float					m_AllTime;			// 运行时间
//	float					m_nextFireTiem;		// 下次开火时间
//
//};
