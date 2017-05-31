#include "SettingLayer.h"
#include "Scene/MainScene.h"
#include "Common/FileUtils.h"
#include "DB/DBManager.h"
#include "Sound/LXSoundSystem.h"
#include "Common/LXUtils.h"

SettingLayer::SettingLayer(void):Layer()
{
	m_btn_Close = nullptr;
	m_btn_Help = nullptr;
	m_btn_About = nullptr;
	m_cb_BgMusic = nullptr;
	m_cb_Effect = nullptr;
	m_EjectImage = nullptr;
	m_EjectBackImage = nullptr;
	m_EjectClose = nullptr;
}


SettingLayer::~SettingLayer(void)
{
}

bool SettingLayer::init()
{
	 // 1. super init first
	 if ( !Layer::init() )
	 {
		 return false;
	 }

	 LXFileUtils::AddSpriteFrameCache("ui/MessageBox.plist","ui/MessageBox.png");
	 LXFileUtils::AddSpriteFrameCache("ui/ranking.plist","ui/ranking.png");

	 std::string key = "Setings";
	 LXFileUtils::Get_UI_Json_FullPath(key);
	 Layout* pSettingLayout = static_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile(key.c_str()));
	 if (!pSettingLayout)
	 {
		 return false;
	 }

	 this->addChild(pSettingLayout);

	 m_btn_Close =  static_cast<Button*>(Helper::seekWidgetByName(pSettingLayout,"Button_Close"));
	 m_btn_Close->addTouchEventListener(CC_CALLBACK_2(SettingLayer::CloseEvent, this));

	 m_btn_Help = static_cast<Button*>(Helper::seekWidgetByName(pSettingLayout,"Button_Help"));
	 m_btn_Help->addTouchEventListener(CC_CALLBACK_2(SettingLayer::HelpEvent, this));

	 m_btn_About = static_cast<Button*>(Helper::seekWidgetByName(pSettingLayout,"Button_Info"));
	 m_btn_About->addTouchEventListener(CC_CALLBACK_2(SettingLayer::AboutEvent, this));

	 m_cb_BgMusic = static_cast<CheckBox*>(Helper::seekWidgetByName(pSettingLayout,"CheckBox_BG_MUSIC"));
	 m_cb_BgMusic->addTouchEventListener(CC_CALLBACK_2(SettingLayer::BgMusicEvent, this));
	 

	 m_cb_Effect = static_cast<CheckBox*>(Helper::seekWidgetByName(pSettingLayout,"CheckBox_EFFECT"));
	 m_cb_Effect->addTouchEventListener(CC_CALLBACK_2(SettingLayer::EffectEvent, this));
	 

	 /*---------------------------------------------------------------------*/

	 int nWidth = Utils().GetDesignWidth();
	 int nHeight = Utils().GetDesignHeight();

	 m_EjectBackImage = ImageView::create( "MessageBox_01.png",Widget::TextureResType::PLIST );
	 if ( !m_EjectBackImage )
	 {
		 return false;
	 }

	 m_EjectBackImage->setOpacity(200);
	 m_EjectBackImage->setScaleX( nWidth/24 + 100 );
	 m_EjectBackImage->setScaleY(  nHeight/24 + 100 );
	 m_EjectBackImage->setPosition( Vec2( nWidth/2-24, nHeight/2 ) );
	 m_EjectBackImage->setTouchEnabled(true);

	 this->addChild( m_EjectBackImage );

	 m_EjectImage = ImageView::create("settings_10.png",Widget::TextureResType::PLIST);
	 if ( !m_EjectImage )
	 {
		 return false;
	 }

	 m_EjectImage->setPosition( Vec2(  nWidth/2 + 9, nHeight/2 - 7 ) );
	 this->addChild( m_EjectImage );

	 m_EjectClose = Button::create( "ranking_08.png", "ranking_09.png", "", Widget::TextureResType::PLIST );
	 if ( !m_EjectClose )
	 {
		 return false;
	 }
	 m_EjectImage->addChild( m_EjectClose );
	 m_EjectClose->setPosition( Vec2(403,579) );
	 m_EjectClose->addTouchEventListener( CC_CALLBACK_2( SettingLayer::EjectCloseTouchEvent, this ) );

	 m_EjectBackImage->setVisible(false);
	 m_EjectImage->setVisible(false);

	 return true;
}

void SettingLayer::Init()
{
	if (DBManager().GetData(DATA_MUSIC_ON))
	{
		m_cb_BgMusic->setSelectedState(true);
	}
	else
	{
		m_cb_BgMusic->setSelectedState(false);
	}

	if (DBManager().GetData(DATA_EFFECT_ON))
	{
		m_cb_Effect->setSelectedState(true);
	}
	else
	{
		m_cb_Effect->setSelectedState(false);
	}
}

void SettingLayer::CloseEvent( Ref* pSender, Widget::TouchEventType type )
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		{
			SoundSystem().playEffect(Mic_sure);
			dynamic_cast<MainScene*>(this->getParent())->CloseSettingLayer();
		}
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void SettingLayer::HelpEvent( Ref* pSender, Widget::TouchEventType type )
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		{
			SoundSystem().playEffect(Mic_sure);
			return; //фа╠н

			m_EjectBackImage->setVisible(true);
			m_EjectImage->setVisible(true);
			m_EjectImage->setPosition( Vec2(  Utils().GetDesignWidth()/2 + 8, Utils().GetDesignHeight()/2 - 7 ) );
			m_EjectImage->loadTexture("settings_10.png",Widget::TextureResType::PLIST);
			m_EjectClose->setPosition( Vec2(409,579) );
		}
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void SettingLayer::AboutEvent( Ref* pSender, Widget::TouchEventType type )
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		{
			SoundSystem().playEffect(Mic_sure);
			return; //фа╠н

			m_EjectBackImage->setVisible(true);
			m_EjectImage->setVisible(true);
			m_EjectImage->setPosition( Vec2(  Utils().GetDesignWidth()/2 + 8, Utils().GetDesignHeight()/2 - 7 ) );
			m_EjectImage->loadTexture("settings_11.png",Widget::TextureResType::PLIST);
			m_EjectClose->setPosition( Vec2(410,579) );
		}
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void SettingLayer::BgMusicEvent( Ref* pSender, Widget::TouchEventType type )
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		{
			
			if (DBManager().GetData(DATA_MUSIC_ON))
			{
				DBManager().SetData(DATA_MUSIC_ON,0);
				SoundSystem().setBackgroundMusicState(false);
			}
			else
			{
				DBManager().SetData(DATA_MUSIC_ON,1);
				SoundSystem().setBackgroundMusicState(true);
			}
			SoundSystem().playEffect(Mic_sure);
			DBManager().SaveDB();
			
		}
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void SettingLayer::EffectEvent( Ref* pSender, Widget::TouchEventType type )
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		{
			
			if (DBManager().GetData(DATA_EFFECT_ON))
			{
				DBManager().SetData(DATA_EFFECT_ON,0);
				SoundSystem().setEffectState(false);
				SoundSystem().stopAllEffects();
			}
			else
			{
				DBManager().SetData(DATA_EFFECT_ON,1);
				SoundSystem().setEffectState(true);
			}
			SoundSystem().playEffect(Mic_sure);
			DBManager().SaveDB();
		}
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void SettingLayer::EjectCloseTouchEvent(Ref* pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		{
			m_EjectBackImage->setVisible(false);
			m_EjectImage->setVisible(false);
		}
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}