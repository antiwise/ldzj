//TDDBC_Struct.h
//基本数据库结构


#pragma once

//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
//							数据表格式定义								//
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
// 怪物表
#define DBC_ENEMY		( 101 )
struct _DBC_ENEMY
{
	int				nID;				// 索引ID
	int				nType;				// 类型0小怪 1BOSS
	int				nResID;				// 资源ID 首列 0 plane表 首列 1 boss表
	const char*		szShooter;			// 弹幕对应data/shooters
	int				nHP;				// 血量
	int				nScore;				// 分数
	int				nDiamondNum;		// 掉落数量（宝石）
	int				nItemID;			// 掉落道具
};

// BOSS表
#define DBC_BOSS		( 102 )
struct _DBC_BOSS
{
	int				nID;				// 索引ID
	const char*		szFileName;			// Animation文件名称
	const char*		szAnimation;		// 出场动作
	const char*		szRect1;			// 碰撞范围
	const char*		szRect2;			// 碰撞范围
	const char*		szDeadEffect;		// 死亡特效
	const char*		szDeadAnimation;	// 死亡动作数组
};

// 小怪表
#define DBC_PLANE		( 103 )
struct _DBC_PLANE
{
	int				nID;				// 索引ID
	const char*		szAnimation;		// 出场动作
	int				nRect1;				// 碰撞范围1
	int				nRect2;				// 碰撞范围2
	int				nRect3;				// 碰撞范围3
	int				nRect4;				// 碰撞范围4
	const char*		szDeadEffect;		// 死亡特效
	const char*		szDeadAnimation;	// 死亡动作数组
};

// 玩家表
#define DBC_USER		( 104 )
struct _DBC_USER
{
	int				nID;				// 索引ID
	int				nRect1;				// 碰撞范围1
	int				nRect2;				// 碰撞范围2
	int				nRect3;				// 碰撞范围3
	int				nRect4;				// 碰撞范围4
	const char*		szShooter;			// 弹幕
};

// 成就表
#define DBC_ACHIEVE		(105)
struct _DBC_ACHIEVE
{
	int				nID;				// 索引ID
	const char*		szName;				// 名称
	const char*		szDescribe;			// 描述
	int				nType;				// 奖励类型
	int				nNum;				// 奖励数量
};


// 子弹表
#define  DBC_BULLET		(106)
struct _DBC_BULLET
{
	int				nID;				// 索引ID
	int				nType;				// 子弹类型0Animation 1particle
	const char*		szName;				// 子弹名称
	int				nRect1;				// 碰撞范围1
	int				nRect2;				// 碰撞范围2
	int				nRect3;				// 碰撞范围3
	int				nRect4;				// 碰撞范围4
	int				nValue1;			// 子弹是否需要指定方向
	int				nValue2;			// 无用
	int				nValue3;			// 子弹伤害加成
};

// 战机升级表
#define DBC_UPGRADE		(107)
struct _DBC_UPGRADE
{
	int				nID;				// 索引ID
	int				nCost;				// 水晶消耗
	const char*		szDescribe;			// 描述
};

// 音乐音效表
#define DBC_SOUND		(108)
struct _DBC_SOUND
{
	int		nSoundID;				// 音效ID
	const	char* szDesc;			// 描述
	const	char* szPath;			// 文件路径
	int		nType;					// 类型:0背景音乐,1音效
	int		nLoop;					// 是否循环播放
};

// 关卡信息表
#define DBC_MISSION_INFO (109)
struct	_DBC_MISSION_INFO
{
	int				nMissionID;				// 关卡ID（也为索引）
	char*			szStarImage;			// 星球图片
	char*			szLockStarNameImage;	// 星球名图片-未解锁
	char*			szStarNameImage;		// 星球名称-已解锁
	int				nStarX;					// 星球X坐标
	int				nStarY;					// 星球Y坐标
	char*			szBossFile;				// boss文件名称
	char*			szBossXoffset;			// boss位置x偏移
	char*			szBossYoffset;			// boss位置y偏移
	char*			szBossScale;			// 缩放值
	char*			szBossNameImage;		// boss名称图片
	char*			szBossInfoIamge;		// boss介绍图片
	int				nBossHp;				// 血量
	int				nBossAttack;			// 攻击
	int				nBossSpeed;				// 速度

	int				GetMissionID(){return nMissionID;}							// 关卡ID（也为索引）
	char*			GetStarImage(){ return szStarImage; }						// 星球图片
	char*			GetLockStarNameImage(){return szLockStarNameImage;}			// 星球名图片-未解锁
	char*			GetStarNameImage(){ return szStarNameImage; }				// 星球名称-已解锁
	int				GetStarPosX(){return nStarX;}								// 星球X坐标
	int				GetStarPosY(){return nStarY;}								// 星球Y坐标
	char*			GetBossFileName(){ return szBossFile; }						// boss文件名称
	char*			GetBossOffsetX(){ return szBossXoffset; }					// boss位置x偏移
	char*			GetBossOffsetY(){ return szBossYoffset; }					// boss位置y偏移
	char*			GetBossScale(){ return szBossScale; }						// 缩放值
	char*			GetBossNameImage(){ return szBossNameImage; }				// boss名称图片
	char*			GetBossInfoImage(){return szBossInfoIamge;}					// boss介绍图片
	int				GetBossHp(){ return nBossHp; }								// 血量
	int				GetBossAttack(){return nBossAttack;}						// 攻击
	int				GetBossSpeed(){ return nBossSpeed; }						// 速度

};


/*---------------------------------------------------------------------*/
// 公用参数表
#define DBC_PARAM_INFO (205)
struct	_DBC_PARAM_INFO
{
	static const int	INDEX_ID_COLUMN = 0;

	int				nTypeID;				// ID
	int				nIntValue;				// int型值
	const char*		szDesc;					// 字符型值

	int				getID() const { return nTypeID;}
	int				getIntValue() const { return nIntValue;}
	const char*		getStringValue() const { return szDesc;}
};

// 计费
#define DBC_CHARGING_INFO (206)
struct	_DBC_CHARGING_INFO
{
	int				nTypeID;				// ID 同 BuyType 对应
	char*			szName;					// 名称
	int				nIntValue;				// 钱

	int				getID() const { return nTypeID;}
	int				getIntValue() const { return nIntValue;}
};

//屏蔽字
#define DBC_TALK_FILTER	(207)
struct _DBC_TALK_FILTER
{
	int				nID;			// ID
	const char* 	szString;		// 过滤词汇(即：不能说的词汇)

	int getID() const {return nID;}
	const char* getString() const {return szString;}
};