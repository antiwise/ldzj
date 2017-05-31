#include "BattleLayer.h"
#include "Scene/GameScene.h"
#include "DB/DBManager.h"
#include "ObjectEx/Plane/LXPlayerPlane.h"
#include "ObjectEx/LXObjectManager.h"
#include "Effect/LXEffectManager.h"
#include "GameDefine.h"
#include "MapLayer.h"
#include "Sound/LXSoundSystem.h"
#include "Common/FileUtils.h"
#include "Scene/MainScene.h"
#include "MyAnimation/NiceGirl.h"

BattleLayer::BattleLayer():Layer()
{
	m_tempScoreNum = 0;
	m_tempShieldNum = 0;
	m_tempBombNum = 0;
	m_tempLifeNum = 0;

	m_Pause = nullptr;
	m_Score = nullptr;
	m_BloodYellow = nullptr;
	m_BloodRed = nullptr;
	m_Bomb = nullptr;
	m_Protect = nullptr;
	m_BombNum = nullptr;
	m_ProtectNum = nullptr;
	m_PlaneLifeNum = nullptr;
	m_BattleLayout = nullptr;
}

BattleLayer::~BattleLayer()
{

}

bool BattleLayer::init()
{
	// 1. super init first
	if ( !Layer::init() )
	{
		return false;
	}

	// 暂停界面
	std::string key = "fight";
	LXFileUtils::Get_UI_Json_FullPath(key);
	m_BattleLayout = static_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile(key.c_str()));
	if (!m_BattleLayout)
	{
		return false;
	}

	this->addChild(m_BattleLayout);

	m_Pause = static_cast<ImageView*>(Helper::seekWidgetByName(m_BattleLayout,"Button_pause"));
	m_Pause->addTouchEventListener(CC_CALLBACK_2(BattleLayer::PauseEvent,this));

	m_Bomb = static_cast<Button*>(Helper::seekWidgetByName(m_BattleLayout,"Button_bomb"));
	m_Bomb->addTouchEventListener(CC_CALLBACK_2(BattleLayer::BombEvent,this));

	m_Protect = static_cast<Button*>(Helper::seekWidgetByName(m_BattleLayout,"Button_protect"));
	m_Protect->addTouchEventListener(CC_CALLBACK_2(BattleLayer::ProtectEvent,this));

	m_Score = static_cast<TextAtlas*>(Helper::seekWidgetByName(m_BattleLayout,"AtlasLabel_score"));
	m_Score->setString("0");

	m_BloodYellow = static_cast<LoadingBar*>(Helper::seekWidgetByName(m_BattleLayout,"ProgressBar_blood"));
	m_BloodYellow->setVisible(false);

	m_BloodRed = static_cast<LoadingBar*>(Helper::seekWidgetByName(m_BattleLayout,"ProgressBar_bloodRed"));
	m_BloodRed->setVisible(false);

	m_BombNum = static_cast<TextAtlas*>(Helper::seekWidgetByName(m_BattleLayout,"bishashu"));
	m_BombNum->setString("0");

	m_ProtectNum = static_cast<TextAtlas*>(Helper::seekWidgetByName(m_BattleLayout,"hudunshu"));
	m_ProtectNum->setString("0");

	return true;
}

void	BattleLayer::Tick(float dt)
{
	if ( m_tempScoreNum != DBManager().GetMissionScore() )
	{
		m_tempScoreNum = DBManager().GetMissionScore();
		m_Score->setString( StringUtils::toString( m_tempScoreNum ) );
	}

	if ( m_tempShieldNum != DBManager().GetMissionShield() )
	{
		m_tempShieldNum = DBManager().GetMissionShield();
		m_ProtectNum->setString( StringUtils::toString(m_tempShieldNum) );
	}
	
	if ( m_tempBombNum != DBManager().GetMissionBomb() )
	{
		m_tempBombNum = DBManager().GetMissionBomb();
		m_BombNum->setString( StringUtils::toString(m_tempBombNum) );
	}
	if ( m_tempLifeNum != DBManager().GetMissionLife() )
	{
		m_tempLifeNum = DBManager().GetMissionLife();
		ShowLifePlane();
	}
}

void	BattleLayer::PauseEvent(Ref* pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		{
			//调用暂停界面
			SoundSystem().playEffect(Mic_pause);
			static_cast<GameScene*>(this->getParent()->getParent())->GamePause();
		}
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void	BattleLayer::BombEvent(Ref* pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		{

			/*---------------------------------------------------------------------*/
			// 新手教学 时不消耗炸弹
			if ( DBManager().GetData( DATA_FRESH_MAN ) < 1 )
			{
				if ( NiceGirl::GetFreshManCurrStep() == 11 )
				{
					SoundSystem().playEffect(Mic_shield);// 声音

					LXPlayerPlane* Player = dynamic_cast<LXPlayerPlane*>(ObjectManager().GetPlayer());
					if (Player)
					{
						Player->ActiveBombPlane();

						NiceGirl::SetFreshManCurrStep( 12 );

						Director::getInstance()->resume();

						dynamic_cast<MainScene*>(this->getParent()->getParent()->getParent())->ShowNewPlayer();

						return ;
					}
				}
			}
			/*---------------------------------------------------------------------*/


			//释放炸弹
			LXPlayerPlane* Player = dynamic_cast<LXPlayerPlane*>(ObjectManager().GetPlayer());
			if (Player)
			{
				if (Player->IsBombPlaneActive() == false)
				{
					if ( DBManager().GetMissionBomb() > 0 )
					{
						// 使用护盾
						DBManager().UseMissionBomb();
						Player->ActiveBombPlane();
						// com.sg.raiden.b.b.l.M(); 必杀成就计算
					}else
					{
						// 购买道具
						dynamic_cast<MainScene*>( this->getParent()->getParent()->getParent())->ShowOverflowBag( BuyType::Buy_Bomb,false );
						dynamic_cast<GameScene*>(this->getParent()->getParent())->GamePauseAndNoSuspendLayer();
					}
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

void	BattleLayer::ProtectEvent(Ref* pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		{
			// 在 b.c.a.f.d()

			// w.c("shield.ogg");

			/*---------------------------------------------------------------------*/
			// 新手教学 时不消耗护盾
			if ( DBManager().GetData( DATA_FRESH_MAN ) < 1 )
			{
				if ( NiceGirl::GetFreshManCurrStep() == 8 )
				{
					SoundSystem().playEffect(Mic_shield);// 声音

					LXPlayerPlane* Player = dynamic_cast<LXPlayerPlane*>(ObjectManager().GetPlayer());
					if (Player)
					{
						Player->SetProtectTime(1.0f);
						Player->CreatProtectEffect();
						Player->ShowProtectEffect();

						NiceGirl::SetFreshManCurrStep( 9 );

						dynamic_cast<MainScene*>(this->getParent()->getParent()->getParent())->ShowNewPlayer();

						return ;
					}
				}
			}
			/*---------------------------------------------------------------------*/

			//释放护盾
			if ( DBManager().GetMissionShield() > 0 )
			{
				SoundSystem().playEffect(Mic_shield);// 声音
				// 使用护盾
				DBManager().UseMissionShield();

				// 添加震屏效果

				LXPlayerPlane* Player = dynamic_cast<LXPlayerPlane*>(ObjectManager().GetPlayer());
				if (Player)
				{
					Player->SetProtectTime(1.0f);
					Player->CreatProtectEffect();
					Player->ShowProtectEffect();
				}
			}else
			{
				//p.b(true);				// 粒子特效
				//w.c();					// 音效
				//a(0);						// 暂停游戏。购买护盾 调用 0 号道具购买界面

				dynamic_cast<MainScene*>( this->getParent()->getParent()->getParent())->ShowOverflowBag( BuyType::Buy_Shield,false );
				dynamic_cast<GameScene*>(this->getParent()->getParent())->GamePauseAndNoSuspendLayer();
			}
		}
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void BattleLayer::ShowLifePlane()
{
	// 清理
	if (!m_BattleLayout)
	{
		return ;
	}

	for ( auto it : m_listPlaneLife )
	{
		m_BattleLayout->removeChild(it);
	}

	m_listPlaneLife.clear();

	if (m_PlaneLifeNum)
	{
		m_BattleLayout->removeChild(m_PlaneLifeNum);
	}

	// 路径
	std::string var[] = {"022-2", "023-2", "024-2", "025-2", "027"};

	auto glview = Director::getInstance()->getOpenGLView();
	const auto screenSize = glview->getDesignResolutionSize();

	int valX = m_Pause->getPositionX() - 25;
	int valY = screenSize.height - 60;
	int nFireNum = DBManager().GetMissionLife();
	
	std::stringstream imagePath;
	imagePath << "play_"<<  var[DBManager().GetCurrPlaneID()] << ".png";

	if ( nFireNum > 6 )
	{
		// 创建飞机图片
		ImageView* image  = ImageView::create(imagePath.str(),Widget::TextureResType::PLIST);
		if (!image)
		{
			return ;
		}
		image->setPositionX( 2.0f + valX );
		image->setPositionY(valY);

		// 创建飞机图片 数字
		std::stringstream str;
		str << "X" << nFireNum;

		m_PlaneLifeNum = Text::create(str.str(),"微软雅黑",14);
		if (!m_PlaneLifeNum)
		{
			return ;
		}
		m_PlaneLifeNum->setColor( Color3B( 0.99215686F*255, 0.85490197F*255, 0.3137255F*255 ) );
		m_PlaneLifeNum->setPositionX( image->getContentSize().width + valX);
		m_PlaneLifeNum->setPositionY( valY - 7 );

		m_BattleLayout->addChild(image);
		m_BattleLayout->addChild(m_PlaneLifeNum);
		
		m_listPlaneLife.push_back(image);

	}else
	{
		for (int i = 0; i < nFireNum; i++)
		{
			ImageView* image  = ImageView::create( imagePath.str(),Widget::TextureResType::PLIST);
			if (!image)
			{
				return ;
			}			
			image->setPositionX( i*( 2.0f + image->getContentSize().width ) + valX );
			image->setPositionY( valY );
			
			m_BattleLayout->addChild(image);

			m_listPlaneLife.push_back(image);
		}
	}

}

void	BattleLayer::ShowBloodBar(bool bShow)
{ 
	m_BloodYellow->setPercent(100);
	m_BloodRed->setPercent(100);

	m_BloodYellow->setVisible(bShow);
	m_BloodRed->setVisible(bShow);
}

void	BattleLayer::SetBloodPecent(float percent)
{
	if (percent > 50)
	{
		m_BloodYellow->setPercent((percent-50)*2);
	}
	else
	{
		if (m_BloodYellow->isVisible())
		{
			m_BloodYellow->setVisible(false);
		}
		m_BloodRed->setPercent(percent*2);
	}

}

void BattleLayer::StopClickEventWithButton()
{
	m_Pause->setTouchEnabled(false);
	m_Bomb->setTouchEnabled(false);
	m_Protect->setTouchEnabled(false);
}

void BattleLayer::StartClickEventWithButton()
{
	m_Pause->setTouchEnabled(true);
	m_Bomb->setTouchEnabled(true);
	m_Protect->setTouchEnabled(true);
}