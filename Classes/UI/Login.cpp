#include "Login.h"
#include "Common/FileUtils.h"
#include "Time/TimeSystem.h"
#include "DB/DBManager.h"
#include "Scene/MainScene.h"
#include "SDK/SDKManager.h"
#include "UI/MyAnimation/NiceGirl.h"
#include "Sound/LXSoundSystem.h"

Login::Login(void):Layer()
{
	m_ImageSureBack = nullptr;
	m_CurrIndex = 0;
	m_RandNum = 0;
}


Login::~Login(void)
{
}

bool Login::init()
{
	// 1. super init first
	if ( !Layer::init() )
	{
		return false;
	}

	// 暂停界面
	std::string key = "DengLu";
	LXFileUtils::Get_UI_Json_FullPath(key);
	Layout* _DengLuLayout = static_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile(key.c_str()));
	if (!_DengLuLayout)
	{
		return false;
	}

	this->addChild(_DengLuLayout);

	ImageView* _BackImage1 = static_cast<ImageView*>( Helper::seekWidgetByName( _DengLuLayout,"Image_back1" ) );
	ImageView* _BackImage2 = static_cast<ImageView*>( Helper::seekWidgetByName( _BackImage1,"Image_back2" ) );

	// 领取按钮
	Button* _ReceiveBtn = static_cast<Button*>(Helper::seekWidgetByName(_BackImage2,"Button_lingqu"));
	_ReceiveBtn->addTouchEventListener(CC_CALLBACK_2(Login::ReceiveTouchEvent,this));

	int length = 7;
	for (int i = 1; i <= length; i++)
	{
		std::stringstream _path;
		_path << "Image_info" << i;

		ImageView* _BackImage = static_cast<ImageView*>( Helper::seekWidgetByName( _BackImage2, _path.str() ) );
		m_vecRewardBack.push_back(_BackImage);

		ImageView* _ImageNum = static_cast<ImageView*>( Helper::seekWidgetByName( _BackImage, "Image_Num" ) );
		m_vecRewardNum.push_back(_ImageNum);


		ImageView* _ImageReward = static_cast<ImageView*>( Helper::seekWidgetByName( _BackImage, "Image_yilingqu" ) );		
		m_vecRewardEnd.push_back(_ImageReward);

	}

	// 领奖背景
	m_ImageSureBack = static_cast<ImageView*>( Helper::seekWidgetByName( _DengLuLayout,"Image_Sureback" ) );

	// 领取按钮
	Button* _SureBtn = static_cast<Button*>(Helper::seekWidgetByName( m_ImageSureBack, "Button_queding" ) );
	_SureBtn->addTouchEventListener( CC_CALLBACK_2(Login::SureTouchEvent,this) );

	// 奖品图标、奖品数量
	ImageView* _PrizeImage1 = static_cast<ImageView*>( Helper::seekWidgetByName( m_ImageSureBack, "Image_Icon1" ) );
	ImageView* _PrizeNumImage1 = static_cast<ImageView*>( Helper::seekWidgetByName( m_ImageSureBack, "Image_Num1" ) );
	_PrizeImage1->setVisible(false);
	_PrizeNumImage1->setVisible(false);
	m_vecPrizeImage.push_back(_PrizeImage1);
	m_vecPrizeNum.push_back(_PrizeNumImage1);

	// 奖品图标、奖品数量
	ImageView* _PrizeImage2 = static_cast<ImageView*>( Helper::seekWidgetByName( m_ImageSureBack, "Image_Icon2" ) );
	ImageView* _PrizeNumImage2 = static_cast<ImageView*>( Helper::seekWidgetByName( m_ImageSureBack, "Image_Num2" ) );
	_PrizeImage2->setVisible(false);
	_PrizeNumImage2->setVisible(false);
	m_vecPrizeImage.push_back(_PrizeImage2);
	m_vecPrizeNum.push_back(_PrizeNumImage2);

	// 奖品图标、奖品数量
	ImageView* _PrizeImage3 = static_cast<ImageView*>( Helper::seekWidgetByName( m_ImageSureBack, "Image_Icon3" ) );
	ImageView* _PrizeNumImage3 = static_cast<ImageView*>( Helper::seekWidgetByName( m_ImageSureBack, "Image_Num3" ) );
	_PrizeImage3->setVisible(false);
	_PrizeNumImage3->setVisible(false);
	m_vecPrizeImage.push_back(_PrizeImage3);
	m_vecPrizeNum.push_back(_PrizeNumImage3);

	m_ImageSureBack->setVisible(false);

	OpenListener();

	return true;
}

void Login::Init()
{
	int nLastTime = DBManager().GetData( DATA_LASTTIME_RECEIVE );
	m_CurrIndex = DBManager().GetData( DATA_ALLNUM_RECEIVE );
	int nTime = TimeSystem().GetDateTime();

	if ( nTime == nLastTime )
	{
		// 新手教学判定
		if ( DBManager().GetData( DATA_FRESH_MAN ) < 1 )
		{
			if ( NiceGirl::GetFreshManCurrStep() < 2 )
			{
				dynamic_cast<MainScene*>(this->getParent())->ShowNewPlayer();
			}
		}

		CloseListener();
		this->setVisible(false);
		return ;
	}

	if ( nTime == nLastTime + 1 )
	{
		++m_CurrIndex;

	}else
	{
		m_CurrIndex = 1;
	}

	// 背景变色
	m_vecRewardBack[ m_CurrIndex -1 ]->setOpacity(255);
	m_vecRewardBack[ m_CurrIndex -1 ] ->loadTexture("denglu_03.png",Widget::TextureResType::PLIST);

}

void Login::ReceiveTouchEvent(Ref* pSender, Widget::TouchEventType type)
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
			m_ImageSureBack->setVisible(true);

			switch (m_CurrIndex)
			{
			case 1:
				{
					m_vecPrizeImage[2]->setVisible(true);
					m_vecPrizeNum[2]->setVisible(true);

					m_vecPrizeImage[2]->loadTexture("ranking_03.png",Widget::TextureResType::PLIST);
					m_vecPrizeNum[2]->loadTexture("denglu_026.png",Widget::TextureResType::PLIST);

				}
				break;
			case 2:
				{
					m_vecPrizeImage[0]->setVisible(true);
					m_vecPrizeNum[0]->setVisible(true);

					m_vecPrizeImage[1]->setVisible(true);
					m_vecPrizeNum[1]->setVisible(true);

					m_vecPrizeImage[0]->loadTexture("ranking_20.png",Widget::TextureResType::PLIST);
					m_vecPrizeNum[0]->loadTexture("denglu_020.png",Widget::TextureResType::PLIST);

					m_vecPrizeImage[1]->loadTexture("ranking_19.png",Widget::TextureResType::PLIST);
					m_vecPrizeNum[1]->loadTexture("denglu_020.png",Widget::TextureResType::PLIST);
				}
				break;
			case 3:
				{
					m_vecPrizeImage[2]->setVisible(true);
					m_vecPrizeNum[2]->setVisible(true);

					m_vecPrizeImage[2]->loadTexture("ranking_03.png",Widget::TextureResType::PLIST);
					m_vecPrizeNum[2]->loadTexture("denglu_023.png",Widget::TextureResType::PLIST);
				}
				break;
			case 4:
				{
					m_vecPrizeImage[0]->setVisible(true);
					m_vecPrizeNum[0]->setVisible(true);

					m_vecPrizeImage[1]->setVisible(true);
					m_vecPrizeNum[1]->setVisible(true);

					m_vecPrizeImage[0]->loadTexture("ranking_20.png",Widget::TextureResType::PLIST);
					m_vecPrizeNum[0]->loadTexture("denglu_021.png",Widget::TextureResType::PLIST);

					m_vecPrizeImage[1]->loadTexture("ranking_19.png",Widget::TextureResType::PLIST);
					m_vecPrizeNum[1]->loadTexture("denglu_021.png",Widget::TextureResType::PLIST);
				}
				break;
			case 5:
				{
					m_vecPrizeImage[2]->setVisible(true);
					m_vecPrizeNum[2]->setVisible(true);

					m_vecPrizeImage[2]->loadTexture("ranking_03.png",Widget::TextureResType::PLIST);
					m_vecPrizeNum[2]->loadTexture("denglu_025.png",Widget::TextureResType::PLIST);
				}
				break;
			case 6:
				{
					m_vecPrizeImage[2]->setVisible(true);
					m_vecPrizeNum[2]->setVisible(true);

					m_vecPrizeImage[2]->loadTexture("ranking_03.png",Widget::TextureResType::PLIST);
					m_vecPrizeNum[2]->loadTexture("denglu_024.png",Widget::TextureResType::PLIST);
				}
				break;
			case 7:
				{
					m_RandNum = rand()&3;

					m_vecPrizeImage[0]->setVisible(true);
					m_vecPrizeNum[0]->setVisible(true);

					m_vecPrizeImage[1]->setVisible(true);
					m_vecPrizeNum[1]->setVisible(true);


					if ( m_RandNum == 0 )
					{
						m_vecPrizeImage[0]->loadTexture("ranking_20.png",Widget::TextureResType::PLIST);
						m_vecPrizeImage[1]->loadTexture("ranking_19.png",Widget::TextureResType::PLIST);

					}else if ( m_RandNum == 1 )
					{
						m_vecPrizeImage[0]->loadTexture("ranking_20.png",Widget::TextureResType::PLIST);
						m_vecPrizeImage[1]->loadTexture("ranking_21.png",Widget::TextureResType::PLIST);

					}else
					{
						m_vecPrizeImage[0]->loadTexture("ranking_21.png",Widget::TextureResType::PLIST);
						m_vecPrizeImage[1]->loadTexture("ranking_19.png",Widget::TextureResType::PLIST);
					}


					m_vecPrizeNum[0]->loadTexture("denglu_022.png",Widget::TextureResType::PLIST);
					m_vecPrizeNum[1]->loadTexture("denglu_022.png",Widget::TextureResType::PLIST);

				}
				break;
			default:
				{
					this->setVisible(false);
					CloseListener();
					return ;
				}
				break;
			}

		}
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void Login::SureTouchEvent(Ref* pSender, Widget::TouchEventType type)
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
			switch (m_CurrIndex)
			{
			case 1:
				{
					//SDKManager().bonus(500, bonus_coin_login);
					DBManager().AddData(DATA_DIAMOND_NUM,500);
					DBManager().AddData(DATA_DIAMOND_TOTAL,500);
				}
				break;
			case 2:
				{
					DBManager().AddData(DATA_SHIELD_NUM,1);
					DBManager().AddData(DATA_BOMB_NUM,1);
				}
				break;
			case 3:
				{
					//SDKManager().bonus(1000, bonus_coin_login);
					DBManager().AddData(DATA_DIAMOND_NUM,1000);
					DBManager().AddData(DATA_DIAMOND_TOTAL,1000);
				}
				break;
			case 4:
				{
					DBManager().AddData(DATA_SHIELD_NUM,2);
					DBManager().AddData(DATA_BOMB_NUM,2);
				}
				break;
			case 5:
				{
					//SDKManager().bonus(1500, bonus_coin_login);
					DBManager().AddData(DATA_DIAMOND_NUM,1500);
					DBManager().AddData(DATA_DIAMOND_TOTAL,1500);
				}
				break;
			case 6:
				{
					//SDKManager().bonus(2000, bonus_coin_login);
					DBManager().AddData(DATA_DIAMOND_NUM,2000);
					DBManager().AddData(DATA_DIAMOND_TOTAL,2000);
				}
				break;
			case 7:
				{
					if ( m_RandNum == 0 )
					{
						DBManager().AddData(DATA_SHIELD_NUM,3);
						DBManager().AddData(DATA_BOMB_NUM,3);

					}else if ( m_RandNum == 1 )
					{
						DBManager().AddData(DATA_SHIELD_NUM,3);
						DBManager().AddData(DATA_LIFE_NUM,3);

					}else
					{
						DBManager().AddData(DATA_LIFE_NUM,3);
						DBManager().AddData(DATA_BOMB_NUM,3);
					}
				}
				break;
			default:
				{
					return ;
				}
				break;
			}

			if ( m_CurrIndex == 7 )
			{
				m_CurrIndex = 0; 
			}

			DBManager().SetData(DATA_ALLNUM_RECEIVE,m_CurrIndex);
			DBManager().SetData(DATA_LASTTIME_RECEIVE, TimeSystem().GetDateTime() );
			CloseListener();
			this->setVisible(false);
			
			// 新手教学判定
			if ( DBManager().GetData( DATA_FRESH_MAN ) >= 1 )
			{
				dynamic_cast<MainScene*>(this->getParent())->ShowNiceGirl();

			}else
			{
				if ( NiceGirl::GetFreshManCurrStep() < 1 )
				{
					dynamic_cast<MainScene*>(this->getParent())->ShowNewPlayer();
				}
			}
		}
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void Login::OpenListener()
{
	// 创建监听
	auto listener = EventListenerTouchOneByOne::create();// 创建一个触摸监听(单点触摸）  
	listener->setSwallowTouches(true);					 // 禁止向下传递
	listener->onTouchBegan = CC_CALLBACK_2(Login::onTouchBegan, this);		// 指定触摸的回调函数  
	listener->onTouchEnded = CC_CALLBACK_2(Login::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);	// 将listener放入事件委托中 
}

void Login::CloseListener()
{
	_eventDispatcher->removeEventListenersForTarget(this);
}

bool Login::onTouchBegan(Touch *touch, Event *unused_event)
{
	return false;
}

void Login::onTouchEnded(Touch *touch, Event *unused_event)
{
	log(" lai le ");
}
