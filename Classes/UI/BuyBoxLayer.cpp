#include "BuyBoxLayer.h"
#include "Scene/MainScene.h"
#include "Common/FileUtils.h"
#include "DB/DBManager.h"
#include "TaskOption/TaskOptionMgr.h"
#include "Sound/LXSoundSystem.h"
#include "SDK/SDKManager.h"
#include "DataBase/DataBase.h"
#include "MyAnimation/NiceGirl.h"
#include "ObjectEx/LXObjectManager.h"

bool BuyBoxLayer::m_IsShowBettlePlane = true;

BuyBoxLayer::BuyBoxLayer(void):Layer()
{

	m_BuyType = BuyType::Buy_Null;
	m_BuyInfo = nullptr;
	m_BoxName = nullptr;
	m_OverflowBagNextType = BuyType::Buy_Null;
	m_CountDownDoneNextType = BuyType::Buy_Null;
	m_BigBuyBackImage = nullptr;
	m_IsPlayAnimation = true;
	m_Button_duihuan = nullptr;
	m_Button_lingqu = nullptr;
	m_Button_shengji = nullptr;
	m_Button_shiyong = nullptr;
	m_Button_zhuangbei = nullptr;
	
	m_BackImage = nullptr;
	m_CloseButton = nullptr;

	m_CountDownBack = nullptr;
	m_FrameImage = nullptr;
	M_NumberImageIn = nullptr;
	M_NumberImageOut = nullptr;

	m_GameSceneIsPause = false;
	m_IsPlayAnimation = true;
	m_DurationTime = 5;
	m_BuyWithCountDown = false;

}


BuyBoxLayer::~BuyBoxLayer(void)
{
}

bool BuyBoxLayer::init()
{
	// 1. super init first
	if ( !Layer::init() )
	{
		return false;
	}

	std::string key = "BuyBox";
	LXFileUtils::Get_UI_Json_FullPath(key);
	Layout* pBuyBoxLayout = static_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile(key.c_str()));
	if (!pBuyBoxLayout)
	{
		return false;
	}

	this->addChild(pBuyBoxLayout);

	/*--SmallBuy -------------------------------------------------------------------*/
	// ����
	m_SmallBuyLayout = static_cast<Layout*>(Helper::seekWidgetByName(pBuyBoxLayout,"SmallBuy"));

	// ����������ť
	Button* pSureButton	= static_cast<Button*>(Helper::seekWidgetByName(m_SmallBuyLayout, "Button_kaiqi"));
	pSureButton->addTouchEventListener(CC_CALLBACK_2(BuyBoxLayer::SureTouchEvent, this));

	// �رհ�ť
	Button* pCloseButton = static_cast<Button*>(Helper::seekWidgetByName(m_SmallBuyLayout, "Button_GB"));
	pCloseButton->addTouchEventListener(CC_CALLBACK_2(BuyBoxLayer::CloseTouchEvent, this));


	/*--BigBuy -------------------------------------------------------------------*/
	// ����
	m_BigBuyBackImage = static_cast<ImageView*>(Helper::seekWidgetByName(pBuyBoxLayout,"Image_BigBuy"));
	m_BigBuyLayout = static_cast<Layout*>(Helper::seekWidgetByName(m_BigBuyBackImage,"BigBuy"));
	
	m_BackImage = static_cast<ImageView*>(Helper::seekWidgetByName(m_BigBuyLayout,"Image_8"));

	//�һ���ť
	m_Button_duihuan	= static_cast<Button*>(Helper::seekWidgetByName(m_BigBuyLayout, "Button_duihuan"));
	m_Button_duihuan->addTouchEventListener(CC_CALLBACK_2(BuyBoxLayer::SureTouchEvent, this));

	m_Button_lingqu	= static_cast<Button*>(Helper::seekWidgetByName(m_BigBuyLayout, "Button_lingqu"));
	m_Button_lingqu->addTouchEventListener(CC_CALLBACK_2(BuyBoxLayer::SureTouchEvent, this));

	m_Button_shengji	= static_cast<Button*>(Helper::seekWidgetByName(m_BigBuyLayout, "Button_shengji"));
	m_Button_shengji->addTouchEventListener(CC_CALLBACK_2(BuyBoxLayer::SureTouchEvent, this));

	m_Button_shiyong	= static_cast<Button*>(Helper::seekWidgetByName(m_BigBuyLayout, "Button_shiyong"));
	m_Button_shiyong->addTouchEventListener(CC_CALLBACK_2(BuyBoxLayer::SureTouchEvent, this));

	m_Button_zhuangbei	= static_cast<Button*>(Helper::seekWidgetByName(m_BigBuyLayout, "Button_zhuangbei"));
	m_Button_zhuangbei->addTouchEventListener(CC_CALLBACK_2(BuyBoxLayer::SureTouchEvent, this));

	// �رհ�ť
	m_CloseButton = static_cast<Button*>(Helper::seekWidgetByName(m_BigBuyLayout, "Button_guanbi"));
	m_CloseButton->addTouchEventListener(CC_CALLBACK_2(BuyBoxLayer::CloseTouchEvent, this));

	// ����
	m_BoxName = static_cast<ImageView*>(Helper::seekWidgetByName(m_BigBuyLayout, "Image_Name"));

	// ͼƬ
	m_BuyInfo = static_cast<ImageView*>(Helper::seekWidgetByName(m_BigBuyLayout, "Image_Info"));

	// ����ʱ ---------------------------
	m_CountDownBack = dynamic_cast<Layout*>( Helper::seekWidgetByName(m_BigBuyBackImage, "Panel_daojishi") );
	m_FrameImage	= dynamic_cast<ImageView*>( Helper::seekWidgetByName(m_BigBuyBackImage,"Image_quanwai") );
	M_NumberImageIn = dynamic_cast<ImageView*>( Helper::seekWidgetByName(m_BigBuyBackImage,"Image_wenzi_li") );
	M_NumberImageOut= dynamic_cast<ImageView*>( Helper::seekWidgetByName(m_BigBuyBackImage,"Image_wenzi_wai") );


	m_Button_duihuan->setVisible(false);
	m_Button_lingqu->setVisible(false);
	m_Button_shengji->setVisible(false);
	m_Button_shiyong->setVisible(false);
	m_Button_zhuangbei->setVisible(false);
	m_CountDownBack->setVisible(false);

	return true;
}

void BuyBoxLayer::InitBuyType(BuyType eType, bool isPlayAnima /* = true */)
{
	m_BuyWithCountDown = false;
	m_CloseButton->setVisible(true);
	m_BackImage->loadTexture("MessageBox_1.png",Widget::TextureResType::PLIST);
	this->setVisible(true);
	

	m_IsPlayAnimation = isPlayAnima;
	m_BuyType = eType;

	m_Button_duihuan->setVisible(false);
	m_Button_lingqu->setVisible(false);
	m_Button_shengji->setVisible(false);
	m_Button_shiyong->setVisible(false);
	m_Button_zhuangbei->setVisible(false);

	switch (eType)
	{
	case Buy_Null:
		{
			this->setVisible(false);
			return ;
		}
		break;
	case Buy_Life:
		{
			m_Button_shiyong->setVisible(true);
			m_BoxName->loadTexture("MessageBox_14.png",Widget::TextureResType::PLIST);
			m_BuyInfo->loadTexture("MessageBox_15.png",Widget::TextureResType::PLIST);
		}
		break;
	case Buy_Shield:
		{
			m_Button_shiyong->setVisible(true);
			m_BoxName->loadTexture("MessageBox_16.png",Widget::TextureResType::PLIST);
			m_BuyInfo->loadTexture("MessageBox_17.png",Widget::TextureResType::PLIST);
		}
		break;
	case Buy_Bomb:
		{
			m_Button_shiyong->setVisible(true);
			m_BoxName->loadTexture("MessageBox_8.png",Widget::TextureResType::PLIST);
			m_BuyInfo->loadTexture("MessageBox_9.png",Widget::TextureResType::PLIST);
		}
		break;
	case Buy_Diamond:
		{
			m_Button_duihuan->setVisible(true);
			m_BoxName->loadTexture("MessageBox_6.png",Widget::TextureResType::PLIST);
			m_BuyInfo->loadTexture("MessageBox_7.png",Widget::TextureResType::PLIST);
		}
		break;
	case Buy_ChaoZhi:
		{
			m_Button_lingqu->setVisible(true);
			m_BoxName->loadTexture("MessageBox_10.png",Widget::TextureResType::PLIST);
			m_BuyInfo->loadTexture("MessageBox_11.png",Widget::TextureResType::PLIST);
		}
		break;
	case Buy_TuHaoJin:
		{
			m_Button_lingqu->setVisible(true);
			m_BoxName->loadTexture("MessageBox_18.png",Widget::TextureResType::PLIST);
			m_BuyInfo->loadTexture("MessageBox_19.png",Widget::TextureResType::PLIST);
		}
		break;
	case Buy_Plane3:
		{
			m_Button_zhuangbei->setVisible(true);
			m_BoxName->loadTexture("MessageBox_22.png",Widget::TextureResType::PLIST);
			m_BuyInfo->loadTexture("MessageBox_23.png",Widget::TextureResType::PLIST);
		}
		break;
	case Buy_Plane4:
		{
			m_Button_zhuangbei->setVisible(true);
			m_BoxName->loadTexture("MessageBox_22.png",Widget::TextureResType::PLIST);
			m_BuyInfo->loadTexture("MessageBox_25.png",Widget::TextureResType::PLIST);
		}
		break;
	case Buy_Genuine:
		{
			// ���ﲻ����κζ���
		}
		break;
	case Buy_PartDiamond:
		{
			m_Button_shengji->setVisible(true);
			m_BoxName->loadTexture("MessageBox_12.png",Widget::TextureResType::PLIST);
			m_BuyInfo->loadTexture("MessageBox_30.png",Widget::TextureResType::PLIST);
		}
		break;
	case Buy_PartWing:
		{
			m_Button_shengji->setVisible(true);
			m_BoxName->loadTexture("MessageBox_12.png",Widget::TextureResType::PLIST);
			m_BuyInfo->loadTexture("MessageBox_29.png",Widget::TextureResType::PLIST);
		}
		break;
	case Buy_PartFire:
		{
			m_Button_shengji->setVisible(true);
			m_BoxName->loadTexture("MessageBox_12.png",Widget::TextureResType::PLIST);
			m_BuyInfo->loadTexture("MessageBox_28.png",Widget::TextureResType::PLIST);
		}
		break;
	case Buy_PartErupt:
		{
			m_Button_shengji->setVisible(true);
			m_BoxName->loadTexture("MessageBox_12.png",Widget::TextureResType::PLIST);
			m_BuyInfo->loadTexture("MessageBox_27.png",Widget::TextureResType::PLIST);
		}
		break;
	case Buy_PartShield:
		{
			m_Button_shengji->setVisible(true);
			m_BoxName->loadTexture("MessageBox_12.png",Widget::TextureResType::PLIST);
			m_BuyInfo->loadTexture("MessageBox_13.png",Widget::TextureResType::PLIST);
		}
		break;
	case Buy_PartSkill:
		{
			m_Button_shengji->setVisible(true);
			m_BoxName->loadTexture("MessageBox_12.png",Widget::TextureResType::PLIST);
			m_BuyInfo->loadTexture("MessageBox_26.png",Widget::TextureResType::PLIST);
		}
		break;
	case Buy_AllPartUp:
		{
			m_Button_shengji->setVisible(true);
			m_BoxName->loadTexture("MessageBox_12.png",Widget::TextureResType::PLIST);
			m_BuyInfo->loadTexture("MessageBox_21.png",Widget::TextureResType::PLIST);
		}
		break;
	case Buy_FirstCharge:
		{
			m_Button_lingqu->setVisible(true);
			m_BoxName->loadTexture("MessageBox_31.png",Widget::TextureResType::PLIST);
			m_BuyInfo->loadTexture("MessageBox_32.png",Widget::TextureResType::PLIST);
		}
		break;
	default:
		{
			m_BuyType = Buy_Null;
			this->setVisible(false);
			return ;
		}
		break;
	}

	if ( eType == Buy_Genuine )
	{
		m_SmallBuyLayout->setVisible(true);
		m_BigBuyBackImage->setVisible(false);

	}else
	{
		m_SmallBuyLayout->setVisible(false);
		m_BigBuyBackImage->setVisible(true);

		if ( m_IsPlayAnimation )
		{
			m_BigBuyBackImage->setScale(0.0);

			ActionInterval* _ScaleToBig = ScaleTo::create(0.3,1);
			m_BigBuyBackImage->runAction(_ScaleToBig);
		}

	}

}

void BuyBoxLayer::InitOverflowBag( BuyType eType /* = BuyType::Buy_Diamond */, bool isPlayAnima /* = true */ )
{
	InitBuyType(BuyType::Buy_ChaoZhi,isPlayAnima);
	m_OverflowBagNextType = eType;
	m_CountDownDoneNextType = BuyType::Buy_Null;
	m_DurationTime = 0;

}


void BuyBoxLayer::InitCountDown( BuyType eType /* = BuyType::Buy_Life */, int durationTime /* = 5  */, bool isPlayAnima /* = true */ )
{
	m_CountDownDoneNextType = eType;
	m_OverflowBagNextType = BuyType::Buy_Null;
	m_DurationTime = 5;

	InitCountDownInfo();

	InitBuyType( BuyType::Buy_ChaoZhi, isPlayAnima );

	m_CloseButton->setVisible(false);
	m_BackImage->loadTexture("MessageBox_33.png",Widget::TextureResType::PLIST);
	m_BuyWithCountDown = true;
}

void BuyBoxLayer::GoToBuy()
{
	switch (m_BuyType)
	{
	case Buy_Null:
		{
			return ;
		}
		break;
	case Buy_Life:
		{

// #ifdef _DEBUG
// 			DBManager().AddMissionLife(1);
// #else
			DBManager().AddData(DATA_LIFE_NUM,5);
			DBManager().AddMissionLife(5);
// #endif // _DEBUG

			// �ɻ�����
			dynamic_cast<MainScene*>( this->getParent() )->PlaneRenascence();

			TaskMgr().DoneTaskOption(6);	// ��� ԭ�ظ��� ����
		}
		break;
	case Buy_Shield:
		{
			DBManager().AddData(DATA_SHIELD_NUM,5);
			DBManager().AddMissionShield(5);

			// �����ɻ�����
			dynamic_cast<MainScene*>( this->getParent() )->PlaneCreatShieldEffect();
			dynamic_cast<MainScene*>( this->getParent() )->GameContinue();

		}
		break;
	case Buy_Bomb:
		{
			DBManager().AddData(DATA_BOMB_NUM,5);
			DBManager().AddMissionBomb(5);

			dynamic_cast<MainScene*>( this->getParent() )->GameContinue();
		}
		break;
	case Buy_Diamond:
		{
			// ˢ����ʯ����
			//SDKManager().bonus(10000, bonus_coin_buy);
			DBManager().AddData(DATA_DIAMOND_NUM,10000);
			DBManager().AddData(DATA_DIAMOND_TOTAL,10000);
			
			if ( dynamic_cast<MainScene*>( this->getParent() )->IsShowUpgradeLayer() )
			{
				dynamic_cast<MainScene*>( this->getParent() )->RefreshUpgradeLayerState();
			}
			
		}
		break;
	case Buy_ChaoZhi:
		{
			//SDKManager().bonus(30000, bonus_coin_buy);
			DBManager().AddData(DATA_DIAMOND_NUM,30000);
			DBManager().AddData(DATA_DIAMOND_TOTAL,30000);
			DBManager().AddData(DATA_LIFE_NUM,1);
			DBManager().AddData(DATA_SHIELD_NUM,5);
			DBManager().AddData(DATA_BOMB_NUM,5);
			DBManager().AddMissionDiamon(30000);
			DBManager().AddMissionLife(1);
			DBManager().AddMissionShield(5);
			DBManager().AddMissionBomb(5);

			if( !ObjectManager().GetGameSceneNode() )
			{
				dynamic_cast<MainScene*>( this->getParent() )->RefreshUpgradeLayerState();
			}			

			if ( m_BuyWithCountDown )
			{
				// ����е���ʱ
				if ( this->isScheduled( schedule_selector( BuyBoxLayer::CountDownTick ) ) )
				{
					StopCountDownTick();
				}

				if ( m_CountDownDoneNextType == Buy_Life )
				{
					// �ɻ�����
					dynamic_cast<MainScene*>( this->getParent() )->PlaneRenascence();					
				}

			}else
			{
				if ( m_OverflowBagNextType == Buy_Bomb || m_OverflowBagNextType == Buy_Shield )
				{
					dynamic_cast<MainScene*>( this->getParent() )->PlaneCreatShieldEffect();
					dynamic_cast<MainScene*>( this->getParent() )->GameContinue();

				}else
				{
					// �ɻ�����
					dynamic_cast<MainScene*>( this->getParent() )->PlaneRenascence();
				}
			}

		}
		break;
	case Buy_TuHaoJin:
		{
			//SDKManager().bonus(70000, bonus_coin_buy);
			DBManager().AddData(DATA_DIAMOND_NUM,70000);
			DBManager().AddData(DATA_DIAMOND_TOTAL,70000);
			DBManager().AddData(DATA_LIFE_NUM,2);
			DBManager().AddData(DATA_SHIELD_NUM,8);
			DBManager().AddData(DATA_BOMB_NUM,8);

			if ( dynamic_cast<MainScene*>( this->getParent() )->IsShowUpgradeLayer() )
			{
				dynamic_cast<MainScene*>( this->getParent() )->RefreshUpgradeLayerState();
			}
		}
		break;
	case Buy_Plane3:
		{
			// ����3�ŷɻ�
			DBManager().SetData(DATA_HAVE_PLANE3,1);
			dynamic_cast<MainScene*>( this->getParent() )->ResfreshMenuLayerState();
		}
		break;
	case Buy_Plane4:
		{
			// ����3�ŷɻ�
			DBManager().SetData(DATA_HAVE_PLANE4,1);
			dynamic_cast<MainScene*>( this->getParent() )->ResfreshMenuLayerState();
			
			TaskMgr().DoneTaskOption(7);	// ��� ��ħ֮ŭ ����
		}
		break;
	case Buy_Genuine:
		{
			// ���湺��
			//SDKManager().bonus(2000, bonus_coin_buy);
			DBManager().SetData(DATA_ACTIVATION_TYPE,1);
			DBManager().AddData(DATA_DIAMOND_NUM,2000);
			DBManager().AddData(DATA_DIAMOND_TOTAL,2000);
			DBManager().AddData(DATA_SHIELD_NUM,3);
			DBManager().AddData(DATA_BOMB_NUM,3);
			dynamic_cast<MainScene*>( this->getParent() )->ResfreshMenuLayerState();
		}
		break;
	case Buy_PartDiamond:
		{
			DBManager().SetData(DATA_DIAMOND_LEVEL,5);
			dynamic_cast<MainScene*>( this->getParent() )->RefreshUpgradeLayerState();
		}
		break;
	case Buy_PartWing:
		{
			DBManager().SetData(DATA_WING_LEVEL,5);
			dynamic_cast<MainScene*>( this->getParent() )->RefreshUpgradeLayerState();
		}
		break;
	case Buy_PartFire:
		{
			DBManager().SetData(DATA_FIRE_LEVEL,5);
			dynamic_cast<MainScene*>( this->getParent() )->RefreshUpgradeLayerState();
		}
		break;
	case Buy_PartErupt:
		{
			DBManager().SetData(DATA_CRAZY_LEVEL,5);
			dynamic_cast<MainScene*>( this->getParent() )->RefreshUpgradeLayerState();
		}
		break;
	case Buy_PartShield:
		{
			DBManager().SetData(DATA_SHIELD_LEVEL,5);
			dynamic_cast<MainScene*>( this->getParent() )->RefreshUpgradeLayerState();
		}
		break;
	case Buy_PartSkill:
		{
			DBManager().SetData(DATA_BOMB_LEVEL,5);
			dynamic_cast<MainScene*>( this->getParent() )->RefreshUpgradeLayerState();
		}
		break;
	case Buy_AllPartUp:
		{
			DBManager().SetData(DATA_DIAMOND_LEVEL,5);
			DBManager().SetData(DATA_WING_LEVEL,5);
			DBManager().SetData(DATA_FIRE_LEVEL,5);
			DBManager().SetData(DATA_CRAZY_LEVEL,5);
			DBManager().SetData(DATA_SHIELD_LEVEL,5);
			DBManager().SetData(DATA_BOMB_LEVEL,5);
			dynamic_cast<MainScene*>( this->getParent() )->RefreshUpgradeLayerState();
		}
		break;
	case Buy_FirstCharge:
		{
			//SDKManager().bonus(20000, bonus_coin_buy);
			DBManager().SetData(DATA_FIRST_CHARGE,1);			
			DBManager().AddData(DATA_DIAMOND_NUM,20000);
			DBManager().AddData(DATA_DIAMOND_TOTAL,20000);
			DBManager().AddData(DATA_LIFE_NUM,1);
			DBManager().AddData(DATA_SHIELD_NUM,2);
			DBManager().AddData(DATA_BOMB_NUM,2);

			/*---------------------------------------------------------------------*/
			// ���ֽ�ѧ���
			if ( DBManager().GetData( DATA_FRESH_MAN ) < 1 )
			{
				if ( NiceGirl::GetFreshManCurrStep() ==  14 )
				{
					NiceGirl::SetFreshManCurrStep( 15 );

					dynamic_cast<MainScene*>(this->getParent())->ShowNewPlayer();
				}
			}
		}
		break;
	default:
		{
		}
		break;
	}

	this->setVisible(false);
}

void BuyBoxLayer::SureTouchEvent(Ref* pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
			SoundSystem().playEffect(SoundName::Mic_sure);

			// ��ȡ �۸�
			DBC_DEFINEHANDLE(s_pChargingDBC, DBC_CHARGING_INFO);
			const _DBC_CHARGING_INFO *pChargingInfo = (const _DBC_CHARGING_INFO*)s_pChargingDBC->Search_Index_EQU( m_BuyType );
			if ( !pChargingInfo )
			{
				log("BuyBoxLayer::SureTouchEvent cannot find m_BuyType:%d",m_BuyType);
				return ;
			}

			//����
			SDKManager().BuyItem(pChargingInfo->szName , m_BuyType, pChargingInfo->nIntValue);

		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void BuyBoxLayer::CloseTouchEvent(Ref* pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
			SoundSystem().playEffect(SoundName::Mic_back);
			this->setVisible(false);

			switch (m_BuyType)
			{
			case Buy_Null:
				{

				}
				break;
			case Buy_Life:
				{
					static_cast<MainScene*>(this->getParent())->LeaveGameSence( INTERFACE_ID::INTERFACE_MAIN );
					
					// ��ʾ���������
					InitBuyType(BuyType::Buy_TuHaoJin,true);
				}
				break;
			case Buy_Shield:
				{
					static_cast<MainScene*>(this->getParent())->GameContinue();
				}
				break;
			case Buy_Bomb:
				{
					static_cast<MainScene*>(this->getParent())->GameContinue();
				}
				break;
			case Buy_Diamond:
				{

				}
				break;
			case Buy_ChaoZhi:
				{
					int nType = m_OverflowBagNextType;

					if ( m_BuyWithCountDown )
					{
						if ( isScheduled( schedule_selector(BuyBoxLayer::CountDownTick) ) )
						{
							StopCountDownTick();
						}

						nType = m_CountDownDoneNextType;
					}
					

					switch (nType)
					{
					case Buy_Null:
						break;
					case Buy_Life:
						{
							InitBuyType(Buy_Life,m_IsPlayAnimation);
							return ;
						}
						break;
					case Buy_Shield:
						{
							InitBuyType(Buy_Shield,m_IsPlayAnimation);
							return ;
						}
						break;
					case Buy_Bomb:
						{
							InitBuyType(Buy_Bomb,m_IsPlayAnimation);
							return ;
						}
						break;
					case Buy_Diamond:
						{
							InitBuyType(Buy_Diamond,m_IsPlayAnimation);
							return ;
						}
						break;
					case Buy_ChaoZhi:
						break;
					case Buy_TuHaoJin:
						break;
					case Buy_Plane3:
						break;
					case Buy_Plane4:
						break;
					case Buy_Genuine:
						break;
					case Buy_PartDiamond:
						break;
					case Buy_PartWing:
						break;
					case Buy_PartFire:
						break;
					case Buy_PartErupt:
						break;
					case Buy_PartShield:
						break;
					case Buy_PartSkill:
						break;
					case Buy_AllPartUp:
						break;
					case Buy_FirstCharge:
						break;
					default:
						break;
					}

				}
				break;
			case Buy_TuHaoJin:
				{

				}
				break;
			case Buy_Plane3:
				{

				}
				break;
			case Buy_Plane4:
				{

				}
				break;
			case Buy_Genuine:
				{
					if ( m_IsShowBettlePlane )
					{
						// ��������ʾ��õķɻ�
						dynamic_cast<MainScene*>( this->getParent() )->ShowMenuLayerAndBetterPlane();
					}else
					{
						m_IsShowBettlePlane = true;
					}
					
				}
				break;
			case Buy_PartDiamond:
				{

				}
				break;
			case Buy_PartWing:
				{

				}
				break;
			case Buy_PartFire:
				{

				}
				break;
			case Buy_PartErupt:
				{

				}
				break;
			case Buy_PartShield:
				{

				}
				break;
			case Buy_PartSkill:
				{

				}
				break;
			case Buy_AllPartUp:
				{

				}
				break;
			case Buy_FirstCharge:
				{
					// ���ֽ�ѧ���
					if ( DBManager().GetData( DATA_FRESH_MAN ) < 1 )
					{
						if ( NiceGirl::GetFreshManCurrStep() ==  14 )
						{
							NiceGirl::SetFreshManCurrStep( 15 );

							dynamic_cast<MainScene*>(this->getParent())->ShowNewPlayer();
						}
					}
				}
				break;
			default:
				{
					return ;
				}
				break;
			}		
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void BuyBoxLayer::InitCountDownInfo()
{
	std::stringstream str;
	str << "zanting_m" << m_DurationTime << ".png";

	M_NumberImageIn->loadTexture( str.str(), Widget::TextureResType::PLIST );
	M_NumberImageOut->loadTexture( str.str(), Widget::TextureResType::PLIST );

	schedule( schedule_selector( BuyBoxLayer::CountDownTick ),1.0f );

	m_CountDownBack->setVisible(true);

}

void BuyBoxLayer::CountDownTick(float dt)
{
	-- m_DurationTime;

	if ( m_DurationTime <= 0 )
	{
		unschedule( schedule_selector( BuyBoxLayer::CountDownTick ) );

		m_CountDownBack->setVisible(false);
		this->setVisible(false);

		InitBuyType( static_cast<BuyType>(m_CountDownDoneNextType), m_IsPlayAnimation );

	}else
	{
		// ���� �Ŵ� and ���� -- ����ʱ 5 4 3 2 1
		ActionInterval* _scaleto1	=	ScaleTo::create( 0.01, 1 );			// ��λ
		ActionInterval* _fadeIn		=	FadeIn::create( 0.01 );				// ����
		ActionInstant*	_show		=	Show::create();						// ��ʾ
		ActionInterval* _scaleto2	=	ScaleTo::create( 0.7f, 1.5 );		// �Ŵ�	1.5
		ActionInterval* _scaleto3	=	ScaleTo::create( 0.7f, 2.0 );		// �Ŵ�	2.0
		ActionInterval* _fadeout	=	FadeOut::create( 0.7f );			// ����
		ActionInstant*  _hide		=	Hide::create();

		FiniteTimeAction* _spawn1 =		Spawn::create( _scaleto2, _fadeout, NULL );		// �Ŵ� �� ���� ͬʱ����
		FiniteTimeAction* _spawn2 =		Spawn::create( _scaleto3, _fadeout, NULL );		// �Ŵ� �� ���� ͬʱ����

		ActionInterval* _sequenc1_1 =	Sequence::create( _scaleto1, _fadeIn, _show, _spawn1, _hide, NULL );

		ActionInstant*	_CallFun	=	CallFunc::create( CC_CALLBACK_0( BuyBoxLayer::CountDownAnimationCallBack,this ) );
		ActionInterval* _sequenc1_2 =	Sequence::create( _scaleto1->clone(), _fadeIn->clone(), _show->clone(), _spawn2, _hide->clone(), _CallFun, NULL );

		m_FrameImage->runAction( _sequenc1_1 );
		M_NumberImageOut->runAction( _sequenc1_2 );
	}
}

void BuyBoxLayer::CountDownAnimationCallBack()
{
	if ( m_DurationTime > 0 )
	{
		std::stringstream str;
		str << "zanting_m" << m_DurationTime << ".png";
		M_NumberImageIn->loadTexture( str.str(), Widget::TextureResType::PLIST );
		M_NumberImageOut->loadTexture( str.str(), Widget::TextureResType::PLIST );
	}
}

void BuyBoxLayer::StopCountDownTick()
{
	M_NumberImageOut->stopAllActions();
	m_FrameImage->stopAllActions();

	unschedule( schedule_selector(BuyBoxLayer::CountDownTick) );
	m_CountDownBack -> setVisible(false);
	m_DurationTime = 0;
}