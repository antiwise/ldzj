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

// �ɻ�����
const std::string planeType[4] = {"airplane", "airplane2", "airplane3", "erhaoji"};


// �ɻ���Ϣ
const std::string planeIconPath[4] = {"zhujiemian_34.png", "zhujiemian_35.png", "zhujiemian_36.png", "zhujiemian_37.png"};
const std::string planeInfoPath[4] = {"zhujiemian_30.png", "zhujiemian_31.png", "zhujiemian_32.png", "zhujiemian_33.png"};

// �ɻ�����
const int planeInfo[4][3] = { {4,8,12},{5,9,13},{6,10,14},{7,11,15} };

// �ɻ���ֵ
const int planeValue[4][2] = { {40,50},{60,50},{80,50},{100,100} };

MenuLayer::MenuLayer():Layer()
{
	m_PageView = nullptr;				// �ɻ�ҳ
	m_ToLeftButton = nullptr;			// ����ť
	m_ToRightButton = nullptr;			// ���Ұ�ť
	m_PlaneIcon = nullptr;				// �ɻ�ͼ��
	m_Attack = nullptr;					// ����
	m_GemChangeRatio = nullptr;			// ��ʯת����
	m_PlaneName = nullptr;				// �ɻ���
	m_AttackBar = nullptr;				// ������
	m_GemChangeBar = nullptr;			// ��ʯת����
	m_PlaneStrengthen = nullptr;		// ս��ǿ��
	m_PlaneBuy = nullptr;				// �ɻ�����
	m_CurrCoin = nullptr;				// ��ǰǮ��
	m_IsStopTick = false;
	m_IsJudgementPlane = true;			// �Ƿ��ж�
	
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

	// �����Ұ�ť
	ImageView* BuyCoinImage = dynamic_cast<ImageView*>(Helper::seekWidgetByName(pMenuLayout, "jinbigoumai"));
	BuyCoinImage->setTouchEnabled(true);
	BuyCoinImage->addTouchEventListener(CC_CALLBACK_2(MenuLayer::BuyCoinTouchEvent, this));

	// ��
	ImageView* LeftImage = dynamic_cast<ImageView*>(Helper::seekWidgetByName(pMenuLayout, "zuoImage"));
	
	m_ToLeftButton = ImageView::create();
	m_ToLeftButton->ignoreContentAdaptWithSize(false);		//  �Ƿ����
	m_ToLeftButton->setContentSize(Size(80,400));
	m_ToLeftButton->setPosition(LeftImage->getPosition());
	m_ToLeftButton->setTouchEnabled(true);
	m_ToLeftButton->addTouchEventListener(CC_CALLBACK_2(MenuLayer::ToLeftTouchEvent, this));
	pMenuLayout->addChild(m_ToLeftButton, 3);

	// ��
	ImageView* RightImage = dynamic_cast<ImageView*>(Helper::seekWidgetByName(pMenuLayout, "youImage"));

	m_ToRightButton = ImageView::create();
	m_ToRightButton->ignoreContentAdaptWithSize(false);
	m_ToRightButton->setContentSize(Size(80,400));
	m_ToRightButton->setPosition(RightImage->getPosition());
	m_ToRightButton->setTouchEnabled(true);
	m_ToRightButton->addTouchEventListener(CC_CALLBACK_2(MenuLayer::ToRightTouchEvent, this));
	pMenuLayout->addChild(m_ToRightButton, 3);

	// ��ҳ�ؼ�
	m_PageView = dynamic_cast<PageView*>(Helper::seekWidgetByName(pMenuLayout, "PageView"));
	m_PageView->addEventListener(CC_CALLBACK_2(MenuLayer::PageViewEvent, this));
	// setSwallowsTouches(false);		// �Ƿ����´��ݴ���

	m_AttackBar					= dynamic_cast<LoadingBar*>(Helper::seekWidgetByName(pMenuLayout, "gongjitiao"));
	m_GemChangeBar				= dynamic_cast<LoadingBar*>(Helper::seekWidgetByName(pMenuLayout, "zhuanhuantiao"));

	// ������ ת����
	m_Attack			= dynamic_cast<Text*>(Helper::seekWidgetByName(pMenuLayout,"gongjili"));
	m_GemChangeRatio	= dynamic_cast<Text*>(Helper::seekWidgetByName(pMenuLayout,"zhuanhuanlv"));

	// ҳ����ʾ
	ImageView*	pageImagIdx1 = dynamic_cast<ImageView*>(Helper::seekWidgetByName(pMenuLayout, "yeshu1"));
	ImageView*	pageImagIdx2 = dynamic_cast<ImageView*>(Helper::seekWidgetByName(pMenuLayout, "yeshu2"));
	ImageView*	pageImagIdx3 = dynamic_cast<ImageView*>(Helper::seekWidgetByName(pMenuLayout, "yeshu3"));
	ImageView*	pageImagIdx4 = dynamic_cast<ImageView*>(Helper::seekWidgetByName(pMenuLayout, "yeshu4"));
	m_vecPageIdx.push_back(pageImagIdx1);
	m_vecPageIdx.push_back(pageImagIdx2);
	m_vecPageIdx.push_back(pageImagIdx3);
	m_vecPageIdx.push_back(pageImagIdx4);

	// �ɻ�ͼ�ꡢ���ơ����ܡ�����˵��
	m_PlaneIcon	= dynamic_cast<ImageView*>(Helper::seekWidgetByName(pMenuLayout, "icon"));
	m_PlaneName = dynamic_cast<ImageView*>(Helper::seekWidgetByName(pMenuLayout,"Image_zhanjiinfo"));

	// ����
	Button* RankingBtn = dynamic_cast<Button*>(Helper::seekWidgetByName(pMenuLayout,"paihangBTN"));
	RankingBtn->addTouchEventListener(CC_CALLBACK_2(MenuLayer::RankingTouchEvent, this));

	// ����
	Button* SettingBtn = dynamic_cast<Button*>(Helper::seekWidgetByName(pMenuLayout,"shezhiBTN"));
	SettingBtn->addTouchEventListener(CC_CALLBACK_2(MenuLayer::SettingTouchEvent, this));

	// Bossģʽ
	m_BossOpenBtn = dynamic_cast<Button*>(Helper::seekWidgetByName(pMenuLayout,"BossBTN"));
	m_BossOpenBtn->addTouchEventListener(CC_CALLBACK_2(MenuLayer::BossTouchEvent, this));

	// �ؿ�ģʽ
	Button* ThemeBtn = dynamic_cast<Button*>(Helper::seekWidgetByName(pMenuLayout,"guanqiaBTN"));
	ThemeBtn->addTouchEventListener(CC_CALLBACK_2(MenuLayer::ThemeTouchEvent, this));

	// ǿ��
	m_PlaneStrengthen = dynamic_cast<Button*>(Helper::seekWidgetByName(pMenuLayout,"qianghuaBTN"));
	m_PlaneStrengthen->addTouchEventListener(CC_CALLBACK_2(MenuLayer::PlaneStrengthenTouchEvent, this));

	// ����ɻ�
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
	// ���طɻ� --------------------------------------------
 	for (int i = 0; i < 5; i++)
 	{
		// ��Ҫ3�Ż���
		if ( i == 3 )
		{
			continue;
		}

 		// �����ؼ�
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
		_Player->setName(StringUtils::toString(i) + "����");

		// ���طɻ�----------------------
		_Player->SetUIObject(true);
 		_Player->Init(i);

 		// ����λ��
 		_Player->setPosition(Vec2(m_PageView->getContentSize().width/2, m_PageView->getContentSize().height/2));
 
 		// ��������
 		// ObjectManager().RigisterPlayer(_Player);
 
 		// �����Ż�----------------------
 		const int nWingNum = 6;
 		for (int j = 0; j < nWingNum; j++)
 		{
			// ����ս�����ͼ����Ż�����
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

				pWing->setName(StringUtils::toString(i) + "����" + StringUtils::toString(j) +"�Ż�");

 				pWing->Init(nResID, bValue);
				pWing->SetUIObject(true);	
 				// λ��
 				float xPos = _Player->getPosition().x + g_WingXPos[j];
 				float yPos = _Player->getPosition().y - g_WingYPos[j];
 				pWing->setPosition(ccp(xPos, yPos));
 				// ����Ż�����
 				_Player->AddWingPlane(pWing);
 			}
 		}

 		// ��¼
 		m_vecPagePlane.push_back(_Player);
 
 	}

	// ��Ļ��ʼ��
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
		// չʾӵ�е���õķɻ�
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
	// ����ǰ��ť״̬
	int totalPages = m_PageView->getPages().size();
	int currentIndex = m_PageView->getCurPageIndex();

	if ( _nPage < 0 || _nPage >= totalPages )
	{
		return ;
	}

	if ( _nPage !=  currentIndex )
	{
		m_IsJudgementPlane = false;
		// �ƶ���ĳһҳ
		m_PageView->scrollToPage( _nPage );
	}else
	{
		m_IsJudgementPlane = true;
	}

	// ��ʼ��ʱ��
	m_AllTime = 0.0f;

	m_nSelectPage = _nPage;

	// �Ƿ���Ҫ����ɻ�
	if( IsNeedBuyItem(_nPage) )
	{
		m_PlaneStrengthen->setVisible(false);
		m_PlaneBuy->setVisible(true);

		// ����Ҫ��ʾ
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
	if ( m_AllTime > 0.8f )		// �߹�0.8���
	{
		int _currPage = m_PageView->getCurPageIndex();					// ��ȡ��ǰ��ҳ��

		int _planeLeve =  m_vecPagePlane[ _currPage ]->GetLevel();		// ��ȡ��ǰ�ɻ��ȼ�

		if ( _planeLeve != 4 )			// �ɻ��ȼ�������ߵȼ�
		{
			m_vecPagePlane[ _currPage ]->SetLevel( ++_planeLeve );						// �ɻ�����

			_planeLeve =  m_vecPagePlane[ _currPage ]->GetLevel();						// ��ȡ��ǰ�ɻ��ȼ�

			if ( ( _planeLeve == 4 ) && (  m_vecPagePlane[ _currPage ]->GetResID() != 3 )  )						// ����֮��ĵȼ�������ߵȼ� ���ҷɻ����Ͳ��� 3 
			{
				std::string path = planeType[_currPage];

				m_vecPagePlane[ _currPage ]->PlayAnimaton(path, g_AnimationName_g13 );								// ���Ŷ���

				m_vecPagePlane[ _currPage ]->SetNextAnimation( g_AnimationName_g8 );								// ������һ������

			}

			m_AllTime = 0.0f;								// ʱ������

			m_vecPagePlane[_currPage]->InitShootLogic();		// ��ʼ����Ļ

		}else if ( m_AllTime > 2.0f )
		{
			//log(" _planeLeve --------------------------- ************ ");
			if ( m_vecPagePlane[ _currPage ]->GetResID() != 3 )				// �ɻ����Ͳ��� 3 
			{
				std::string path = planeType[_currPage];

				m_vecPagePlane[ _currPage ]->PlayAnimaton(path,g_AnimationName_g13,"",animation_type_rrepeat);			// ���Ŷ���

				m_vecPagePlane[ _currPage ]->SetNextAnimation( g_AnimationName_g3 );										// ������һ������

			}

			m_AllTime = 0.0f;

			m_vecPagePlane[ _currPage ]->SetLevel(0);				// ����֮�����õȼ�
			m_vecPagePlane[ _currPage ]->InitShootLogic();			// ��ʼ������

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

	// ����ǰ��ť״̬
	int currentIndex = m_PageView->getCurPageIndex();
	int totalPages = m_PageView->getPages().size();


	bool isNeedOpen = false;

	if( currentIndex == 1 )
	{
		// û�м���2�ŷɻ�
		if ( DBManager().GetData(DATA_ACTIVATION_TYPE) <= 0 )
		{
			isNeedOpen = true;
		}

	}else if( currentIndex == 2 )
	{
		// û�м���3�ŷɻ�
		if ( DBManager().GetData(DATA_HAVE_PLANE3) <= 0 )
		{
			dynamic_cast<MainScene*>(this->getParent())->ShowBuyBoxLayer(BuyType::Buy_Plane3);

			isNeedOpen = true;
		}

	}else if ( currentIndex == 3 )
	{
		// û�м���4�ŷɻ�
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

			m_vecPagePlane[i]->PlayAnimaton(path, g_AnimationName_g3);											// ���Ŷ���

			m_vecPagePlane[i]->InitShootLogic();

			// ��ʾ��Ӧ��ҳ��
			m_vecPageIdx[i]->setVisible(true);

		}else
		{
			m_vecPagePlane[i]->StopShootLogic();

			//  ���ر��ҳ��
			m_vecPageIdx[i]->setVisible(false);
		}
	}
}

void MenuLayer::ShowAttack(int nCurrVal,int nMaxVal)
{
	int nVal = ( nCurrVal * (100/nMaxVal) );
	m_AttackBar->setPercent( nVal );

	// ��ʾ������
	char temp[32] = {0};
	sprintf(temp,"%d",nCurrVal);
	m_Attack->setString(temp);
}


void MenuLayer::ShowGemChanegRatio(int nCurrVal,int nMaxVal)
{
	int nVal = ( nCurrVal * (100/nMaxVal) );
	m_GemChangeBar->setPercent( nVal );

	// ��ʾ��ʯת����
	char temp[32] = {0};
	sprintf(temp,"%d%%",nCurrVal);
	m_GemChangeRatio->setString(temp);
}

void MenuLayer::ShowPlaneInfo(int idx)
{
	// ��ȡ�ɻ�������

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
	//	m_PlaneName->setString( pNmae->getStringValue() );				// �ɻ���
	//}


	std::string _path = planeInfoPath[idx];
	m_PlaneName->loadTexture(_path, Widget::TextureResType::PLIST );				// �ɻ���Ϣ

	_path = planeIconPath[idx];
	m_PlaneIcon->loadTexture(_path,Widget::TextureResType::PLIST);					// ���طɻ�ͼ��

	ShowAttack(planeValue[idx][0],100);												// ������

	ShowGemChanegRatio(planeValue[idx][1],100);										// ��ʯת����
}

void MenuLayer::ShowMyCurrCoin(int nNum)
{
	if (nNum < 0)
	{
		nNum = 0;
	}
	
	// ��ʾ���
	m_CurrCoin->setString(StringUtils::toString(nNum));
}

/*------------------------------------------------------------------------------------------------------------------------------------------*/
// ��ҹ���ص�
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

// ���� �ص�
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


// ���� �ص�
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


// ��ҳ�ص�
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


// ���� �ص�
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
			dynamic_cast<MainScene*>(this->getParent())->OpenRankingLayer();			// ��ʱ����
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


// ���� �ص�
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
			dynamic_cast<MainScene*>(this->getParent())->OpenSettingLayer();			// ��ʱ����
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


// Boss �ص�
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

			// û�п�������������
			if ( DBManager().GetData(DATA_ACTIVATION_TYPE) <= 0 )
			{
				dynamic_cast<MainScene*>(this->getParent())->ShowBuyBoxLayer(BuyType::Buy_Genuine);

				BuyBoxLayer::SetIsShowBettlePlane(false);

				return ;
			}

			int idx = m_PageView->getCurPageIndex();

			// �Ƿ���Ҫ����ɻ�
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
			//��ʾboss�ؿ����Ѵ���
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


// �ؿ� �ص�
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

				// ���ֽ�ѧ
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


// ǿ�� �ص�
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
			//����ǿ������
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

// ����ɻ��ص�
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
	// ����������������
	if (  DBManager().GetData(DATA_ACTIVATION_TYPE) > 0 )
	{
		// �����ʾ����2�ŷɻ�������ť���أ�ǿ����ť��ʾ
		if ( m_nSelectPage == 1 )
		{
			m_PlaneStrengthen->setVisible(true);
			m_PlaneBuy->setVisible(false);
		}
	}

	// ���������3�ŷɻ�
	if ( DBManager().GetData(DATA_HAVE_PLANE3) > 0 )
	{
		// �����ʾ����3�ŷɻ�������ť���أ�ǿ����ť��ʾ
		if ( m_nSelectPage == 2 )
		{
			m_PlaneStrengthen->setVisible(true);
			m_PlaneBuy->setVisible(false);
		}
	}

	// ���������4�ŷɻ�
	if ( DBManager().GetData(DATA_HAVE_PLANE4) > 0 )
	{
		// �����ʾ����4�ŷɻ�������ť���أ�ǿ����ť��ʾ
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
			// û�м���2�ŷɻ�
			if ( DBManager().GetData(DATA_ACTIVATION_TYPE) <= 0 )
			{
				dynamic_cast<MainScene*>(this->getParent())->ShowBuyBoxLayer(BuyType::Buy_Genuine);

				return true;
			}
		}
		break;
	case 2:
		{
			// û�м���3�ŷɻ�
			if ( DBManager().GetData(DATA_HAVE_PLANE3) <= 0 )
			{
				dynamic_cast<MainScene*>(this->getParent())->ShowBuyBoxLayer(BuyType::Buy_Plane3);

				return true;

			}
		}
		break;
	case 3:
		{
			// û�м���4�ŷɻ�
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