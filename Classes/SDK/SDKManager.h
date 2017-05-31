#pragma once
#include <map>
#include "cocos2d.h"
#include "GameDefine.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "jni/Java_org_cocos2dx_lib_Cocos2dxHelper.h"
#undef  CHANNEL
#define CHANNEL CHANNLE_ZHANGYU   //编译对应渠道版本修改宏定义
#endif

enum
{
	bonus_coin_monster = 2,		// 关卡
	bonus_coin_login,			// 每日登录
	bonus_coin_mission,			// 任务
	bonus_coin_buy				// 购买
};


class LXSDKManager
{
public:
	LXSDKManager(void);
	~LXSDKManager(void);

public:
	/************************************************************************/
	/* 
		id = 101 开机大礼包
		id = 102 退出游戏大礼包
	*/
	/************************************************************************/
	void		requestPay(int id);

	void		BuyItem(std::string strName, int nID, int nPrice); //物品ID ,名称
	
	void		GetBuyResult(bool isSuccess,int type);
private:
	int			m_nPriceToFen;
	int			m_nPrice;
	int			m_nItemID;
	bool		m_bNeedTick;

};


LXSDKManager &SDKManager();

