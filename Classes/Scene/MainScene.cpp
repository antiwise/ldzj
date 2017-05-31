#include "MainScene.h"
#include "GameScene.h"
#include "Animation/AnimationManager.h"
#include "Action/ActionManager.h"
#include "DataBase/DataBase.h"
#include "UI/MenuLayer.h"
#include "UI/UpgradeLayer.h"
#include "ObjectEx/LXObjectManager.h"
#include "Effect/LXEffectManager.h"
#include "Shooter/LXShooterManager.h"
#include "DB/DBManager.h"
#include "UI/RankingLayer.h"
#include "UI/MissionLayer.h"
#include "Time/TimeSystem.h"
#include "Sound/LXSoundSystem.h"
#include "GameDefine.h"
#include "UI/BuyBoxLayer.h"
#include "Common/FileUtils.h"
#include "Common/LXUtils.h"
#include "TaskOption/TaskOptionMgr.h"
#include "UI/MyAnimation/TuHaoJin.h"
#include "UI/MyAnimation/NiceGirl.h"
#include "UI/SettingLayer.h"
#include "Common/LXUtils.h"
#include "UI/Login.h"
#include "SDK/SDKManager.h"
#include "UI/LoadingLayer.h"
#include "UI/TaskTips.h"

bool clean = false;
std::string dijiFileName = "dijiall";

MainScene::MainScene():Layer(),
	pBg_1(nullptr),
	pBg_2(nullptr),
	pBg_3(nullptr),
	pBg_4(nullptr),
	pBg_5(nullptr),
	pAnimate_1_1(nullptr),
	pAnimate_1_2(nullptr),
	pAnimate_2_1(nullptr),
	pAnimate_4_1(nullptr),
	pAnimate_5_1(nullptr),
	pAnimate_5_2(nullptr),
	pTimer(nullptr),
	pTimer21(nullptr),
	pTimer22(nullptr),
	pTimer31(nullptr),
	pTimer32(nullptr),
	pTimer4(nullptr),
	pTimer5(nullptr),
	pSkipText(nullptr),
	pSkipBtn(nullptr),
	m_nProc(proc_loadui),
	m_nUILoadingProc(ui_load_MainLayer),
	m_nProgress(0),
	m_nTargetProgress(0),
	m_fProgressDelta(0.01),
	m_bLoadOver(false),
	m_pPreLoad(nullptr)
{
	for(int i = 0; i < GAME_ZORDER_NUM; i++)
	{
		Layer *pLayer = Layer::create();
		this->addChild(pLayer, i);
		m_LayerList.push_back(pLayer);
	}

	m_pLoadingLayer = nullptr;		// 加载界面			
	m_pGameScene = nullptr;			// 游戏场景
	m_pMenuLayer = nullptr;			// 主界面
	m_pUpgradeLayer = nullptr;		// 装备强化界面
	m_pRankingLayer = nullptr;		// 排行界面
	m_pMissionLayer = nullptr;		// 关卡选择界面
	m_pBuyBoxLayer = nullptr;		// 
	m_pSettingLayer = nullptr;		// 设置界面
	m_pLogin = nullptr;
	m_pNiceGirl = nullptr;
	m_pKeyBoardLayer = nullptr;
	m_pTaskTipsLayer = nullptr;

	m_bCleanGameSence = false;		// 战斗场景清理标识

	m_bIsBossMisson = false;		// 是否BOSS挑战模式
	m_nMission = 0;					// 关卡编号
	m_nDifficulty = 0;				// 关卡难度 0:easy 1:normal 2:hard
	m_nDefaultPlane = 0;			// 默认飞机
	m_bWin = false;

	m_GoToInterfaceType = INTERFACE_ID::INTERFACE_MAIN;	// 是否转到关卡选择界面

}

MainScene::~MainScene()
{

}

bool MainScene::init()
{
	// 1. super init first
	if ( !Layer::init() )
	{
		return false;
	}

	m_bIsBossMisson = false;
	m_nMission = 1;
	m_nDifficulty = 0;
	m_nDefaultPlane = DBManager().GetCurrPlaneID();
	m_pGameScene = nullptr;
	m_pUpgradeLayer = nullptr;
	m_pMenuLayer = nullptr;
	m_pRankingLayer = nullptr;
	m_pMissionLayer = nullptr;
	m_bCleanGameSence = false;

	// 设置随机种子
	srand( TimeSystem().GetFrameTime() );

	m_pLoadingLayer = LoadingLayer::create();
	addChild(m_pLoadingLayer, 9999);
	
	ObjectManager().SetMainSceneNode(this);
	
	this->schedule(schedule_selector(MainScene::Tick));

	//添加按键监听层
	m_pKeyBoardLayer = KeyBoardLayer::create();
	addChild(m_pKeyBoardLayer,GAME_ZORDER_KEYBOARD);

	return true;
}


void	MainScene::End()
{
	log("MainScene::End");
	delete m_pPreLoad;
	m_pPreLoad = nullptr;
}

void	MainScene::LoadUI()
{
	//主界面
	if (m_nUILoadingProc == ui_load_MainLayer)
	{
		m_pMenuLayer = MenuLayer::create();
		this->addChild(m_pMenuLayer);
		m_pMenuLayer->setVisible(false);
	}
	else if (m_nUILoadingProc == ui_load_UpgradeLayer)
	{
		// 强化界面
		m_pUpgradeLayer = UpgradeLayer::create();
		this->addChild(m_pUpgradeLayer);
		m_pUpgradeLayer->setVisible(false);	
	}
	else if (m_nUILoadingProc == ui_load_RankingLayer)
	{
		// 排行界面
		m_pRankingLayer = RankingLayer::create();
		this->addChild(m_pRankingLayer);
		m_pRankingLayer->setVisible(false);
	}
	else if (m_nUILoadingProc == ui_load_MissionLayer)
	{
		// 关卡界面
		m_pMissionLayer = MissionLayer::create();
		this->addChild(m_pMissionLayer);
		m_pMissionLayer->setVisible(false);
	}
	else if (m_nUILoadingProc == ui_load_BuyBoxLayer)
	{
		//购买界面
		m_pBuyBoxLayer = BuyBoxLayer::create();
		this->addChild(m_pBuyBoxLayer,GAME_ZORDER_SUSPEND);
		m_pBuyBoxLayer->setVisible(false);
	}
	else if (m_nUILoadingProc == ui_load_SettingLayer)
	{
		m_pSettingLayer = SettingLayer::create();
		this->addChild(m_pSettingLayer);
		m_pSettingLayer->setVisible(false);
	}
	else if (m_nUILoadingProc == ui_load_Prop)
	{
		//其他界面......
		TuHaoJin::SetMainSceneNode(this);


		// 道具初始化
		for(int i = 0; i < 12; i++)
		{
			std::string itemInfo = g_PreItem[i];
			std::vector<std::string> vec;
			Utils().split(itemInfo, vec, ",");

			if (vec.size() == 2)
			{
				ObjectManager().PreCreateObject(atoi(vec[0].c_str()), this, object_type_dropItem, false, atoi(vec[1].c_str()));
			}
		}
	}
	else if (m_nUILoadingProc == ui_load_NiceGirl)
	{
		/*---------------------------------------------------------------------*/
		// 加载妹子
		m_pNiceGirl = NiceGirl::create();
		if (m_pNiceGirl)
		{
			m_pNiceGirl->setVisible(false);
			m_pNiceGirl->setPosition( Utils().GetFixedPosion( 0, 0) );
			this->addChild( m_pNiceGirl,GAME_ZORDER_NEW_PLAYER );
		}

	}else if ( m_nUILoadingProc == ui_load_TaskTips )
	{
		m_pTaskTipsLayer = TaskTips::create();
		if( m_pTaskTipsLayer )
		{
			m_pTaskTipsLayer->setVisible(false);
			this->addChild( m_pTaskTipsLayer, GAME_ZORDER_NEW_PLAYER );
		}
	}
	else if ( m_nUILoadingProc == ui_load_Login )
	{
		/*---------------------------------------------------------------------*/
		// 加载每日登录
		m_pLogin = Login::create();
		if (m_pLogin)
		{
			this->addChild( m_pLogin, GAME_ZORDER_NEW_PLAYER );
			m_pLogin->setVisible(false);
		}

		m_bLoadOver = true;
		// 创建资源解析线程
		m_pPreLoad = new std::thread(&MainScene::ParseThread,this,this);
		m_pPreLoad->detach(); 
	}

	m_nUILoadingProc++;
}

void	MainScene::PlayStory()
{

	LXFileUtils::AddSpriteFrameCache("story.plist","story.png");
	auto desionSize = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();
	const float		middleX = desionSize.width/2;
	const float		middleY = desionSize.height/2;
	const float		animateX = middleX + 35;
	const float		animateY = middleY + 240;
	const float		fadeintime = 1;
	const float		delaytime  = fadeintime + 3; //故事2
	const float		delaytime2 = delaytime + 4;  //故事3
	const float		delaytime3 = delaytime2 + 5; //切换屏幕

	pBg_1 = Sprite::createWithSpriteFrameName("story_01.png");
	pBg_2 = Sprite::createWithSpriteFrameName("story_02.png");
	pBg_3 = Sprite::createWithSpriteFrameName("story_03-1.png");
	pBg_4 = Sprite::createWithSpriteFrameName("story_04.png");
	pBg_5 = Sprite::createWithSpriteFrameName("story_05.png");

	pAnimate_1_1 = Sprite::createWithSpriteFrameName("story_01-1.png");
	pAnimate_1_2 = Sprite::createWithSpriteFrameName("story_01-2.png");
	pAnimate_2_1 = Sprite::createWithSpriteFrameName("story_02-1.png");
	cocos2d::Vector<SpriteFrame*> frames;
	frames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("story_03-1.png"));
	frames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("story_03-2.png"));
	frames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("story_03-3.png"));
	Animation* pAnimation_3 = Animation::createWithSpriteFrames(frames,0.1,10);
	auto pAnimate_3 = CCAnimate::create(pAnimation_3);
	pAnimate_4_1 = Sprite::createWithSpriteFrameName("story_04-1.png");
	pAnimate_5_1 = Sprite::createWithSpriteFrameName("story_05-1.png");
	pAnimate_5_2 = Sprite::createWithSpriteFrameName("story_05-2.png");

	auto pStory_1  = Sprite::createWithSpriteFrameName("story_01-z1.png");
	auto pStory_21  = Sprite::createWithSpriteFrameName("story_02-z1.png");
	auto pStory_22  = Sprite::createWithSpriteFrameName("story_02-z2.png");
	auto pStory_31  = Sprite::createWithSpriteFrameName("story_03-z1.png");
	auto pStory_32  = Sprite::createWithSpriteFrameName("story_03-z2.png");
	auto pStory_4  = Sprite::createWithSpriteFrameName("story_04-z1.png");
	auto pStory_5  = Sprite::createWithSpriteFrameName("story_05-z1.png");

	//故事一
	//背景
	pBg_1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	pBg_1->setPosition(middleX,middleY + pBg_1->getContentSize().height/2);
	this->addChild(pBg_1);
	pBg_1->setOpacity(0);
	FadeIn* pFadeIn = FadeIn::create(fadeintime);
	pBg_1->runAction(pFadeIn);

	//动画部件激光
	pAnimate_1_1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	pAnimate_1_1->setPosition(animateX ,  animateY );
	this->addChild(pAnimate_1_1);
	pAnimate_1_1->setOpacity(0);
	FadeIn* pFadeIn1 = FadeIn::create(fadeintime);
	RotateBy* pRotaeby = RotateBy::create(2,30);
	Sequence* pSeq = Sequence::create(pFadeIn1,pRotaeby,nullptr);
	pAnimate_1_1->runAction(pSeq);

	//动画部件圆球
	pAnimate_1_2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	pAnimate_1_2->setPosition(animateX ,  animateY );
	this->addChild(pAnimate_1_2);
	pAnimate_1_2->setOpacity(0);
	FadeIn* pFadeIn2 = FadeIn::create(fadeintime);
	ScaleBy* pScaleBy = ScaleBy::create(1.5,1.2);
	Sequence* pseq1 = Sequence::create(pFadeIn2,pScaleBy,pScaleBy->reverse(),nullptr);
	pAnimate_1_2->runAction(pseq1);


	//文字进度条
	pTimer =  ProgressTimer::create(pStory_1);
	pTimer->setPosition(std::min(desionSize.width,pBg_1->getContentSize().width) - 10 - pStory_1->getContentSize().width,middleY + 10);
	pTimer->setAnchorPoint(Vec2(0,0));//Vec2(0,0)
	pTimer->setType(kCCProgressTimerTypeBar);
	pTimer->setMidpoint(ccp(0,1)); //设置起点
	pTimer->setBarChangeRate(ccp(1,0));//设置X,Y轴是否随着进度变化
	pTimer->setPercentage(0);
	this->addChild(pTimer);
	DelayTime* pDelay  = DelayTime::create(fadeintime);
	ProgressFromTo* fromto = ProgressFromTo::create(3 ,0, 100);
	Sequence* seq2 = Sequence::create(pDelay,fromto,nullptr);
	pTimer->runAction(seq2);

	//显示跳过按钮
	Sprite* pSpriteSkip = Sprite::create();
	this->addChild(pSpriteSkip);
	DelayTime* pDelaySkip = DelayTime::create(fadeintime);
	Sequence* pSeqSkip = Sequence::create(pDelaySkip, CallFunc::create(CC_CALLBACK_0(MainScene::ShowSkipButton,this)),nullptr);
	pSpriteSkip->runAction(pSeqSkip);



	//故事二
	//背景
	pBg_2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	pBg_2->setPosition(middleX - pBg_2->getContentSize().width/2 + 33,middleY - pBg_2->getContentSize().height/2);
	this->addChild(pBg_2);
	pBg_2->setOpacity(0);
	FadeIn* pFadeIn_s2_1 = FadeIn::create(fadeintime);
	DelayTime* pDelays2_1 =DelayTime::create(delaytime);
	Sequence* pSeqs2_1 = Sequence::create(pDelays2_1,pFadeIn_s2_1,nullptr);
	pBg_2->runAction(pSeqs2_1);

	//动画部件激光
	pAnimate_2_1->setAnchorPoint(Vec2(0.5,0.66));
	pAnimate_2_1->setPosition(pBg_2->getContentSize().width - 205,  pBg_2->getContentSize().height -150 );
	pBg_2->addChild(pAnimate_2_1);
	pAnimate_2_1->setOpacity(0);
	pAnimate_2_1->setRotation(-15);
	DelayTime* pDelays2_2 =DelayTime::create(delaytime);
	FadeIn* pFadeIns2_2 = FadeIn::create(fadeintime);
	RotateBy* pRotaebys2_2 = RotateBy::create(3,45);
	Sequence* pSeqs2_2 = Sequence::create(pDelays2_2,pFadeIns2_2,pRotaebys2_2,nullptr);
	pAnimate_2_1->runAction(pSeqs2_2);

	//文字进度条
	pTimer21 =  ProgressTimer::create(pStory_21);
	pTimer21->setPosition(pBg_2->getContentSize().width - 60 - pStory_21->getContentSize().width, pBg_2->getContentSize().height - 320);
	pTimer21->setAnchorPoint(Vec2(0,0));
	pTimer21->setType(kCCProgressTimerTypeBar);
	pTimer21->setMidpoint(ccp(0,1)); //设置起点
	pTimer21->setBarChangeRate(ccp(1,0));//设置X,Y轴是否随着进度变化
	pTimer21->setPercentage(0);
	pBg_2->addChild(pTimer21);
	DelayTime* pDelay21  = DelayTime::create(delaytime);
	ProgressFromTo* fromto21 = ProgressFromTo::create(2 ,0, 100);
	Sequence* seq21 = Sequence::create(pDelay21,fromto21,nullptr);
	pTimer21->runAction(seq21);

	pTimer22 =  ProgressTimer::create(pStory_22);
	pTimer22->setPosition(pBg_2->getContentSize().width - 80 - pStory_22->getContentSize().width, pBg_2->getContentSize().height - 350);
	pTimer22->setAnchorPoint(Vec2(0,0));
	pTimer22->setType(kCCProgressTimerTypeBar);
	pTimer22->setMidpoint(ccp(0,1)); //设置起点
	pTimer22->setBarChangeRate(ccp(1,0));//设置X,Y轴是否随着进度变化
	pTimer22->setPercentage(0);
	pBg_2->addChild(pTimer22);
	DelayTime* pDelay22  = DelayTime::create(delaytime+2);
	ProgressFromTo* fromto22 = ProgressFromTo::create(2 ,0, 100);
	Sequence* seq22 = Sequence::create(pDelay22,fromto22,nullptr);
	pTimer22->runAction(seq22);

	//故事三

	//背景
	pBg_3->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	pBg_3->setPosition(middleX + pBg_3->getContentSize().width/2 - 39,middleY - pBg_3->getContentSize().height/2 -10);
	this->addChild(pBg_3);
	pBg_3->setOpacity(0);
	FadeIn* pFadeIn_s3_1 = FadeIn::create(fadeintime);
	DelayTime* pDelays3_1 =DelayTime::create(delaytime2);
	Sequence* pSeqs3_1 = Sequence::create(pDelays3_1,pFadeIn_s3_1,pAnimate_3,nullptr);
	pBg_3->runAction(pSeqs3_1);

	//文字
	pTimer31 =  ProgressTimer::create(pStory_31);
	pTimer31->setPosition( middleX + pBg_3->getContentSize().width/2 - 150, middleY - pBg_3->getContentSize().height/2 - 130);
	pTimer31->setAnchorPoint(Vec2(0,0));
	pTimer31->setType(kCCProgressTimerTypeBar);
	pTimer31->setMidpoint(ccp(0,1)); //设置起点
	pTimer31->setBarChangeRate(ccp(1,0));//设置X,Y轴是否随着进度变化
	pTimer31->setPercentage(0);
	this->addChild(pTimer31);
	DelayTime* pDelay31  = DelayTime::create(delaytime2);
	ProgressFromTo* fromto31 = ProgressFromTo::create(2 ,0, 100);
	Sequence* seq31 = Sequence::create(pDelay31,fromto31,nullptr);
	pTimer31->runAction(seq31);

	pTimer32 =  ProgressTimer::create(pStory_32);
	pTimer32->setPosition(middleX + pBg_3->getContentSize().width/2 - 150, middleY - pBg_3->getContentSize().height/2 - 160);
	pTimer32->setAnchorPoint(Vec2(0,0));
	pTimer32->setType(kCCProgressTimerTypeBar);
	pTimer32->setMidpoint(ccp(0,1)); //设置起点
	pTimer32->setBarChangeRate(ccp(1,0));//设置X,Y轴是否随着进度变化
	pTimer32->setPercentage(0);
	this->addChild(pTimer32);
	DelayTime* pDelay32  = DelayTime::create(delaytime2+2);
	ProgressFromTo* fromto32 = ProgressFromTo::create(1 ,0, 100);
	Sequence* seq32 = Sequence::create(pDelay32,fromto32,nullptr);
	pTimer32->runAction(seq32);


	//调用清理图片接口
	Sprite* pSprite = Sprite::create();
	this->addChild(pSprite);
	DelayTime* pDelay40 = DelayTime::create(delaytime2+2+2);
	Sequence* pSeq40 = Sequence::create(pDelay40, CallFunc::create(CC_CALLBACK_0(MainScene::HideStoryPage1,this)),nullptr);
	pSprite->runAction(pSeq40);

	//故事四
	//背景
	pBg_4->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	pBg_4->setPosition(middleX,middleY + pBg_4->getContentSize().height/2);
	this->addChild(pBg_4);
	pBg_4->setOpacity(0);
	DelayTime* pDelay41 = DelayTime::create(delaytime3);
	FadeIn* pFadeIn41 = FadeIn::create(fadeintime);
	Sequence* pSeq41 = Sequence::create(pDelay41,pFadeIn41,nullptr);
	pBg_4->runAction(pSeq41);

	//特效
	pAnimate_4_1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	pAnimate_4_1->setPosition(125,330);
	pBg_4->addChild(pAnimate_4_1);
	pAnimate_4_1->setOpacity(0);
	DelayTime* pDelays4_1 = DelayTime::create(delaytime3);
	FadeIn* pFadeIns4_1 = FadeIn::create(fadeintime);
	FadeOut* pFadeOuts4_1 = FadeOut::create(fadeintime);
	Sequence* pSeqs4_1 = Sequence::create(pFadeIns4_1,pFadeOuts4_1,nullptr);
	Repeat* pRepeats4_1 = Repeat::create(pSeqs4_1,2);
	Sequence* pSeqs4_2 = Sequence::create(pDelays4_1,pRepeats4_1,nullptr);
	pAnimate_4_1->runAction(pSeqs4_2);

	//文字进度条
	pTimer4 =  ProgressTimer::create(pStory_4);
	pTimer4->setPosition(std::min(desionSize.width,pBg_4->getContentSize().width) - 10 - pStory_4->getContentSize().width,middleY + 10);
	pTimer4->setAnchorPoint(Vec2(0,0));
	pTimer4->setType(kCCProgressTimerTypeBar);
	pTimer4->setMidpoint(ccp(0,1)); //设置起点
	pTimer4->setBarChangeRate(ccp(1,0));//设置X,Y轴是否随着进度变化
	pTimer4->setPercentage(0);
	this->addChild(pTimer4);
	DelayTime* pDelay43  = DelayTime::create(delaytime3 + fadeintime);
	ProgressFromTo* fromto43 = ProgressFromTo::create(3 ,0, 100);
	Sequence* seq43 = Sequence::create(pDelay43,fromto43,nullptr);
	pTimer4->runAction(seq43);

	//故事五
	//背景
	pBg_5->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	pBg_5->setPosition(middleX,middleY - pBg_5->getContentSize().height/2);
	this->addChild(pBg_5);
	pBg_5->setOpacity(0);
	DelayTime* pDelay51 = DelayTime::create(delaytime3 + fadeintime + 3);
	FadeIn* pFadeIn51 = FadeIn::create(fadeintime);
	Sequence* pSeq51 = Sequence::create(pDelay51,pFadeIn51,nullptr);
	pBg_5->runAction(pSeq51);

	// 飞机
	pAnimate_5_1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	pAnimate_5_1->setPosition(desionSize.width, middleY - pBg_5->getContentSize().height/2 - 250);
	this->addChild(pAnimate_5_1);
	DelayTime* pDelay52 = DelayTime::create(delaytime3 + fadeintime + 3 + 1);
	MoveTo* pMoveto52 = MoveTo::create(0.5,Vec2(middleX,middleY - pBg_5->getContentSize().height/2));
	Sequence* pSeq52 = Sequence::create(pDelay52,pMoveto52,nullptr);
	pAnimate_5_1->runAction(pSeq52);

	//飞机火焰
	pAnimate_5_2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	pAnimate_5_2->setPosition(desionSize.width, middleY - pBg_5->getContentSize().height/2 - 250);
	this->addChild(pAnimate_5_2);
	DelayTime* pDelay53 = DelayTime::create(delaytime3 + fadeintime + 3 + 1);
	MoveTo* pMoveto53 = MoveTo::create(0.5,Vec2(middleX,middleY - pBg_5->getContentSize().height/2));
	MoveBy* pMoveby53 = MoveBy::create(0.1,Vec2(5,-3));
	Sequence* pseq53rp = Sequence::create(pMoveby53,pMoveby53->reverse(),nullptr);
	Repeat*	pRepeat53 = Repeat::create(pseq53rp,10);
	Sequence* pSeq53 = Sequence::create(pDelay53, pMoveto53,pRepeat53,CallFunc::create(CC_CALLBACK_0(MainScene::CleanStory,this)), nullptr);
	pAnimate_5_2->runAction(pSeq53);


	//文字
	pTimer5 = ProgressTimer::create(pStory_5);
	pTimer5->setPosition(pBg_5->getPosition().x - pBg_5->getContentSize().width/2, 15);
	pTimer5->setAnchorPoint(Vec2(0,0));
	pTimer5->setType(kCCProgressTimerTypeBar);
	pTimer5->setMidpoint(ccp(0,1)); //设置起点
	pTimer5->setBarChangeRate(ccp(1,0));//设置X,Y轴是否随着进度变化
	pTimer5->setPercentage(0);
	this->addChild(pTimer5);
	DelayTime* pDelay54  = DelayTime::create(delaytime3 + fadeintime + 3);
	ProgressFromTo* fromto54 = ProgressFromTo::create(3 ,0, 100);
	Sequence* seq54 = Sequence::create(pDelay54,fromto54,nullptr);
	pTimer5->runAction(seq54);
	


}

void	MainScene::HideStoryPage1()
{
	if (pBg_1)
	{
		pBg_1->setVisible(false);
	}

	if (pBg_2)
	{
		pBg_2->setVisible(false);
	}

	if (pBg_3)
	{
		pBg_3->setVisible(false);
	}

	if (pAnimate_1_1)
	{
		pAnimate_1_1->setVisible(false);
	}

	if (pAnimate_1_2)
	{
		pAnimate_1_2->setVisible(false);
	}

	if (pAnimate_2_1)
	{
		pAnimate_2_1->setVisible(false);
	}

	//文字
	if (pTimer)
	{
		pTimer->setVisible(false);
	}
	if (pTimer21)
	{
		pTimer21->setVisible(false);
	}
	if (pTimer22)
	{
		pTimer22->setVisible(false);
	}
	if (pTimer31)
	{
		pTimer31->setVisible(false);
	}
	if (pTimer32)
	{
		pTimer32->setVisible(false);
	}

}

void	MainScene::ShowSkipButton()
{
	// 添加图片动画
	auto desionSize = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();
	pSkipText = Sprite::createWithSpriteFrameName("story_skip01.png");
	pSkipBtn = Sprite::createWithSpriteFrameName("story_skip02.png");

	pSkipText->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	pSkipText->setPosition(desionSize.width -10 - pSkipBtn->getContentSize().width , 20 );
	this->addChild(pSkipText,1);

	pSkipBtn->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	pSkipBtn->setPosition(desionSize.width -10 - pSkipBtn->getContentSize().width , 20 );
	this->addChild(pSkipBtn,2);
	MoveBy* moveby = MoveBy::create(0.3,Vec2(5,0));
	Sequence* seq = Sequence::create(moveby,moveby->reverse(),nullptr);
	RepeatForever* repeat = RepeatForever::create(seq);
	pSkipBtn->runAction(repeat);

	// 创建监听
	auto listener = EventListenerTouchOneByOne::create();//创建一个触摸监听(单点触摸）  
	listener->onTouchBegan = CC_CALLBACK_2(MainScene::onTouchBegan, this);//指定触摸的回调函数  
	listener->onTouchEnded = CC_CALLBACK_2(MainScene::onTouchEnded, this);
	listener->onTouchMoved = CC_CALLBACK_2(MainScene::onTouchMoved, this);  
	listener->onTouchCancelled = CC_CALLBACK_2(MainScene::onTouchCancelled, this);  
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);//将listener放入事件委托中  
}
void	MainScene::SkipStory()
{
	CleanStory();
}

bool	MainScene::onTouchBegan(Touch *touch, Event *unused_event)
{
	return true;
}

void	MainScene::onTouchMoved(Touch *touch, Event *unused_event)
{	
}

void MainScene::onTouchEnded(Touch *touch, Event *unused_event)
{
	Vec2 touchLocation = this->convertTouchToNodeSpace(touch);
	if (pSkipBtn->getBoundingBox().intersectsRect(Rect(touchLocation.x,touchLocation.y,1,1)))
	{
		SkipStory();
	}
}

void MainScene::onTouchCancelled(Touch *touch, Event *unused_event)
{
}

void MainScene::CleanStory()
{
	_eventDispatcher->removeEventListenersForTarget(this);

	Director::getInstance()->getActionManager()->removeAllActions();

	if (pAnimate_1_1)
	{
		pAnimate_1_1->removeFromParent();
	}

	if (pAnimate_1_2)
	{
		pAnimate_1_2->removeFromParent();
	}

	if (pAnimate_2_1)
	{
		pAnimate_2_1->removeFromParent();
	}

	if (pAnimate_4_1)
	{
		pAnimate_4_1->removeFromParent();
	}

	if (pAnimate_5_1)
	{
		pAnimate_5_1->removeFromParent();
	}

	if (pAnimate_5_2)
	{
		pAnimate_5_2->removeFromParent();
	}
	//文字
	if (pTimer)
	{
		pTimer->removeFromParent();
	}
	if (pTimer21)
	{
		pTimer21->removeFromParent();
	}
	if (pTimer22)
	{
		pTimer22->removeFromParent();
	}
	if (pTimer31)
	{
		pTimer31->removeFromParent();
	}
	if (pTimer32)
	{
		pTimer32->removeFromParent();
	}
	if (pTimer4)
	{
		pTimer4->removeFromParent();
	}
	if (pTimer5)
	{
		pTimer5->removeFromParent();
	}

	if (pSkipText)
	{
		pSkipText->removeFromParent();
	}
	
	if (pSkipBtn)
	{
		pSkipBtn->removeFromParent();
	}

	if (pBg_1)
	{
		pBg_1->removeFromParent();
	}

	if (pBg_2)
	{
		pBg_2->removeFromParent();
	}

	if (pBg_3)
	{
		pBg_3->removeFromParent();
	}

	if (pBg_4)
	{
		pBg_4->removeFromParent();
	}

	if (pBg_5)
	{
		pBg_5->removeFromParent();
	}
	
	m_nProc = proc_main;
}

void	MainScene::Tick(float dt)
{
	if (m_nProgress < 100)
	{
		static float delta = 0.0f;
		delta += dt;
		if (delta > m_fProgressDelta)
		{
			if (m_nProgress < m_nTargetProgress)
			{
				m_nProgress++;
			}
			std::stringstream ss;
			ss << m_nProgress << "/";
			if (m_pLoadingLayer)
			{
				m_pLoadingLayer->setProgressInfo(ss.str());
			}
			
			delta = 0.0f;
		}
	}
	
	if (m_nProc == proc_loadui)
	{
		if (m_bLoadOver == false)
		{
			LoadUI();
		}
		return;
	}
	else if (m_nProc == proc_loading)
	{
		return;
	}
	else if (m_nProc == proc_story)
	{
		//if (DBManager().GetData(DATA_SHOW_STORY)) //屏蔽该功能
		if (false) //屏蔽该功能
		{
			SetLoadingLayerVisible(false);
			DBManager().SetData(DATA_SHOW_STORY,0);
			DBManager().SaveDB();
			PlayStory();
			m_nProc = proc_storying;
		}
		else
		{
			m_nProc = proc_main;
		}
		return;
	}
	else if (m_nProc == proc_storying)
	{
		return;
	}
	else if (m_nProc == proc_main)
	{
		SoundSystem().playBackgroundMusic(Mic_bgm_menu);
		EffectManager().PreLoad();
		m_pNiceGirl->Init();
		m_pMenuLayer->Init();
		m_pMenuLayer->setVisible(true);
		m_pRankingLayer->LoadRewardInfo();
		m_pMissionLayer->Init();
		m_pLogin->setVisible(true);
		m_pLogin->Init();
		m_pSettingLayer->Init();

		SetLoadingLayerVisible(false);
		m_nProc = proc_end;

		runAction(Sequence::create(DelayTime::create(0.2f), CallFunc::create([](){
			SDKManager().requestPay(101);
		}), nullptr));

		return;
	}

	//SDKManager().Tick(dt);
	if (m_bCleanGameSence)
	{
		m_bCleanGameSence = false;
		CleanGameScene();
	}
	m_pMenuLayer->Tick(dt);
	if (m_pRankingLayer)
	{
		m_pRankingLayer->Tick(dt);
	}
	if (m_pMissionLayer)
	{
		m_pMissionLayer->Tick(dt);
	}
	ObjectManager().Tick(dt);
	if (m_pGameScene && m_pGameScene->IsPause())
	{
		return;
	}
	EffectManager().Tick(dt);
}

void	MainScene::SetMissonType(bool isbossmisson)
{ 
	m_bIsBossMisson = isbossmisson;
	DBManager().SetBossMission(isbossmisson);
}

void	MainScene::SetMisson(int missionID)
{ 
	m_nMission = missionID;
	DBManager().SetCurrMission(missionID);
}


void	MainScene::TurntoGameSence()
{
	log("TurntoGameSence  ");
	//游戏场景初始化
	m_pMissionLayer->setVisible(false);
	if (!m_pGameScene)
	{
		log("===NEW m_pGameScene ===  ");
		m_pGameScene = GameScene::create();
		ObjectManager().SetGameSceneNode(m_pGameScene);
		this->addChild(m_pGameScene);
		log("===NEW m_pGameScene END ===  ");
	}
	else
	{
		log("===m_pGameScene VISIBLE TRUE===  ");
		m_pGameScene->setVisible(true);
		log("===m_pGameScene VISIBLE TRUE  END ===  ");
	}

	m_bWin = false;
	log("===InitGame ===  ");
	m_pGameScene->InitGame(m_nDefaultPlane,m_bIsBossMisson,m_nMission,m_nDifficulty);
	log("===InitGame END===  ");
}

void MainScene::LeaveGameSence( INTERFACE_ID eType /* = INTERFACE_MAIN */ )
{
	m_bCleanGameSence = true;
	m_GoToInterfaceType = eType;
	SoundSystem().playBackgroundMusic(Mic_bgm_menu);
}

void MainScene::CleanGameScene()
{
	ObjectManager().CleanAllObject();
	SoundSystem().stopAllEffects();
	EffectManager().CleanEffects();

	/*std::stringstream ss;
	ss << m_nMission;
	if(GetMissionState())
	{
	SDKManager().finishLevel(ss.str().c_str());
	}
	else
	{
	SDKManager().failLevel(ss.str().c_str());
	}*/

	m_pGameScene->getParent()->removeChild(m_pGameScene);
	ObjectManager().SetGameSceneNode(nullptr);
	m_pGameScene = nullptr;
	switch (m_GoToInterfaceType)
	{
	case INTERFACE_LOADING:
		break;
	case INTERFACE_MAIN:
		{
			m_pMenuLayer->setVisible(true);
			m_pMenuLayer->calcUIState();			
		}
		break;
	case INTERFACE_INTENSIFY:
		{
			ShowUpgradeLayer();
		}
		break;
	case INTERFACE_MISSION:
		{
			ShowMissonLayer( DBManager().IsBossMission() );
		}
		break;
	default:
		break;
	}

}

void	MainScene::OpenRankingLayer()
{
	if (m_pRankingLayer)
	{
		m_pRankingLayer->SetMyScore( DBManager().GetData(DATA_TOP_SCORE) );
		m_pRankingLayer->Init();
	}
}

void MainScene::CloseRankingLayer()
{
	if (m_pRankingLayer && m_pMenuLayer)
	{
		m_pRankingLayer->setVisible(false);
		m_pMenuLayer->setVisible(true);
	}
}

void MainScene::OpenSettingLayer()
{
	if (m_pSettingLayer)
	{
		m_pSettingLayer->setVisible(true);
	}
}

void MainScene::CloseSettingLayer()
{
	if (m_pSettingLayer)
	{
		m_pSettingLayer->setVisible(false);
		m_pMenuLayer->calcUIState();
	}
}

void MainScene::ShowUpgradeLayer()
{
	//界面
	m_pUpgradeLayer->InitData();
	m_pUpgradeLayer->setVisible(true);
	m_pMenuLayer->setVisible(false);

	runAction(Sequence::create(DelayTime::create(0.2f), CallFunc::create([](){
		SDKManager().requestPay(101);
	}), nullptr));
}

void MainScene::HideUpgradeLayer()
{
	m_pUpgradeLayer->setVisible(false);
	m_pMenuLayer->setVisible(true);	
	m_pMenuLayer->calcUIState();

	runAction(Sequence::create(DelayTime::create(0.2f), CallFunc::create([](){
		SDKManager().requestPay(101);
	}), nullptr));
}

void MainScene::HideUpgradeLayerWithNewPlayer()
{
	m_pUpgradeLayer->setVisible(false);
	m_pMenuLayer->setVisible(true);	
	
	if( DBManager().GetData( DATA_ACTIVATION_TYPE ) > 0 )
	{
		m_pMenuLayer->calcUIState();

	}else
	{
		// 如果没有激活正版，就转到正版激活
		m_pMenuLayer->InitData(1);
	}

}

void MainScene::ShowMissonLayer(bool isBossMisson)
{
	SetMissonType(isBossMisson);
	m_pMissionLayer->InitData( isBossMisson );
	m_pMissionLayer->setVisible(true);
	m_pMenuLayer->setVisible(false);

	runAction(Sequence::create(DelayTime::create(0.2f), CallFunc::create([](){
		SDKManager().requestPay(101);
	}), nullptr));
}

void MainScene::HideMissonLayer()
{
	m_pMissionLayer->setVisible(false);
	m_pMenuLayer->setVisible(true);
	m_pMenuLayer->calcUIState();
	runAction(Sequence::create(DelayTime::create(0.2f), CallFunc::create([](){
		SDKManager().requestPay(101);
	}), nullptr));
}

void MainScene::SetPlaneID(int idx)
{
	m_nDefaultPlane = idx;
	DBManager().SetCurrPlaneID(idx);
}

void MainScene::ShowBuyBoxLayer(BuyType eType, bool isPlayAnima /* = true */ )
{
	if (m_pBuyBoxLayer)
	{
		m_pBuyBoxLayer->InitBuyType(eType,isPlayAnima);
	}
}

void MainScene::ShowOverflowBag(BuyType eType /* = BuyType::Buy_Diamond */,bool isPlayAnima /* = true */ )
{
	if (m_pBuyBoxLayer)
	{
		m_pBuyBoxLayer->InitOverflowBag(eType,isPlayAnima);
	}
}

void MainScene::ShowCounDownFlowBag(BuyType eType /* = BuyType::Buy_Life */,int duration /* = 5 */,bool isPlayAnima /* = true */ )
{
	if (m_pBuyBoxLayer)
	{
		m_pBuyBoxLayer->InitCountDown(eType,duration,isPlayAnima);
	}
}


void MainScene::HideBuyBoxLayer()
{
	if (m_pBuyBoxLayer)
	{
		m_pBuyBoxLayer->setVisible(false);
	}
}

Node* MainScene::GetNodeByZOrder(int zorder)
{
	if (zorder >= 0 && zorder < GAME_ZORDER_NUM)
	{
		return m_LayerList[zorder];
	}

	return nullptr;
}

/*------- 主界面相关 --------------------------------------------------------------*/
void MainScene::ResfreshMenuLayerState()
{
	if (m_pMenuLayer)
	{
		m_pMenuLayer->ResfreshButtonState();
	}
}

void MainScene::ShowMenuLayerAndBetterPlane()
{
	if ( m_pMenuLayer )
	{
		// 展示拥有的最好的飞机
		if ( DBManager().GetData( DATA_HAVE_PLANE4 ) > 0 )
		{
			m_pMenuLayer->InitData( 3 );
			SetPlaneID(4);
		}else if( DBManager().GetData( DATA_HAVE_PLANE3 ) > 0 )
		{
			m_pMenuLayer->InitData( 2 );
			SetPlaneID(2);
		}else if( DBManager().GetData( DATA_ACTIVATION_TYPE ) > 0 )
		{
			m_pMenuLayer->InitData( 1 );
			SetPlaneID(1);
		}else
		{
			m_pMenuLayer->InitData( 0 );
		}
	}
}

/*------- 强化界面相关 --------------------------------------------------------------*/
bool MainScene::IsShowUpgradeLayer()
{
	if (m_pUpgradeLayer)
	{
		return m_pUpgradeLayer->isVisible();
	}
	return false;
}

void MainScene::RefreshUpgradeLayerState()
{
	if (m_pUpgradeLayer)
	{
		m_pUpgradeLayer->InitData();
	}
	
}


/*------- 战斗场景相关 --------------------------------------------------------------*/
void MainScene::GamePause()
{
	if (m_pGameScene)
	{
		m_pGameScene->GamePauseAndNoSuspendLayer();
	}	
}

bool MainScene::IsGamePause()
{
	if (m_pGameScene)
	{
		return m_pGameScene->IsPause();
	}
	return false;
}

void MainScene::GameContinue()
{
	if (m_pGameScene)
	{
		m_pGameScene->GameContinueAndNoSuspendLayer();
	}
}

void MainScene::PlaneRenascence()
{
	if (m_pGameScene)
	{
		m_pGameScene->PlayerPlaneRenascence();
	}
}

void MainScene::ClearGameScenePlayerPlane()
{
	if (m_pGameScene)
	{
		m_pGameScene->ClearPlayerPlane();
	}
}

void MainScene::ShowNiceGirl()
{
	if(m_pNiceGirl)
	{
		m_pNiceGirl->ShowNiceGirl();
	}
}

void MainScene::ShowNewPlayer()
{
	if(m_pNiceGirl)
	{
		m_pNiceGirl->ShowNewPlayerInfo();
	}
}

void MainScene::GoToNextStep()
{
	if(m_pNiceGirl)
	{
		m_pNiceGirl->NewPlayerStep();
	}
}

void MainScene::ShowMissionStartStory(int nIdx)
{
	if(m_pNiceGirl)
	{
		m_pNiceGirl->ShowMissionStartStory(nIdx);
	}
}

void MainScene::ShowMissionEndStory(int nIdx)
{
	if ( m_pNiceGirl )
	{
		m_pNiceGirl->ShowMissionEndStory(nIdx);
	}
}

void MainScene::PayContinue()
{
	m_pBuyBoxLayer->GoToBuy();
}

void  MainScene::PlaneCreatShieldEffect()
{
	if ( m_pGameScene )
	{
		m_pGameScene->PlayerPlaneShieldEffect();
	}
}

void MainScene::ParseThread(MainScene *pScene)
{

	// DB初始化
	DataBaseSystem().Initial(0);
	pScene->SetTargetProgress(10);

	// 加载玩家数据
	DBManager().LoadDB();
	pScene->SetTargetProgress(20);

	// 加载行为
	std::string AIFileName = "eScript";
	LXActionManager().LoadAction(AIFileName);
	pScene->SetTargetProgress(80);

	// 弹幕管理器
	ShooterManager().ParseShootersData();
	pScene->SetTargetProgress(90);
	pScene->SetProgressDelta(0.01);

	// 特效管理器
	EffectManager().LoadAllEffect();
	pScene->SetTargetProgress(95);

	// 加载怪物动画
	AnimationManager().LoadAnimation(dijiFileName);
	pScene->SetTargetProgress(98);

	// 加载任务数据
	TaskMgr().CreatTaskOption();

	pScene->SetStory(proc_story);
	pScene->SetTargetProgress(100);

//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//	//vm->DetachCurrentThread();
//#endif
}

void MainScene::SetLoadingLayerVisible(bool value)
{
	if(m_pLoadingLayer)	
	{
		m_pLoadingLayer->removeFromParentAndCleanup(true);
		m_pLoadingLayer = nullptr;
	}
}

void MainScene::SetTargetProgress(int value)
{
	if(value == 100 && m_pLoadingLayer)
	{
		m_pLoadingLayer->setProgressInfo("100/"); 
	}
		
	m_nTargetProgress = value; 
}

void MainScene::ReLoadRankingInfo()
{
	if ( m_pRankingLayer )
	{
		m_pRankingLayer -> LoadRankingInfo();
	}
}

void MainScene::TaskTipsShow(std::string _str)
{
	if ( m_pTaskTipsLayer )
	{
		m_pTaskTipsLayer->Init(_str);
	}
}


KeyBoardLayer::KeyBoardLayer()
{
}

KeyBoardLayer::~KeyBoardLayer()
{
	_eventDispatcher->removeEventListenersForTarget(this);
}

bool KeyBoardLayer::init()
{
	//添加back按钮响应
	//对手机返回键的监听  
	auto listener = EventListenerKeyboard::create();
	//和回调函数绑定  
	listener->onKeyReleased = CC_CALLBACK_2(KeyBoardLayer::onKeyReleased,this);
	//添加到事件分发器中  
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
	//Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);  
	return true;
}

void KeyBoardLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_BACK){
		//save data
		DBManager().SetData(DATA_FRESH_MAN, 1);
		SDKManager().requestPay(102);
	}
}