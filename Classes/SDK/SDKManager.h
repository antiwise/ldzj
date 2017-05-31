#pragma once
#include <map>
#include "cocos2d.h"
#include "GameDefine.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "jni/Java_org_cocos2dx_lib_Cocos2dxHelper.h"
#undef  CHANNEL
#define CHANNEL CHANNLE_ZHANGYU   //�����Ӧ�����汾�޸ĺ궨��
#endif

enum
{
	bonus_coin_monster = 2,		// �ؿ�
	bonus_coin_login,			// ÿ�յ�¼
	bonus_coin_mission,			// ����
	bonus_coin_buy				// ����
};


class LXSDKManager
{
public:
	LXSDKManager(void);
	~LXSDKManager(void);

public:
	/************************************************************************/
	/* 
		id = 101 ���������
		id = 102 �˳���Ϸ�����
	*/
	/************************************************************************/
	void		requestPay(int id);

	void		BuyItem(std::string strName, int nID, int nPrice); //��ƷID ,����
	
	void		GetBuyResult(bool isSuccess,int type);
private:
	int			m_nPriceToFen;
	int			m_nPrice;
	int			m_nItemID;
	bool		m_bNeedTick;

};


LXSDKManager &SDKManager();

