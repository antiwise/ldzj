#include "SDKManager.h"
#include "DB/DBManager.h"
#include "TaskOption/TaskOptionMgr.h"
#include "Scene/MainScene.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "../../cocos2d/cocos/platform/android/jni/JniHelper.h"
#endif

#define  CLASS_NAME "org/cocos2dx/cpp/AppActivity"

USING_NS_CC;

LXSDKManager &SDKManager()
{
	static LXSDKManager mgr;
	return mgr;
}


LXSDKManager::LXSDKManager(void)
	:m_nItemID(0),
	m_bNeedTick(false),
	m_nPriceToFen(0),
	m_nPrice(0)
{
	
}

LXSDKManager::~LXSDKManager(void)
{
}

	
void LXSDKManager::BuyItem(std::string strName, int nID, int nPrice)
{
	m_nPriceToFen = nPrice*100; //转换为分
	m_nPrice = nPrice;
	m_nItemID = nID;

	//questPay
	requestPay(nID);
}


void LXSDKManager::requestPay(int id)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "openPayDialog", "(I)V"))
	{
		t.env->CallStaticVoidMethod(t.classID, t.methodID, id);
		t.env->DeleteLocalRef(t.classID);
	}
#else
	GetBuyResult(true,id);
#endif
}



	
void	LXSDKManager::GetBuyResult(bool isSuccess,int type)
{
	if (type == 101 || type == 102)
	{
		if (isSuccess)
		{
			DBManager().AddData(DATA_DIAMOND_NUM, 10000);		//钻石增加10000
			DBManager().AddData(DATA_DIAMOND_TOTAL, 10000);
		}
		return;
	}

	if (!isSuccess){
		m_nItemID = Buy_Null;
		return;
	}

	auto director = Director::getInstance();
	auto pSence = director->getRunningScene();
	auto mainlayer = pSence->getChildByName("mainSence");
	static_cast<MainScene*>(mainlayer)->PayContinue();
}