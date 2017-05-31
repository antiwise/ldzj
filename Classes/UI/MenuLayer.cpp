#include "MenuLayer.h"
#include "Scene/MainScene.h"
#include "GameDefine.h"
#include "DB/DBManager.h"
#include "ObjectEx/LXObjectManager.h"
#include "ObjectEx/Plane/LXPlayerPlane.h"
#include "ObjectEx/Plane/LXWingPlane.h"
#include "Animation/AnimationLogic.h"
#include "Sound/LXSoundSystem.h"
#include "DataBase/DataBase.h"
#include "Common/FileUtils.h"
#include "Common/LXUtils.h"
#include "MyAnimation/TuHaoJin.h"
#include "MyAnimation/NiceGirl.h"
#include "BuyBoxLayer.h"
#include "RankUp.h"
#include "TaskOption/TaskOptionMgr.h"

// 飞机创建
const std::string planeType[4] = {"airplane", "airplane2", "airplane3", "erhaoji"};


// 飞机信息
const std::string planeIconPath[4] = {"zhujiemian_34.png", "zhujiemian_35.png", "zhujiemian_36.png", "zhujiemian_37.png"};
const std::string planeInfoPath[4] = {"zhujiemian_30.png", "zhujiemian_31.png", "zhujiemian_32.png", "zhujiemian_33.png"};

// 飞机介绍
const int planeInfo[4][3] = { {4,8,12},{5,9,13},{6,10,14},{7,11,15} };

// 飞机数值
const int planeValue[4][2] = { {40,50},{60,50},{80,50},{100,100} };

MenuLayer::MenuLayer():Layer()
{
	m_PageView = nullptr;				// 飞机页
	m_ToLeftButton = nullptr;			// 向左按钮
	m_ToRightButton = nullptr;			// 向右按钮
	m_PlaneIcon = nullptr;				// 飞机图标
	m_Attack = nullptr;					// 攻击
	m_GemChangeRatio = nullptr;			// 宝石转化率
	m_PlaneName = nullptr;				// 飞机名
	m_AttackBar = nullptr;				// 攻击条
	m_GemChangeBar = nullptr;			// 宝石转换条
	m_PlaneStrengthen = nullptr;		// 战机强化
	m_PlaneBuy = nullptr;				// 飞机购买
	m_CurrCoin = nullptr;				// 当前钱币
	m_IsStopTick = false;
	m_IsJudgementPlane = true;			// 是否判断
	
	m_AllTime = 0.0f;
	m_nSelectPage = 0;
	m_TempCoinNum = 0;
	m_TempRewardNum = 0;
}

MenuLayer::~MenuLayer()
{
	for (auto it : m_vecPagePlane)
	{
		it->SetCleanUp(true);
	}

	m_vecPagePlane.clear();

	m_vecPageIdx.clear();
}

bool MenuLayer::init()
{
	// 1. super init first
	if ( !Layer::init() )
	{
		return false;
	}
	
	std::string key = "zhujiemian";
	LXFileUtils::Get_UI_Json_FullPath(key);
	Layout* pMenuLayout = dynamic_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile( key.c_str() ));
	if (!pMenuLayout)
	{
		return false;
	}

	this->addChild(pMenuLayout);
	
	m_CurrCoin	=	dynamic_cast<TextAtlas*>(Helper::seekWidgetByName(pMenuLayout,"jinbi"));
	m_CurrCoin	-> setString("0");

	// 购买金币按钮
	ImageView* BuyCoinImage = dynamic_cast<ImageView*>(Helper::seekWidgetByName(pMenuLayout, "jinbigoumai"));
	BuyCoinImage->setTouchEnabled(true);
	BuyCoinImage->addTouchEventListener(CC_CALLBACK_2(MenuLayer::BuyCoinTouchEvent, this));

	// 左
	ImageView* LeftImage = dynamic_cast<ImageView*>(Helper::seekWidgetByName(pMenuLayout, "zuoImage"));
	
	m_ToLeftButton = ImageView::create();
	m_ToLeftButton->ignoreContentAdaptWithSize(false);		//  是否高亮
	m_ToLeftButton->setContentSize(Size(80,400));
	m_ToLeftButton->setPosition(LeftImage->getPosition());
	m_ToLeftButton->setTouchEnabled(true);
	m_ToLeftButton->addTouchEventListener(CC_CALLBACK_2(MenuLayer::ToLeftTouchEvent, this));
	pMenuLayout->addChild(m_ToLeftButton, 3);

	// 右
	ImageView* RightImage = dynamic_cast<ImageView*>(Helper::seekWidgetByName(pMenuLayout, "youImage"));

	m_ToRightButton = ImageView::create();
	m_ToRightButton->ignoreContentAdaptWithSize(false);
	m_ToRightButton->setContentSize(Size(80,400));
	m_ToRightButton->setPosition(RightImage->getPosition());
	m_ToRightButton->setTouchEnabled(true);
	m_ToRightButton->addTouchEventListener(CC_CALLBACK_2(MenuLayer::ToRightTouchEvent, this));
	pMenuLayout->addChild(m_ToRightButton, 3);

	// 翻页控件
	m_PageView = dynamic_cast<PageView*>(Helper::seekWidgetByName(pMenuLayout, "PageView"));
	m_PageView->addEventListener(CC_CALLBACK_2(MenuLayer::PageViewEvent, this));
	// setSwallowsTouches(false);		// 是否向下传递触摸

	m_AttackBar					= dynamic_cast<LoadingBar*>(Helper::seekWidgetByName(pMenuLayout, "gongjitiao"));
	m_GemChangeBar				= dynamic_cast<LoadingBar*>(Helper::seekWidgetByName(pMenuLayout, "zhuanhuantiao"));

	// 攻击里 转换率
	m_Attack			= dynamic_cast<Text*>(Helper::seekWidgetByName(pMenuLayout,"gongjili"));
	m_GemChangeRatio	= dynamic_cast<Text*>(Helper::seekWidgetByName(pMenuLayout,"zhuanhuanlv"));

	// 页数表示
	ImageView*	pageImagIdx1 = dynamic_cast<ImageView*>(Helper::seekWidgetByName(pMenuLayout, "yeshu1"));
	ImageView*	pageImagIdx2 = dynamic_cast<ImageView*>(Helper::seekWidgetByName(pMenuLayout, "yeshu2"));
	ImageView*	pageImagIdx3 = dynamic_cast<ImageView*>(Helper::seekWidgetByName(pMenuLayout, "yeshu3"));
	ImageView*	pageImagIdx4 = dynamic_cast<ImageView*>(Helper::seekWidgetByName(pMenuLayout, "yeshu4"));
	m_vecPageIdx.push_back(pageImagIdx1);
	m_vecPageIdx.push_back(pageImagIdx2);
	m_vecPageIdx.push_back(pageImagIdx3);
	m_vecPageIdx.push_back(pageImagIdx4);

	// 飞机图标、名称、技能、技能说明
	m_PlaneIcon	= dynamic_cast<ImageView*>(Helper::seekWidgetByName(pMenuLayout, "icon"));
	m_PlaneName = dynamic_cast<ImageView*>(Helper::seekWidgetByName(pMenuLayout,"Image_zhanjiinfo"));

	// 排行
	Button* RankingBtn = dynamic_cast<Button*>(Helper::seekWidgetByName(pMenuLayout,"paihangBTN"));
	RankingBtn->addTouchEventListener(CC_CALLBACK_2(MenuLayer::RankingTouchEvent, this));

	// 设置
	Button* SettingBtn = dynamic_cast<Button*>(Helper::seekWidgetByName(pMenuLayout,"shezhiBTN"));
	SettingBtn->addTouchEventListener(CC_CALLBACK_2(MenuLayer::SettingTouchEvent, this));

	// Boss模式
	m_BossOpenBtn = dynamic_cast<Button*>(Helper::seekWidgetByName(pMenuLayout,"BossBTN"));
	m_BossOpenBtn->addTouchEventListener(CC_CALLBACK_2(MenuLayer::BossTouchEvent, this));

	// 关卡模式
	Button* ThemeBtn = dynamic_cast<Button*>(Helper::seekWidgetByName(pMenuLayout,"guanqiaBTN"));
	ThemeBtn->addTouchEventListener(CC_CALLBACK_2(MenuLayer::ThemeTouchEvent, this));

	// 强化
	m_PlaneStrengthen = dynamic_cast<Button*>(Helper::seekWidgetByName(pMenuLayout,"qianghuaBTN"));
	m_PlaneStrengthen->addTouchEventListener(CC_CALLBACK_2(MenuLayer::PlaneStrengthenTouchEvent, this));

	// 购买飞机
	m_PlaneBuy = dynamic_cast<Button*>(Helper::seekWidgetByName(pMenuLayout,"goumai"));
	m_PlaneBuy->addTouchEventListener(CC_CALLBACK_2(MenuLayer::PlaneBuyEventTouchEvent, this));

	m_RewardTips = dynamic_cast<ImageView*>(Helper::seekWidgetByName(pMenuLayout, "Image_RenWuTiShi"));
	m_RewardTips->setTouchEnabled(true);
	m_RewardTips->addTouchEventListener(CC_CALLBACK_2(MenuLayer::RankingTouchEvent, this));
	m_RewardTips->setVisible(false);

	log("---------------------------------------load tuhaojinAnim.ExportJson over");

	TuHaoJin* _tuhaojin = TuHaoJin::create();
	if (_tuhaojin)
	{
		_tuhaojin->setPosition( Utils().GetFixedPosion( 380, 305 ) );
		this->addChild( _tuhaojin );
	}

	log("---------------------------------------MenuLayer::init() over");
	
	return true;
}

void MenuLayer::Init()
{
	// 加载飞机 --------------------------------------------
 	for (int i = 0; i < 5; i++)
 	{
		// 不要3号机了
		if ( i == 3 )
		{
			continue;
		}

 		// 创建控件
 		Layout* layout = Layout::create();
 		if (!layout)
 		{
 			return ;
 		}

		layout->setClippingEnabled(true);
		layout->setClippingType(Layout::ClippingType::STENCIL);
		layout->setContentSize( m_PageView->getContentSize() ); 
		m_PageView->addPage(layout);

		layout->setName(StringUtils::toString(i) + "layout");

 		LXPlayerPlane* _Player = dynamic_cast<LXPlayerPlane*>(ObjectManager().CreateObject(i, layout, object_type_plane_player, true)); 		
 		if ( !_Player )
 		{
 			return ;
 		}
		_Player->setName(StringUtils::toString(i) + "主机");

		// 加载飞机----------------------
		_Player->SetUIObject(true);
 		_Player->Init(i);

 		// 设置位置
 		_Player->setPosition(Vec2(m_PageView->getContentSize().width/2, m_PageView->getContentSize().height/2));
 
 		// 增加引用
 		// ObjectManager().RigisterPlayer(_Player);
 
 		// 加载僚机----------------------
 		const int nWingNum = 6;
 		for (int j = 0; j < nWingNum; j++)
 		{
			// 根据战机类型计算僚机类型
 			int nResID = 0;
 			if(_Player->GetResID() == player_plane_type_zuheji)
 			{
 				nResID = j / 2;
 			}
 			else
 			{
 				nResID = _Player->GetResID();
 			}
 
 			LXWingPlane *pWing = dynamic_cast<LXWingPlane*>(ObjectManager().CreateObject(nResID, layout, object_type_plane_wing, true));
 
 			if (pWing)
 			{
 				bool bValue = false;
 				if ((nResID == 0 || nResID == 4) && (j % 2 == 0))
 				{
 					bValue = true;
 				}

				pWing->setName(StringUtils::toString(i) + "主机" + StringUtils::toString(j) +"僚机");

 				pWing->Init(nResID, bValue);
				pWing->SetUIObject(true);	
 				// 位置
 				float xPos = _Player->getPosition().x + g_WingXPos[j];
 				float yPos = _Player->getPosition().y - g_WingYPos[j];
 				pWing->setPosition(ccp(xPos, yPos));
 				// 添加僚机引用
 				_Player->AddWingPlane(pWing);
 			}
 		}

 		// 记录
 		m_vecPagePlane.push_back(_Player);
 
 	}

	// 弹幕初始化
	for(int i = 0; i < 25; i++)
	{
		std::string weaponInfo = g_PrePlayerWeapon[i];
		std::vector<std::string> vec;
		Utils().split(weaponInfo, vec, ",");

		if (vec.size() == 2)
		{
			ObjectManager().PreCreateObject(std::atoi(vec[0].c_str()), this, object_type_weapon_common, false, std::atoi(vec[1].c_str()));
		}
	}

	//InitData();

	if ( DBManager().GetData( DATA_FRESH_STEP ) >= 15 )
	{
		// 展示拥有的最好的飞机
		if ( DBManager().GetData( DATA_HAVE_PLANE4 ) > 0 )
		{
			InitData( 3 );
			dynamic_cast<MainScene*>( this->getParent() )->SetPlaneID(4);
		}else if( DBManager().GetData( DATA_HAVE_PLANE3 ) > 0 )
		{
			InitData( 2 );
			dynamic_cast<MainScene*>( this->getParent() )->SetPlaneID(2);
		}else if( DBManager().GetData( DATA_ACTIVATION_TYPE ) > 0 )
		{
			InitData( 1 );
			dynamic_cast<MainScene*>( this->getParent() )->SetPlaneID(1);
		}else
		{
			InitData( 0 );
		}

	}else
	{
		InitData(0);
	}


}

void MenuLayer::InitData(int _nPage /* = 0 */)
{
	// 计算前后按钮状态
	int totalPages = m_PageView->getPages().size();
	int currentIndex = m_PageView->getCurPageIndex();

	if ( _nPage < 0 || _nPage >= totalPages )
	{
		return ;
	}

	if ( _nPage !=  currentIndex )
	{
		m_IsJudgementPlane = false;
		// 移动到某一页
		m_PageView->scrollToPage( _nPage );
	}else
	{
		m_IsJudgementPlane = true;
	}

	// 初始化时间
	m_AllTime = 0.0f;

	m_nSelectPage = _nPage;

	// 是否需要购买飞机
	if( IsNeedBuyItem(_nPage) )
	{
		m_PlaneStrengthen->setVisible(false);
		m_PlaneBuy->setVisible(true);

		// 不需要显示
		dynamic_cast<MainScene*>( this->getParent() )->HideBuyBoxLayer();

	}else
	{
		m_PlaneStrengthen->setVisible(true);
		m_PlaneBuy->setVisible(false);
	}


	ShowCurrPageIdxImage(_nPage);

	ShowPlaneInfo(_nPage);

	m_nSelectPage = _nPage;

	m_IsStopTick = false;
}


void MenuLayer::StopAllPlaneShoot()
{
	m_IsStopTick = true;

	for (int i = 0; i < m_vecPagePlane.size(); i++)
	{
		m_vecPagePlane[i]->StopShootLogic();
	}
}


void MenuLayer::Tick(float dt)
{
	if (m_IsStopTick)
	{
		return ;
	}

	//*
 	m_AllTime += dt;
	if ( m_AllTime > 0.8f )		// 走过0.8秒后
	{
		int _currPage = m_PageView->getCurPageIndex();					// 获取当前的页码

		int _planeLeve =  m_vecPagePlane[ _currPage ]->GetLevel();		// 获取当前飞机等级

		if ( _planeLeve != 4 )			// 飞机等级不是最高等级
		{
			m_vecPagePlane[ _currPage ]->SetLevel( ++_planeLeve );						// 飞机升级

			_planeLeve =  m_vecPagePlane[ _currPage ]->GetLevel();						// 获取当前飞机等级

			if ( ( _planeLeve == 4 ) && (  m_vecPagePlane[ _currPage ]->GetResID() != 3 )  )						// 升级之后的等级不是最高等级 并且飞机类型不是 3 
			{
				std::string path = planeType[_currPage];

				m_vecPagePlane[ _currPage ]->PlayAnimaton(path, g_AnimationName_g13 );								// 播放动画

				m_vecPagePlane[ _currPage ]->SetNextAnimation( g_AnimationName_g8 );								// 设置下一个动画

			}

			m_AllTime = 0.0f;								// 时间清理

			m_vecPagePlane[_currPage]->InitShootLogic();		// 初始化弹幕

		}else if ( m_AllTime > 2.0f )
		{
			//log(" _planeLeve --------------------------- ************ ");
			if ( m_vecPagePlane[ _currPage ]->GetResID() != 3 )				// 飞机类型不是 3 
			{
				std::string path = planeType[_currPage];

				m_vecPagePlane[ _currPage ]->PlayAnimaton(path,g_AnimationName_g13,"",animation_type_rrepeat);			// 播放动画

				m_vecPagePlane[ _currPage ]->SetNextAnimation( g_AnimationName_g3 );										// 设置下一个动画

			}

			m_AllTime = 0.0f;

			m_vecPagePlane[ _currPage ]->SetLevel(0);				// 满级之后重置等级
			m_vecPagePlane[ _currPage ]->InitShootLogic();			// 初始化数据

			return ;
		}
	}

	/*---------------------------------------------------------------------*/
	if ( m_TempCoinNum !=  DBManager().GetData(DATA_DIAMOND_NUM))
	{
		m_TempCoinNum = DBManager().GetData(DATA_DIAMOND_NUM);
		ShowMyCurrCoin(m_TempCoinNum);
	}

	if ( m_TempRewardNum != TaskMgr().GetRewardNum() )
	{
		m_TempRewardNum = TaskMgr().GetRewardNum();
		if ( m_TempRewardNum > 0  )
		{
			m_RewardTips->setVisible(true);
		}else
		{
			m_RewardTips->setVisible(false);
		}
	}

}

void MenuLayer::calcUIState()
{
	if( m_IsJudgementPlane == false )
	{
		return ;
	}

	// 计算前后按钮状态
	int currentIndex = m_PageView->getCurPageIndex();
	int totalPages = m_PageView->getPages().size();


	bool isNeedOpen = false;

	if( currentIndex == 1 )
	{
		// 没有激活2号飞机
		if ( DBManager().GetData(DATA_ACTIVATION_TYPE) <= 0 )
		{
			isNeedOpen = true;
		}

	}else if( currentIndex == 2 )
	{
		// 没有激活3号飞机
		if ( DBManager().GetData(DATA_HAVE_PLANE3) <= 0 )
		{
			dynamic_cast<MainScene*>(this->getParent())->ShowBuyBoxLayer(BuyType::Buy_Plane3);

			isNeedOpen = true;
		}

	}else if ( currentIndex == 3 )
	{
		// 没有激活4号飞机
		if ( DBManager().GetData(DATA_HAVE_PLANE4) <= 0 )
		{
			dynamic_cast<MainScene*>(this->getParent())->ShowBuyBoxLayer(BuyType::Buy_Plane4);

			isNeedOpen = true;
		}
	}

	if( isNeedOpen )
	{
		m_PlaneStrengthen->setVisible(false);
		m_PlaneBuy->setVisible(true);

	}else
	{
		m_PlaneStrengthen->setVisible(true);
		m_PlaneBuy->setVisible(false);
	}


	m_nSelectPage = m_PageView->getCurPageIndex();

	ShowCurrPageIdxImage( m_nSelectPage );

	ShowPlaneInfo(m_nSelectPage);

	m_IsStopTick = false;

}

void MenuLayer::ShowCurrPageIdxImage(const int idx)
{
	const int totalPages = m_PageView->getPages().size();

	for (int i = 0; i < totalPages; i++)
	{
		if ( idx == i)
		{

			m_AllTime = 0.0f;
			m_vecPagePlane[i]->SetLevel(0);

			std::string path = planeType[i];

			m_vecPagePlane[i]->PlayAnimaton(path, g_AnimationName_g3);											// 播放动画

			m_vecPagePlane[i]->InitShootLogic();

			// 显示相应的页标
			m_vecPageIdx[i]->setVisible(true);

		}else
		{
			m_vecPagePlane[i]->StopShootLogic();

			//  隐藏别的页标
			m_vecPageIdx[i]->setVisible(false);
		}
	}
}

void MenuLayer::ShowAttack(int nCurrVal,int nMaxVal)
{
	int nVal = ( nCurrVal * (100/nMaxVal) );
	m_AttackBar->setPercent( nVal );

	// 显示攻击力
	char temp[32] = {0};
	sprintf(temp,"%d",nCurrVal);
	m_Attack->setString(temp);
}


void MenuLayer::ShowGemChanegRatio(int nCurrVal,int nMaxVal)
{
	int nVal = ( nCurrVal * (100/nMaxVal) );
	m_GemChangeBar->setPercent( nVal );

	// 显示宝石转化率
	char temp[32] = {0};
	sprintf(temp,"%d%%",nCurrVal);
	m_GemChangeRatio->setString(temp);
}

void MenuLayer::ShowPlaneInfo(int idx)
{
	// 获取飞机攻击力

	if ( idx < 0 || idx >= 4)
	{
		return ;
	}

	//DBC_DEFINEHANDLE(s_pUserDBC, DBC_PARAM_INFO);
	//const _DBC_PARAM_INFO *pNmae = (const _DBC_PARAM_INFO*)s_pUserDBC->Search_Index_EQU( planeInfo[idx][0] );
	//const _DBC_PARAM_INFO *pSkill = (const _DBC_PARAM_INFO*)s_pUserDBC->Search_Index_EQU( planeInfo[idx][1] );
	//const _DBC_PARAM_INFO *pInfo = (const _DBC_PARAM_INFO*)s_pUserDBC->Search_Index_EQU( planeInfo[idx][2] );
	//if ( pNmae && pSkill && pInfo )
	//{
	//	m_PlaneName->setString( pNmae->getStringValue() );				// 飞机名
	//}


	std::string _path = planeInfoPath[idx];
	m_PlaneName->loadTexture(_path, Widget::TextureResType::PLIST );				// 飞机信息

	_path = planeIconPath[idx];
	m_PlaneIcon->loadTexture(_path,Widget::TextureResType::PLIST);					// 加载飞机图标

	ShowAttack(planeValue[idx][0],100);												// 攻击力

	ShowGemChanegRatio(planeValue[idx][1],100);										// 宝石转换率
}

void MenuLayer::ShowMyCurrCoin(int nNum)
{
	if (nNum < 0)
	{
		nNum = 0;
	}
	
	// 显示金币
	m_CurrCoin->setString(StringUtils::toString(nNum));
}

/*------------------------------------------------------------------------------------------------------------------------------------------*/
// 金币购买回调
void	MenuLayer::BuyCoinTouchEvent(Ref* pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		{

		}
		break;
	case Widget::TouchEventType::MOVED:
		{

		}
		break;
	case Widget::TouchEventType::ENDED:
		{
			SoundSystem().playEffect(SoundName::Mic_sure);
			dynamic_cast<MainScene*>( this->getParent() )->ShowBuyBoxLayer( BuyType::Buy_Diamond );
		}
		break;
	case Widget::TouchEventType::CANCELED:
		{

		}
		break;
	default:
		break;
	}
}

// 向左 回调
void	MenuLayer::ToLeftTouchEvent(Ref* pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		{

		}
		break;
	case Widget::TouchEventType::MOVED:
		{

		}
		break;
	case Widget::TouchEventType::ENDED:
		{
			SoundSystem().playEffect(SoundName::Mic_sure);
			int totalPages = m_PageView->getPages().size();
			int currentPage = m_PageView->getCurPageIndex();

			if (currentPage > 0)
			{
				m_PageView->scrollToPage(currentPage - 1);
			}
		}
		break;
	case Widget::TouchEventType::CANCELED:
		{

		}
		break;
	default:
		break;
	}
}


// 向右 回调
void	MenuLayer::ToRightTouchEvent(Ref* pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		{

		}
		break;
	case Widget::TouchEventType::MOVED:
		{

		}
		break;
	case Widget::TouchEventType::ENDED:
		{
			SoundSystem().playEffect(SoundName::Mic_sure);
			int totalPages = m_PageView->getPages().size();
			int currentPage = m_PageView->getCurPageIndex();

			if ( currentPage < totalPages - 1 )
			{
				m_PageView->scrollToPage( currentPage + 1 );
			}

		}
		break;
	case Widget::TouchEventType::CANCELED:
		{

		}
		break;
	default:
		break;
	}
}


// 翻页回调
void	MenuLayer::PageViewEvent(Ref* pSender,	PageView::EventType type)
{
	switch (type)
	{
	case PageView::EventType::TURNING:
		{
			//SoundSystem().playEffect(SoundName::Mic_sure);
			calcUIState();
			m_IsJudgementPlane = true;
		}
		break;
	default:
		break;
	}
}


// 排行 回调
void	MenuLayer::RankingTouchEvent(Ref* pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		{

		}
		break;
	case Widget::TouchEventType::MOVED:
		{

		}
		break;
	case Widget::TouchEventType::ENDED:
		{
			SoundSystem().playEffect(SoundName::Mic_sure);
			dynamic_cast<MainScene*>(this->getParent())->OpenRankingLayer();			// 暂时屏蔽
			//StopAllPlaneShoot();
		}
		break;
	case Widget::TouchEventType::CANCELED:
		{

		}
		break;
	default:
		break;
	}
}


// 设置 回调
void	MenuLayer::SettingTouchEvent(Ref* pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		{

		}
		break;
	case Widget::TouchEventType::MOVED:
		{

		}
		break;
	case Widget::TouchEventType::ENDED:
		{
			SoundSystem().playEffect(SoundName::Mic_sure);
			dynamic_cast<MainScene*>(this->getParent())->OpenSettingLayer();			// 暂时屏蔽
			StopAllPlaneShoot();
		}
		break;
	case Widget::TouchEventType::CANCELED:
		{

		}
		break;
	default:
		break;
	}
}


// Boss 回调
void	MenuLayer::BossTouchEvent(Ref* pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		{

		}
		break;
	case Widget::TouchEventType::MOVED:
		{

		}
		break;
	case Widget::TouchEventType::ENDED:
		{

			SoundSystem().playEffect(SoundName::Mic_sure);

			// 没有开启，购买正版
			if ( DBManager().GetData(DATA_ACTIVATION_TYPE) <= 0 )
			{
				dynamic_cast<MainScene*>(this->getParent())->ShowBuyBoxLayer(BuyType::Buy_Genuine);

				BuyBoxLayer::SetIsShowBettlePlane(false);

				return ;
			}

			int idx = m_PageView->getCurPageIndex();

			// 是否需要购买飞机
			if ( IsNeedBuyItem(idx) )
			{
				return ;
			}

			if ( !m_vecPagePlane.empty() && m_vecPagePlane[idx] )
			{
				SoundSystem().stopAllEffects();
				StopAllPlaneShoot();
				dynamic_cast<MainScene*>(this->getParent())->SetPlaneID( m_vecPagePlane[idx]->GetResID() );
				dynamic_cast<MainScene*>(this->getParent())->ShowMissonLayer(true);
			}
		}
		break;
	case Widget::TouchEventType::CANCELED:
		{

		}
		break;
	default:
		break;
	}
}

void	MenuLayer::BossLockTouchEvent(Ref* pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		{

		}
		break;
	case Widget::TouchEventType::MOVED:
		{

		}
		break;
	case Widget::TouchEventType::ENDED:
		{
			SoundSystem().playEffect(SoundName::Mic_sure);
			//显示boss关卡付费窗口
			log("bosslock");
			dynamic_cast<MainScene*>(this->getParent())->ShowBuyBoxLayer(BuyType::Buy_Genuine);

		}
		break;
	case Widget::TouchEventType::CANCELED:
		{

		}
		break;
	default:
		break;
	}
}


// 关卡 回调
void	MenuLayer::ThemeTouchEvent(Ref* pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		{

		}
		break;
	case Widget::TouchEventType::MOVED:
		{

		}
		break;
	case Widget::TouchEventType::ENDED:
		{
			SoundSystem().playEffect(SoundName::Mic_sure);

			int idx = m_PageView->getCurPageIndex();
			
			if ( IsNeedBuyItem(idx) )
			{
				return ;
			}
			
			
			if ( !m_vecPagePlane.empty() && m_vecPagePlane[idx] )
			{
				SoundSystem().stopAllEffects();
				StopAllPlaneShoot();
				dynamic_cast<MainScene*>(this->getParent())->SetPlaneID( m_vecPagePlane[idx]->GetResID() );
				dynamic_cast<MainScene*>(this->getParent())->ShowMissonLayer(false);

				// 新手教学
				if ( DBManager().GetData( DATA_FRESH_MAN ) < 1 )
				{
					if ( NiceGirl::GetFreshManCurrStep() == 1 )
					{
						NiceGirl::SetFreshManCurrStep( 2 );

						dynamic_cast<MainScene*>(this->getParent())->ShowNewPlayer();

					}
				}
			}
		}
		break;
	case Widget::TouchEventType::CANCELED:
		{

		}
		break;
	default:
		break;
	}
}


// 强化 回调
void	MenuLayer::PlaneStrengthenTouchEvent(Ref* pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		{

		}
		break;
	case Widget::TouchEventType::MOVED:
		{

		}
		break;
	case Widget::TouchEventType::ENDED:
		{
			SoundSystem().playEffect(SoundName::Mic_sure);
			//调用强化界面
			dynamic_cast<MainScene*>(this->getParent())->ShowUpgradeLayer();
			StopAllPlaneShoot();
		}
		break;
	case Widget::TouchEventType::CANCELED:
		{

		}
		break;
	default:
		break;
	}
}

// 购买飞机回调
void	MenuLayer::PlaneBuyEventTouchEvent(Ref* pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		{

		}
		break;
	case Widget::TouchEventType::MOVED:
		{

		}
		break;
	case Widget::TouchEventType::ENDED:
		{
			SoundSystem().playEffect(SoundName::Mic_sure);

			IsNeedBuyItem( m_nSelectPage );
			if ( m_nSelectPage == 1 )
			{
				BuyBoxLayer::SetIsShowBettlePlane(false);
			}

		}
		break;
	case Widget::TouchEventType::CANCELED:
		{

		}
		break;
	default:
		break;
	}
}

void MenuLayer::ResfreshButtonState()
{
	// 如果购买了正版礼包
	if (  DBManager().GetData(DATA_ACTIVATION_TYPE) > 0 )
	{
		// 如果显示的是2号飞机，购买按钮隐藏，强化按钮显示
		if ( m_nSelectPage == 1 )
		{
			m_PlaneStrengthen->setVisible(true);
			m_PlaneBuy->setVisible(false);
		}
	}

	// 如果购买了3号飞机
	if ( DBManager().GetData(DATA_HAVE_PLANE3) > 0 )
	{
		// 如果显示的是3号飞机，购买按钮隐藏，强化按钮显示
		if ( m_nSelectPage == 2 )
		{
			m_PlaneStrengthen->setVisible(true);
			m_PlaneBuy->setVisible(false);
		}
	}

	// 如果购买了4号飞机
	if ( DBManager().GetData(DATA_HAVE_PLANE4) > 0 )
	{
		// 如果显示的是4号飞机，购买按钮隐藏，强化按钮显示
		if ( m_nSelectPage == 3 )
		{
			m_PlaneStrengthen->setVisible(true);
			m_PlaneBuy->setVisible(false);
		}
	}

}

bool MenuLayer::IsNeedBuyItem(int _Idx)
{
	switch (_Idx)
	{
	case 0:
		break;
	case 1:
		{
			// 没有激活2号飞机
			if ( DBManager().GetData(DATA_ACTIVATION_TYPE) <= 0 )
			{
				dynamic_cast<MainScene*>(this->getParent())->ShowBuyBoxLayer(BuyType::Buy_Genuine);

				return true;
			}
		}
		break;
	case 2:
		{
			// 没有激活3号飞机
			if ( DBManager().GetData(DATA_HAVE_PLANE3) <= 0 )
			{
				dynamic_cast<MainScene*>(this->getParent())->ShowBuyBoxLayer(BuyType::Buy_Plane3);

				return true;

			}
		}
		break;
	case 3:
		{
			// 没有激活4号飞机
			if ( DBManager().GetData(DATA_HAVE_PLANE4) <= 0 )
			{
				dynamic_cast<MainScene*>(this->getParent())->ShowBuyBoxLayer(BuyType::Buy_Plane4);

				return true;
			}
		}
		break;
	default:
		break;
	}

	return false;
}