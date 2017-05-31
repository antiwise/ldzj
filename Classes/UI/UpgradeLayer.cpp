#include "UpgradeLayer.h"
#include "Scene/MainScene.h"
#include "GameDefine.h"
#include "DB/DBManager.h"
#include "DataBase/DataBase.h"
#include "Common/FileUtils.h"
#include "TaskOption/TaskOptionMgr.h"
#include "UI/MyAnimation/TuHaoJin.h"
#include "Common/LXUtils.h"
#include "Sound/LXSoundSystem.h"
#include "MyAnimation/NiceGirl.h"

UpgradeLayer::UpgradeLayer():Layer()
{
	m_Diamond = nullptr;			// 拥有宝石
	m_UpgradeCost = nullptr;		// 升级消耗

	m_Upgrade = nullptr;			// 升级
	m_Max = nullptr;				// 升至满级
	m_MaxAll = nullptr;				// 一键满级
	m_Back = nullptr;				// 返回
	m_Help = nullptr;				// 帮助
	m_BuyDiamond = nullptr;			// 获得宝石

	m_Describe = nullptr;			// 描述

	m_DiamonCB = nullptr;			// 宝石转化
	m_WingCB = nullptr;				// 僚机支援
	m_FireCB = nullptr;				// 粒子光炮
	m_EruptCB = nullptr;			// 暴走攻击
	m_ShieldCB = nullptr;			// 量子护盾
	m_SkillCB = nullptr;			// 必杀攻击

	int length = 6;
	for (int i = 0; i < length; i++)
	{

		m_CheckBox[i] = nullptr;		// checkbox合集

	}
	
	length = 5;
	for (int i = 0; i < length; i++)
	{
		m_DiamondLevel[i] = nullptr;	// 宝石等级
		m_WingLevel[i] = nullptr;		// 僚机等级
		m_FireLevel[i] = nullptr;		// 光炮等级
		m_EruptLevel[i] = nullptr;		// 暴走等级
		m_ShieldLevel[i] = nullptr;		// 护盾等级
		m_SkillLevel[i] = nullptr;		// 必杀等级
	}

	
	m_HelpBackImage = nullptr;	//帮助图片背景 
	m_HelpImage = nullptr;		//帮助图片
	m_HelpClose = nullptr;		//帮助关闭按钮
}

UpgradeLayer::~UpgradeLayer()
{

}

bool UpgradeLayer::init()
{
	// 1. super init first
	if ( !Layer::init() )
	{
		return false;
	}

	// 暂停界面
	std::string key = "upgrade";
	LXFileUtils::Get_UI_Json_FullPath(key);
	Layout* upgradeLayer = static_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile( key.c_str() ));
	if (!upgradeLayer)
	{
		return false;
	}

	this->addChild(upgradeLayer);

	//按钮
	m_Upgrade = static_cast<Button*>(Helper::seekWidgetByName(upgradeLayer,"Button_upgrade"));
	m_Upgrade->addTouchEventListener(CC_CALLBACK_2(UpgradeLayer::UpgradeEvent,this));

	m_Max = static_cast<Button*>(Helper::seekWidgetByName(upgradeLayer,"Button_full"));
	m_Max->addTouchEventListener(CC_CALLBACK_2(UpgradeLayer::MaxEvent,this));

	m_MaxAll = static_cast<Button*>(Helper::seekWidgetByName(upgradeLayer,"Button_fullall"));
	m_MaxAll->addTouchEventListener(CC_CALLBACK_2(UpgradeLayer::MaxAllEvent,this));

	m_Back = static_cast<Button*>(Helper::seekWidgetByName(upgradeLayer,"Button_back"));
	m_Back->addTouchEventListener(CC_CALLBACK_2(UpgradeLayer::BackEvent,this));

	m_Help = static_cast<Button*>(Helper::seekWidgetByName(upgradeLayer,"Button_help"));
	m_Help->addTouchEventListener(CC_CALLBACK_2(UpgradeLayer::HelpEvent,this));

	m_BuyDiamond = static_cast<Button*>(Helper::seekWidgetByName(upgradeLayer,"Button_buydiamond"));
	m_BuyDiamond->addTouchEventListener(CC_CALLBACK_2(UpgradeLayer::BuyEvent,this));

	// 数字

	m_Diamond = static_cast<TextAtlas*>(Helper::seekWidgetByName(upgradeLayer,"AtlasLabel_diamonds"));
	//m_Diamond->setString("0");

	m_UpgradeCost = static_cast<TextAtlas*>(Helper::seekWidgetByName(upgradeLayer,"AtlasLabel_cost"));
	//m_UpgradeCost->setString("0");

	//描述
	m_Describe = static_cast<Text*>(Helper::seekWidgetByName(upgradeLayer,"Label_info"));
	//m_Describe->setString("123456789");

	//选择
	m_DiamonCB = static_cast<CheckBox*>(Helper::seekWidgetByName(upgradeLayer,"CheckBox_diamond"));
	m_DiamonCB->addTouchEventListener(CC_CALLBACK_2(UpgradeLayer::DiamondEvent,this));
	m_DiamonCB->setSelectedState(true);

	m_WingCB = static_cast<CheckBox*>(Helper::seekWidgetByName(upgradeLayer,"CheckBox_wing"));
	m_WingCB->addTouchEventListener(CC_CALLBACK_2(UpgradeLayer::WingEvent,this));
	m_WingCB->setSelectedState(false);

	m_FireCB = static_cast<CheckBox*>(Helper::seekWidgetByName(upgradeLayer,"CheckBox_artillery"));
	m_FireCB->addTouchEventListener(CC_CALLBACK_2(UpgradeLayer::FireEvent,this));
	m_FireCB->setSelectedState(false);

	m_EruptCB = static_cast<CheckBox*>(Helper::seekWidgetByName(upgradeLayer,"CheckBox_change"));
	m_EruptCB->addTouchEventListener(CC_CALLBACK_2(UpgradeLayer::EruptEvent,this));
	m_EruptCB->setSelectedState(false);

	m_ShieldCB = static_cast<CheckBox*>(Helper::seekWidgetByName(upgradeLayer,"CheckBox_protect"));
	m_ShieldCB->addTouchEventListener(CC_CALLBACK_2(UpgradeLayer::ShieldEvent,this));
	m_ShieldCB->setSelectedState(false);

	m_SkillCB = static_cast<CheckBox*>(Helper::seekWidgetByName(upgradeLayer,"CheckBox_kill"));
	m_SkillCB->addTouchEventListener(CC_CALLBACK_2(UpgradeLayer::SkillEvent,this));
	m_SkillCB->setSelectedState(false);

	//方便遍历
	m_CheckBox[0] = m_DiamonCB;
	m_CheckBox[1] = m_WingCB;
	m_CheckBox[2] = m_FireCB;
	m_CheckBox[3] = m_EruptCB;
	m_CheckBox[4] = m_ShieldCB;
	m_CheckBox[5] = m_SkillCB;


	//等级标识
	for (int i = 0; i < 5; i++ )
	{
		m_DiamondLevel[i] = static_cast<ImageView*>(Helper::seekWidgetByName(upgradeLayer,"Image_dm" + StringUtils::toString(i+1)));
		m_WingLevel[i] = static_cast<ImageView*>(Helper::seekWidgetByName(upgradeLayer,"Image_wing" + StringUtils::toString(i+1)));
		m_FireLevel[i] = static_cast<ImageView*>(Helper::seekWidgetByName(upgradeLayer,"Image_ar" + StringUtils::toString(i+1)));
		m_EruptLevel[i] = static_cast<ImageView*>(Helper::seekWidgetByName(upgradeLayer,"Image_ch" + StringUtils::toString(i+1)));
		m_ShieldLevel[i] = static_cast<ImageView*>(Helper::seekWidgetByName(upgradeLayer,"Image_pt" + StringUtils::toString(i+1)));
		m_SkillLevel[i] = static_cast<ImageView*>(Helper::seekWidgetByName(upgradeLayer,"Image_k" + StringUtils::toString(i+1)));

		m_DiamondLevel[i]->setVisible(false);
		m_WingLevel[i]->setVisible(false);
		m_FireLevel[i]->setVisible(false);
		m_EruptLevel[i]->setVisible(false);
		m_ShieldLevel[i]->setVisible(false);
		m_SkillLevel[i]->setVisible(false);
	}

	m_nSelectedType = DATA_DIAMOND_LEVEL;
	
	/*---------------------------------------------------------------------*/
	// 礼包添加
	TuHaoJin* _tuhaojin = TuHaoJin::create();
	if (_tuhaojin)
	{
		_tuhaojin->Init(0.67f,-17.0f);
		_tuhaojin->setPosition( Utils().GetFixedPosion( 400, 260 ) );
		this->addChild( _tuhaojin );
	}


	/*---------------------------------------------------------------------*/

	LXFileUtils::AddSpriteFrameCache("ui/MessageBox.plist","ui/MessageBox.png");
	LXFileUtils::AddSpriteFrameCache("ui/ranking.plist","ui/ranking.png");
	LXFileUtils::AddSpriteFrameCache("ui/settings.plist","ui/settings.png");

	int nWidth = Utils().GetDesignWidth();
	int nHeight = Utils().GetDesignHeight();

	m_HelpBackImage = ImageView::create( "MessageBox_01.png",Widget::TextureResType::PLIST );
	if ( !m_HelpBackImage )
	{
		return false;
	}

	m_HelpBackImage->setOpacity(200);
	m_HelpBackImage->setScaleX( nWidth/24 + 100 );
	m_HelpBackImage->setScaleY(  nHeight/24 + 100 );
	m_HelpBackImage->setPosition( Vec2( nWidth/2-24, nHeight/2 ) );
	m_HelpBackImage->setTouchEnabled(true);

	this->addChild( m_HelpBackImage );

	m_HelpImage = ImageView::create("settings_12.png",Widget::TextureResType::PLIST);
	if ( !m_HelpImage )
	{
		return false;
	}

	m_HelpImage->setPosition( Vec2(  nWidth/2 + 8, nHeight/2 - 7 ) );
	this->addChild( m_HelpImage );

	m_HelpClose = Button::create( "ranking_08.png", "ranking_09.png", "", Widget::TextureResType::PLIST );
	if ( !m_HelpClose )
	{
		return false;
	}
	m_HelpImage->addChild( m_HelpClose );
	m_HelpClose->setPosition( Vec2(409,579) );
	m_HelpClose->addTouchEventListener( CC_CALLBACK_2( UpgradeLayer::HelpCloseTouchEvent, this ) );

	m_HelpBackImage->setVisible(false);
	m_HelpImage->setVisible(false);

	/*---------------------------------------------------------------------*/
	// 扫光特效添加(一)
	Layout* m_MaxAll_EffectBack = static_cast<Layout*>(Helper::seekWidgetByName(m_MaxAll,"Panel_saoguanbj"));
	ImageView* m_MaxAll_Effect = static_cast<ImageView*>(Helper::seekWidgetByName(m_MaxAll,"Image_saoguang"));
	m_MaxAll_Effect->setVisible(false);

	int _BackWidth = m_MaxAll_EffectBack->getContentSize().width;
	int _ImageWidth = m_MaxAll_Effect->getContentSize().width;

	ActionInstant*	_Show	= Show::create();
	ActionInterval* _MoveBy = MoveBy::create( 1.5f, Vec2( _BackWidth + _ImageWidth, 0 ) );
	ActionInstant*	_Hide	= Hide::create();
	ActionInterval* _MoveTo = MoveBy::create( 0.01, Vec2( -(_BackWidth + _ImageWidth),0 ) );
	ActionInterval* _MaxAll_Action = Sequence::create( _Show, _MoveBy, _Hide, _MoveTo, nullptr );
	Action* _maxForever = RepeatForever::create(_MaxAll_Action);
	m_MaxAll_Effect->runAction( _maxForever );

	// 扫光特效添加(二)
	Layout* m_Full_EffectBack = static_cast<Layout*>(Helper::seekWidgetByName(m_Max,"Panel_saoguangbj"));
	ImageView* m_Full_Effect = static_cast<ImageView*>(Helper::seekWidgetByName(m_Max,"Image_saoguang"));
	m_Full_Effect->setVisible(false);

	int _FullBackWidth = m_Full_EffectBack->getContentSize().width;
	int _FullImageWidth = m_Full_Effect->getContentSize().width;

	ActionInstant*	_Full_Show	= Show::create();
	ActionInterval* _Full_MoveBy = MoveBy::create( 1.2f, Vec2( _FullBackWidth + _FullImageWidth, 0 ) );
	ActionInstant*	_Full_Hide	= Hide::create();
	ActionInterval* _Full_MoveTo = MoveBy::create( 0.01, Vec2( -(_FullBackWidth + _FullImageWidth), 0 ) );
	ActionInterval* _Full_Action = Sequence::create( _Full_Show, _Full_MoveBy, _Full_Hide, _Full_MoveTo, nullptr );
	Action* _fullForever = RepeatForever::create(_Full_Action);
	m_Full_Effect->runAction( _fullForever );


	return true;
}

void	UpgradeLayer::InitData()
{
	//初始化强化等级
	SetDiamondLevel(DBManager().GetData(DATA_DIAMOND_LEVEL));
	SetWingLevel(DBManager().GetData(DATA_WING_LEVEL));
	SetFireLevel(DBManager().GetData(DATA_FIRE_LEVEL));
	SetCrazyLevel(DBManager().GetData(DATA_CRAZY_LEVEL));
	SetShieldLevel(DBManager().GetData(DATA_SHIELD_LEVEL));
	SetBombLevel(DBManager().GetData(DATA_BOMB_LEVEL));

	//设置宝石数量
	m_Diamond->setString(StringUtils::toString(DBManager().GetData(DATA_DIAMOND_NUM)));

	//设置选中状态
	SelectCheckBox(m_nSelectedType);


	switch (m_nSelectedType)
	{
	case DATA_DIAMOND_LEVEL:
		m_DiamonCB->setSelectedState(true);
		break;
	case DATA_WING_LEVEL:
		m_WingCB->setSelectedState(true);
		break;
	case DATA_FIRE_LEVEL:
		m_FireCB->setSelectedState(true);
		break;
	case DATA_CRAZY_LEVEL:
		m_EruptCB->setSelectedState(true);
		break;
	case DATA_SHIELD_LEVEL:
		m_ShieldCB->setSelectedState(true);
		break;
	case DATA_BOMB_LEVEL:
		m_SkillCB->setSelectedState(true);
		break;
	default:
		break;
	}
	

	/*---------------------------------------------------------------------*/
	// 新手教学相关
	if ( DBManager().GetData( DATA_FRESH_MAN ) < 1 )
	{
		if ( NiceGirl::GetFreshManCurrStep() ==  15 || NiceGirl::GetFreshManCurrStep() ==  16 )
		{
			NiceGirl::SetFreshManCurrStep( 17 );

			dynamic_cast<MainScene*>( this->getParent() )->ShowNewPlayer();
		}
	}

}

void	UpgradeLayer::UpgradeEvent(Ref* pSender, Widget::TouchEventType type)
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
			// 新手教学相关
			if ( DBManager().GetData( DATA_FRESH_MAN ) < 1 )
			{
				if ( NiceGirl::GetFreshManCurrStep() ==  18 )
				{
					NiceGirl::SetFreshManCurrStep( 19 );

					dynamic_cast<MainScene*>(this->getParent())->ShowNewPlayer();


					if ( DBManager().GetData( DATA_WING_LEVEL ) > 0  )
					{
						return ;
					}
				}

			}

			SoundSystem().playEffect(SoundName::Mic_sure);
			const int nCost = std::atoi(m_UpgradeCost->getString().c_str());
			const int nOwne = DBManager().GetData(DATA_DIAMOND_NUM);
			if (nCost > 0 && nCost <= nOwne)
			{
				DBManager().SetData(DATA_DIAMOND_NUM,nOwne - nCost);
				DBManager().AddData(m_nSelectedType,1);
				if (m_nSelectedType == DATA_SHIELD_LEVEL)
				{
					DBManager().AddData(DATA_SHIELD_NUM,1);
				}
				
				SelectCheckBox(m_nSelectedType);
				DBManager().SaveDB();
				InitData();
				TaskMgr().DoneTaskOption(17);


			}else
			{
				// 宝石不够，直接买
				dynamic_cast<MainScene*>( this->getParent() )->ShowOverflowBag( BuyType::Buy_Diamond );
			}
		}
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void	UpgradeLayer::MaxEvent(Ref* pSender, Widget::TouchEventType type)
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
			BuyType _eType = BuyType::Buy_Null;
			switch (m_nSelectedType)
			{
			case DATA_DIAMOND_LEVEL:
				_eType = BuyType::Buy_PartDiamond;
				break;
			case DATA_WING_LEVEL:
				_eType = BuyType::Buy_PartWing;
				break;
			case DATA_FIRE_LEVEL:
				_eType = BuyType::Buy_PartFire;
				break;
			case DATA_CRAZY_LEVEL:
				_eType = BuyType::Buy_PartErupt;
				break;
			case DATA_SHIELD_LEVEL:
				_eType = BuyType::Buy_PartShield;
				break;
			case DATA_BOMB_LEVEL:
				_eType = BuyType::Buy_PartSkill;
				break;
			default:
				return ;
				break;
			}

			// 购买礼包
			dynamic_cast<MainScene*>( this->getParent() )->ShowBuyBoxLayer( _eType );

		}
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void	UpgradeLayer::MaxAllEvent(Ref* pSender, Widget::TouchEventType type)
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
			dynamic_cast<MainScene*>( this->getParent() )->ShowBuyBoxLayer( BuyType::Buy_AllPartUp);
		}
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void	UpgradeLayer::BackEvent(Ref* pSender, Widget::TouchEventType type)
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
			static_cast<MainScene*>(this->getParent())->HideUpgradeLayer();
		}
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void	UpgradeLayer::HelpEvent(Ref* pSender, Widget::TouchEventType type)
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
			
			m_HelpBackImage->setVisible(true);
			m_HelpImage->setVisible(true);

		}
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void	UpgradeLayer::BuyEvent(Ref* pSender, Widget::TouchEventType type)
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
			dynamic_cast<MainScene*>( this->getParent() )->ShowOverflowBag( BuyType::Buy_Diamond );
		}
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void	UpgradeLayer::DiamondEvent(Ref* pSender, Widget::TouchEventType type)
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
			SelectCheckBox(DATA_DIAMOND_LEVEL);
		}
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void	UpgradeLayer::WingEvent(Ref* pSender, Widget::TouchEventType type)
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
			SelectCheckBox(DATA_WING_LEVEL);


			/*---------------------------------------------------------------------*/
			// 新手教学相关
			if ( DBManager().GetData( DATA_FRESH_MAN ) < 1 )
			{
				if ( NiceGirl::GetFreshManCurrStep() ==  17 )
				{
					NiceGirl::SetFreshManCurrStep( 18 );

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

void	UpgradeLayer::FireEvent(Ref* pSender, Widget::TouchEventType type)
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
			SelectCheckBox(DATA_FIRE_LEVEL);
		}
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void	UpgradeLayer::EruptEvent(Ref* pSender, Widget::TouchEventType type)
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
			SelectCheckBox(DATA_CRAZY_LEVEL);
		}
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void	UpgradeLayer::ShieldEvent(Ref* pSender, Widget::TouchEventType type)
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
			SelectCheckBox(DATA_SHIELD_LEVEL);
		}
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void	UpgradeLayer::SkillEvent(Ref* pSender, Widget::TouchEventType type)
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
			SelectCheckBox(DATA_BOMB_LEVEL);
		}
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void	UpgradeLayer::SelectCheckBox(int Index)
{
	if (Index < DATA_DIAMOND_LEVEL || Index > DATA_BOMB_LEVEL)
	{
		return;
	}

	m_nSelectedType = Index;

	for (int i=0; i<6; i++)
	{
		m_CheckBox[i]->setSelectedState(false);				
	}
	
	int level = DBManager().GetData(Index); //强化等级

	if (level < 5) //MAX=5
	{
		//索引ID
		const int line = (Index)*5 + level;

		DBC_DEFINEHANDLE(DBC,DBC_UPGRADE);
		auto lineData = (const _DBC_UPGRADE*)DBC->Search_LineNum_EQU(line);

		m_UpgradeCost->setString(StringUtils::toString(lineData->nCost));
		m_Describe->setString(lineData->szDescribe);
	}
	else
	{
		m_UpgradeCost->setString("0");
		m_Describe->setString("MAX LEVEL");
	}
	
}


void	UpgradeLayer::SetDiamond(int diamond)
{
	m_Diamond->setString(StringUtils::toString(diamond));
}

void	UpgradeLayer::SetUpgradeCost(int cost)
{
	m_UpgradeCost->setString(StringUtils::toString(cost));
}

void	UpgradeLayer::SetDiscribe(std::string& text)
{
	m_Describe->setString(text);
}


void	UpgradeLayer::SetDiamondLevel(int level)
{
	for (int i = 1; i <= 5; i++)
	{
		if (i <= level)
		{
			m_DiamondLevel[i-1]->setVisible(true);
		}
		else
		{
			m_DiamondLevel[i-1]->setVisible(false);
		}
	}
}
void	UpgradeLayer::SetWingLevel(int level)
{
	for (int i = 1; i <= 5; i++)
	{
		if (i <= level)
		{
			m_WingLevel[i-1]->setVisible(true);
		}
		else
		{
			m_WingLevel[i-1]->setVisible(false);
		}
	}
}
void	UpgradeLayer::SetFireLevel(int level)
{
	for (int i = 1; i <= 5; i++)
	{
		if (i <= level)
		{
			m_FireLevel[i-1]->setVisible(true);
		}
		else
		{
			m_FireLevel[i-1]->setVisible(false);
		}
	}
}
void	UpgradeLayer::SetCrazyLevel(int level)
{
	for (int i = 1; i <= 5; i++)
	{
		if (i <= level)
		{
			m_EruptLevel[i-1]->setVisible(true);
		}
		else
		{
			m_EruptLevel[i-1]->setVisible(false);
		}
	}
}
void	UpgradeLayer::SetShieldLevel(int level)
{
	for (int i = 1; i <= 5; i++)
	{
		if (i <= level)
		{
			m_ShieldLevel[i-1]->setVisible(true);
		}
		else
		{
			m_ShieldLevel[i-1]->setVisible(false);
		}
	}
}
void	UpgradeLayer::SetBombLevel(int level)
{
	for (int i = 1; i <= 5; i++)
	{
		if (i <= level)
		{
			m_SkillLevel[i-1]->setVisible(true);
		}
		else
		{
			m_SkillLevel[i-1]->setVisible(false);
		}
	}
}


int		UpgradeLayer::GetSelectedType()
{
	return m_nSelectedType;
}

void UpgradeLayer::HelpCloseTouchEvent(Ref* pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
			m_HelpBackImage->setVisible(false);
			m_HelpImage->setVisible(false);
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}