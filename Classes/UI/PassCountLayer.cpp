#include "PassCountLayer.h"
#include "Scene/GameScene.h"
#include "Common/FileUtils.h"
#include "GameDefine.h"
#include "Sound/LXSoundSystem.h"
#include "MyAnimation/NiceGirl.h"
#include "DB/DBManager.h"
#include "Scene/MainScene.h"

PassCountLayer::PassCountLayer():Layer()
{
	m_DiamondAtlas = nullptr;
	m_KillAtlas = nullptr;
	m_ScoreAtlas = nullptr;

	m_Diamond = 0;
	m_Kill = 0;
	m_Score = 0;

	m_AllTime = 0.0f;
	m_isShowDoneWithNum = false;

}

PassCountLayer::~PassCountLayer()
{

}

bool PassCountLayer::init()
{
	// 1. super init first
	if ( !Layer::init() )
	{
		return false;
	}

	//
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
	/*---------------------------------------------------------------------*/
	std::string key = "tongguanjiangli";
	LXFileUtils::Get_UI_Json_FullPath(key);
	Layout* pPassCountLayout = static_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile( key.c_str() ));
	if (!pPassCountLayout)
	{
		return false;
	}

	this->addChild(pPassCountLayout);

	// 继续按钮
	Button*  _GoOnButton	= static_cast<Button*>(Helper::seekWidgetByName(pPassCountLayout, "Button_Jixu"));
	_GoOnButton->addTouchEventListener(CC_CALLBACK_2(PassCountLayer::GoOnTouchEvent, this));

	// 强化按钮
	Button*  _IntensifyButton	= static_cast<Button*>(Helper::seekWidgetByName(pPassCountLayout, "Button_qianghua"));
	_IntensifyButton->addTouchEventListener(CC_CALLBACK_2(PassCountLayer::IntensifyTouchEvent, this));

	// 关闭按钮
	Button* _CloseButton = static_cast<Button*>(Helper::seekWidgetByName(pPassCountLayout, "Button_gb"));
	_CloseButton->addTouchEventListener(CC_CALLBACK_2(PassCountLayer::CloseTouchEvent, this));
	
	// 宝石
	m_DiamondAtlas	= static_cast<TextAtlas*>(Helper::seekWidgetByName(pPassCountLayout,"baoshishu"));
	m_DiamondAtlas->setString("0");

	// 杀敌
	m_KillAtlas		= static_cast<TextAtlas*>(Helper::seekWidgetByName(pPassCountLayout,"shadishu"));
	m_KillAtlas->setString("0");

	// 得分
	m_ScoreAtlas	= static_cast<TextAtlas*>(Helper::seekWidgetByName(pPassCountLayout,"defengshu"));
	m_ScoreAtlas->setString("0");

	return true;
}

void PassCountLayer::Tick(float dt)
{
	if ( !m_isShowDoneWithNum )
	{
		return ;
	}

	m_AllTime += dt;

	std::stringstream szTemp;

	if( m_AllTime >= All_Time_Of_Add_Num )
	{
		m_isShowDoneWithNum = true;

		m_DiamondAtlas->setString( StringUtils::toString( m_Diamond ) );

		m_KillAtlas->setString( StringUtils::toString( m_Kill ) );

		m_ScoreAtlas->setString( StringUtils::toString( m_Score ) );
		
	}else
	{
		
		szTemp<< static_cast<int>( m_Diamond * m_AllTime / All_Time_Of_Add_Num ) ;
		m_DiamondAtlas->setString(szTemp.str());

		szTemp.str("");
		szTemp<< static_cast<int>( m_Kill * m_AllTime / All_Time_Of_Add_Num ) ;
		m_KillAtlas->setString(szTemp.str());

		szTemp.str("");
		szTemp<< static_cast<int>( m_Score * m_AllTime / All_Time_Of_Add_Num ) ;
		m_ScoreAtlas->setString(szTemp.str());

	}


}

void PassCountLayer::Init(int Diamond,int KillNum,int Scour)
{
	this->setVisible(true);

	m_Diamond = Diamond;
	m_Kill = KillNum;
	m_Score = Scour;

	m_AllTime = 0.0f;
	m_isShowDoneWithNum = true;


	/*---------------------------------------------------------------------*/
	if ( DBManager().GetData( DATA_FIRST_CHARGE ) < 1 )		// 首充大礼包
	{
		dynamic_cast<MainScene*>( this->getParent()->getParent()->getParent() )->ShowBuyBoxLayer( BuyType::Buy_FirstCharge );
	}else
	{
		// 新手教学相关
		if ( DBManager().GetData( DATA_FRESH_MAN ) < 1 )
		{
			if ( NiceGirl::GetFreshManCurrStep() ==  14 )
			{
				NiceGirl::SetFreshManCurrStep( 15 );

				dynamic_cast<MainScene*>(this->getParent()->getParent()->getParent())->ShowNewPlayer();
			}
		}
	}
}

void PassCountLayer::GoOnTouchEvent(Ref *pSender, Widget::TouchEventType type)
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
			// 这里返回选择界面
			static_cast<GameScene*>(this->getParent()->getParent())->GameGoToSelectMission();
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

void PassCountLayer::IntensifyTouchEvent(Ref* pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		{
			SoundSystem().playEffect(SoundName::Mic_sure);
			// 这里返回选择界面
			static_cast<GameScene*>(this->getParent()->getParent())->GameGoToIntensify();
		}
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void PassCountLayer::CloseTouchEvent(Ref *pSender, Widget::TouchEventType type)
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
			// 这里返回主界面
			static_cast<GameScene*>(this->getParent()->getParent())->GameGiveUp();
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