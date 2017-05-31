#include "NiceGirl.h"
#include "Scene/MainScene.h"
#include "GameDefine.h"
#include "Common/LXUtils.h"
#include "DataBase/DataBase.h"
#include "Sound/LXSoundSystem.h"
#include "DB/DBManager.h"
#include "Common/FileUtils.h"
#include "cocos-ext.h"
#include "../../../cocos2d/cocos/ui/UIScale9Sprite.h"

USING_NS_CC_EXT;

#define ClickTagOf9Sprite	555
#define ClicpNodeTag		556
#define ActionTagOf9Sprite	557

const int startStoryIdx[12] = { 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55 };
const int endStoryIdx[12] = { 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67 };

int NiceGirl::FreshManCurrStep = 0;

NiceGirl::NiceGirl(void):Layer()
{
	m_armatur = nullptr;
	m_ClickImage = nullptr;

	m_IsMissionStoryIng = false;
	m_IsFreshGuidIng = false;		// �Ƿ�����������
 
}


NiceGirl::~NiceGirl(void)
{
	m_armatur = nullptr;
	m_ClickImage = nullptr;
}

bool NiceGirl::init()
{
	if ( ! Layer::init() )
	{
		return false;
	}

	// ��������ͼƬ
	LXFileUtils::AddSpriteFrameCache("ui/xinshou.plist", "ui/xinshou.png");
	LXFileUtils::AddSpriteFrameCache("ui/MessageBox.plist", "ui/MessageBox.png");

	this->setContentSize( Size( Utils().GetDesignWidth() , Utils().GetDesignHeight() ) );

	// ���һ����͸����
	m_ClickImage = ImageView::create( "MessageBox_01.png",Widget::TextureResType::PLIST );
	if (!m_ClickImage)
	{
		return false;
	}

	m_ClickImage->setOpacity(200);
	m_ClickImage->setAnchorPoint( Vec2(0,0) );
	m_ClickImage->setScaleX( Utils().GetDesignWidth()/24 + 100 );
	m_ClickImage->setScaleY(  Utils().GetDesignHeight()/24 +  100 );
	m_ClickImage->setPosition( Vec2( Utils().GetDesignWidth() - 620, -100 ));

	this->addChild( m_ClickImage );


	// ������
	m_BigTalkBack = ImageView::create("xinshou_4.png",Widget::TextureResType::PLIST);
	if ( m_BigTalkBack )
	{
		m_BigTalkBack->setPosition(Vec2( Utils().GetDesignWidth() - 262,130 ));
		this->addChild(m_BigTalkBack);
	}
	
	return true;
}

bool NiceGirl::Init()
{
	// �������鲽��
	SetFreshManCurrStep( DBManager().GetData(DATA_FRESH_STEP) );

	// ����
	DBC_DEFINEHANDLE(s_pParamDBC, DBC_PARAM_INFO);
	const _DBC_PARAM_INFO *pParamInfo = (const _DBC_PARAM_INFO*)s_pParamDBC->Search_Index_EQU(19);

	const _DBC_PARAM_INFO *pParamInfo2 = (const _DBC_PARAM_INFO*)s_pParamDBC->Search_Index_EQU(22);
	if (!( pParamInfo && pParamInfo2 ) )
	{
		return false;
	}

	m_GirlInfo = Text::create( pParamInfo->getStringValue() ,pParamInfo2->getStringValue(), 18 );
	m_GirlInfo->setTextAreaSize( Size(240, 80) );
	m_GirlInfo->setAnchorPoint( Vec2( 0, 1) );
	m_GirlInfo->setColor( Color3B(255,210,54) );
	m_GirlInfo->setPosition( Vec2(  Utils().GetDesignWidth() - 455, 165 ) );
	this->addChild( m_GirlInfo );


	// ���ض�������
	ArmatureDataManager::getInstance()->addArmatureFileInfo("Texture/meinv.ExportJson");
	m_armatur = Armature::create("meinv");
	if ( !m_armatur )
	{
		return false;
	}

	m_armatur->setScale( 0.9 );
	// ��ȡ����
	ArmatureAnimation* _MyAnimation = m_armatur->getAnimation();
	if ( !_MyAnimation  )
	{
		return false;
	}

	// ���Ŷ���
	_MyAnimation->play("Animation1");

	m_armatur->setPosition( Utils().GetDesignWidth() - 290,223 );
	this->addChild(m_armatur);


	/*---------------------------------------------------------------------*/
	// С������
	m_SmallTalkBack = ImageView::create("xinshou_1.png",Widget::TextureResType::PLIST);
	if ( m_SmallTalkBack )
	{
		m_SmallTalkBack->setPosition(Vec2( Utils().GetDesignWidth()/2, Utils().GetDesignHeight() - m_SmallTalkBack->getContentSize().height/2 - 10 ));
		this->addChild(m_SmallTalkBack,1);
	}

	// ��Ůͷ��
	ImageView* _SmallGirlImage = ImageView::create("xinshou_2.png",Widget::TextureResType::PLIST);
	if ( _SmallGirlImage )
	{
		_SmallGirlImage->setScale(0.98);
		_SmallGirlImage->setPosition( Vec2( _SmallGirlImage->getContentSize().width/2 + 10 , m_SmallTalkBack->getContentSize().height / 2 - 4 ) );
		m_SmallTalkBack->addChild(_SmallGirlImage);
	}
	
	// �Ի���
	m_SmallTalkInfo = Text::create( "",pParamInfo2->getStringValue(),18 );
	if ( m_SmallTalkInfo )
	{
		m_SmallTalkInfo->setTextAreaSize( Size(240, 100) );
		m_SmallTalkInfo->setAnchorPoint( Vec2( 0, 1) );
		m_SmallTalkInfo->setColor( Color3B(255,210,54) );
		m_SmallTalkInfo->setPosition( Vec2(  155, 95 ) );
		m_SmallTalkBack->addChild( m_SmallTalkInfo );
	}
	

	return true;
}

void NiceGirl::ShowNiceGirl()
{
	// ����С��ʾ��
	m_SmallTalkBack->setVisible(false);

	m_ClickImage->setOpacity(200);

	OpenListener();
	this->setVisible(true);
}

void NiceGirl::ShowNewPlayerInfo()
{	
	log("ShowNewPlayerInfo==");
	if ( this->isVisible() )
	{
		NewPlayerStep();

	}else
	{
		this->setVisible(true);
		NewPlayerStep();
		OpenListener();
	}

	m_ClickImage->setOpacity(0);

}

void NiceGirl::SetFreshManCurrStep(int nId)
{
	FreshManCurrStep = nId;
}

void NiceGirl::OpenListener()
{
	// ��������
	auto listener = EventListenerTouchOneByOne::create();// ����һ����������(���㴥����  
	listener->setSwallowTouches(true);					 // ��ֹ���´���
	listener->onTouchBegan = CC_CALLBACK_2(NiceGirl::onTouchBegan, this);		// ָ�������Ļص�����  
	listener->onTouchEnded = CC_CALLBACK_2(NiceGirl::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);	// ��listener�����¼�ί���� 
	
}

void NiceGirl::CloseListener()
{
	_eventDispatcher->removeEventListenersForTarget(this);
}

bool NiceGirl::onTouchBegan(Touch *touch, Event *unused_event)
{
	auto tempSprite = this->getChildByTag(ClickTagOf9Sprite);
	if ( tempSprite && touch )
	{
		if ( tempSprite->getBoundingBox().containsPoint( touch->getLocation() )  )
		{
			return false;
		}
	}

	return true;
}

void NiceGirl::onTouchEnded(Touch *touch, Event *unused_event)
{
	SoundSystem().playEffect(SoundName::Mic_sure);
	
	if ( m_IsMissionStoryIng )
	{
		this->setVisible(false);
		CloseListener();

		// ����ǹ��½�����������Ϸ
		dynamic_cast<MainScene*>( this->getParent() )->GameContinue();

		m_IsMissionStoryIng = false;
		
	}
	else if( m_IsFreshGuidIng )
	{
		if ( FreshManCurrStep != 1 && FreshManCurrStep != 3 && FreshManCurrStep != 8 && FreshManCurrStep != 11 && FreshManCurrStep != 16 && FreshManCurrStep != 17 && FreshManCurrStep != 18 )
		{
			FreshManCurrStep += 1;

			NewPlayerStep();
		}

	}else
	{
		this->setVisible(false);
		CloseListener();
	}

}

void NiceGirl::NewPlayerStep()
{
	m_IsFreshGuidIng = true;
	if (DBManager().GetData(DATA_FRESH_MAN) > 1)
	{
		return;
	}
	log("NewPlayerStep  ---");
	// ���ֲ����ж�
	if ( FreshManCurrStep < 1 )
	{
		TalkingWithBattleOut(36);
		DBManager().SetData( DATA_FRESH_STEP, FreshManCurrStep );

	}else if( FreshManCurrStep == 1 )
	{
		// ָ������ؿ�ѡ��ť

		int width = Utils().GetDesignWidth();
		int height = Utils().GetDesignHeight();

		if( width == 480 && height == 854 )
		{
			ClickPos(371, 38 , 8.5, 3, 262, 120 );

		}else if ( width == 533 && height == 800 )
		{
			ClickPos(371, 39 , 9.3, 3, 282, 120 );

		}else if ( width == 600 && height == 800 )
		{
			ClickPos(356, 39 , 9.3, 3, 282, 120 );

		}else
		{
			ClickPos(371, 39 , 8.5, 3, 262, 120 );
		}
		DBManager().SetData( DATA_FRESH_STEP, FreshManCurrStep );

	}else if( FreshManCurrStep == 2 )
	{
		// �ؿ�ѡ���еĶԻ�
		TalkingWithBattleOut(37);

	}else if( FreshManCurrStep == 3 )
	{
		// ָ�� ���� ��ť
		int width = Utils().GetDesignWidth();
		int height = Utils().GetDesignHeight();

		if( width == 480 && height == 854 )
		{
			ClickPos( 412, 48, 5.2, 3, 182, 120 );

		}else if ( width == 533 && height == 800 )
		{
			ClickPos( 410, 55, 5.2, 3, 182, 120 );

		}else if ( width == 600 && height == 800 )
		{
			ClickPos( 391, 55, 5.2, 3, 182, 120 );

		}else
		{
			ClickPos( 412, 48, 5.2, 3, 182, 120 );
		}

	}else if( FreshManCurrStep == 4 )
	{
		log("START CLOSE");
		// �ر�
		CloseMeWithNewPlayer();
		log("CLOSE END");
		log("SETDATA  START");
		DBManager().SetData( DATA_FRESH_STEP, FreshManCurrStep );

	}else if( FreshManCurrStep == 5 && DBManager().GetCurrMission() == 1)
	{
		log("START FIRST Talking");
		// ս�������� ��һ��˵��
		TalkingWhitBattleIn( 38 );
		log("END FIRST Talking");
		// ��ͣ��Ϸ
		dynamic_cast<MainScene*>( this->getParent() )->GamePause();
		//---------------------------------------------------------

	}else if( FreshManCurrStep == 6 )
	{
		// ս�������йر�1

		this->setVisible(false);
		CloseListener();

		// ������Ϸ
		dynamic_cast<MainScene*>( this->getParent() )->GameContinue();
		//---------------------------------------------------------


	}else if( FreshManCurrStep == 7 )
	{
		FreshManCurrStep += 1;

		// ��ͣ��Ϸ
		dynamic_cast<MainScene*>( this->getParent() )->GamePause();
		//---------------------------------------------------------

		// ����һ��
		NewPlayerStep();

		//Director::getInstance()->pause();

	}else if( FreshManCurrStep == 8 )
	{
		// ��ʾʹ�û���
		TalkingWhitBattleIn(39);

		// �������
		int width = Utils().GetDesignWidth();
		int height = Utils().GetDesignHeight();

		if( width == 480 && height == 854 )
		{
			ClickPos( 417, 43, 3.6, 3.6, 146, 136 );

		}else if ( width == 533 && height == 800 )
		{
			ClickPos( 426, 85, 3.6, 3.6, 146, 136 );

		}else if ( width == 600 && height == 800 )
		{
			ClickPos( 405, 85, 3.6, 3.6, 146, 136 );

		}else
		{
			ClickPos( 419, 85, 3.6, 3.6, 146, 136 );
		}
		
		m_SmallTalkBack->setVisible( true );

	}else if( FreshManCurrStep == 9 )
	{
		// ս�������йر�2
		// �ر�
		CloseMeWithNewPlayer();

		// ������Ϸ
		dynamic_cast<MainScene*>( this->getParent() )->GameContinue();
		//---------------------------------------------------------

	}else if( FreshManCurrStep == 10 )
	{
		FreshManCurrStep += 1;

		// ��ͣ��Ϸ
		dynamic_cast<MainScene*>( this->getParent() )->GamePause();
		//---------------------------------------------------------

		// ����һ��
		NewPlayerStep();

	}else if( FreshManCurrStep == 11 )
	{
		// ��ʾʹ��ը��
		TalkingWhitBattleIn(40);

		// ���ը��
		int width = Utils().GetDesignWidth();
		int height = Utils().GetDesignHeight();

		if( width == 480 && height == 854 )
		{
			ClickPos( 55, 43, 3.6, 3.6, 146, 136 );

		}else if ( width == 533 && height == 800 )
		{
			ClickPos( 50, 85, 3.6, 3.6, 146, 136 );

		}else if ( width == 600 && height == 800 )
		{
			ClickPos( 70, 85, 3.6, 3.6, 146, 136 );

		}else
		{
			ClickPos( 56, 85, 3.6, 3.6, 146, 136 );
		}

		m_SmallTalkBack->setVisible( true );

	}else if( FreshManCurrStep == 12 )
	{
		// ս�������йر�3
		// �ر�
		CloseMeWithNewPlayer();

		// ������Ϸ
		dynamic_cast<MainScene*>( this->getParent() )->GameContinue();
		//---------------------------------------------------------



	}else if( FreshManCurrStep == 13 )
	{
		// boss������Ի�

		TalkingWhitBattleIn(41);

		// ��ͣ��Ϸ
		dynamic_cast<MainScene*>( this->getParent() )->GamePause();
		//---------------------------------------------------------
		DBManager().SetData(DATA_FRESH_STEP, FreshManCurrStep);
		DBManager().SetData(DATA_FRESH_MAN, 1);

	}else if( FreshManCurrStep == 14 )
	{
		// ������ر�
		// �رձ�����
		CloseMeWithNewPlayer();
		
		// ������Ϸ
		dynamic_cast<MainScene*>( this->getParent() )->GameContinue();
		//---------------------------------------------------------
		DBManager().SetData(DATA_FRESH_STEP, FreshManCurrStep);
		DBManager().SetData(DATA_FRESH_MAN, 1);
		
	}else if( FreshManCurrStep == 15 )
	{
		// ͳ�ƽ���Ի�
		TalkingWithBattleOut(42);
		
		DBManager().SetData(DATA_FRESH_STEP, FreshManCurrStep);
		DBManager().SetData(DATA_FRESH_MAN, 1);

	}else if( FreshManCurrStep == 16 )
	{
		// ���ս��ǿ��
		int width = Utils().GetDesignWidth();
		int height = Utils().GetDesignHeight();

		if( width == 480 && height == 854 )
		{
			ClickPos(138, 261 , 7.2, 2.5, 230, 108 );

		}else if ( width == 533 && height == 800 )
		{
			ClickPos(154, 239 , 7.2, 2.5, 230, 108 );

		}else if ( width == 600 && height == 800 )
		{
			ClickPos(163, 244 , 7.2, 2.5, 230, 108 );

		}else
		{
			ClickPos(134, 237 , 7.2, 2.5, 230, 108 );
		}
		

		DBManager().SetData(DATA_FRESH_STEP, FreshManCurrStep);
		DBManager().SetData(DATA_FRESH_MAN, 1);

	}else if( FreshManCurrStep == 17 )
	{
		// ѡ���Ż�
		int width = Utils().GetDesignWidth();
		int height = Utils().GetDesignHeight();

		if( width == 480 && height == 854 )
		{
			ClickPos(358, 635, 5, 5, 180, 168 );

		}else if ( width == 533 && height == 800 )
		{
			ClickPos(358, 637, 5, 5, 180, 168 );

		}else if ( width == 600 && height == 800 )
		{
			ClickPos(341, 637, 5, 5, 180, 168 );

		}else
		{
			ClickPos(358, 637, 5, 5, 180, 168 );
		}
		DBManager().SetData(DATA_FRESH_STEP, FreshManCurrStep);
		DBManager().SetData(DATA_FRESH_MAN, 1);

	}else if( FreshManCurrStep == 18 )
	{
		// �������
		int width = Utils().GetDesignWidth();
		int height = Utils().GetDesignHeight();

		if( width == 480 && height == 854 )
		{
			ClickPos(257, 163 , 4.5, 2, 168, 98 );

		}else if ( width == 533 && height == 800 )
		{
			ClickPos(257, 168 , 4.5, 2, 168, 98 );

		}else if ( width == 600 && height == 800 )
		{
			ClickPos(253, 168 , 4.5, 2, 168, 98 );

		}else
		{
			ClickPos(258, 171 , 4.5, 2, 168, 98 );
		}
		DBManager().SetData(DATA_FRESH_STEP, FreshManCurrStep);
		DBManager().SetData(DATA_FRESH_MAN, 1);
		

	}else if( FreshManCurrStep == 19 )
	{
		// ������ɺ�Ի�
		TalkingWithBattleOut(43);
		DBManager().SetData( DATA_FRESH_STEP, FreshManCurrStep + 1 );
		DBManager().SetData( DATA_FRESH_MAN, 1 );

	}else
	{
		DBManager().SetData( DATA_FRESH_STEP, FreshManCurrStep );
		DBManager().SetData( DATA_FRESH_MAN, 1 );

		// �رձ�����
		CloseMeWithNewPlayer();

		// �ر�ǿ������
		dynamic_cast<MainScene*>( this->getParent() )->HideUpgradeLayerWithNewPlayer();
	
	}
}

void NiceGirl::TalkingWithBattleOut(int nTalkId)
{
	//---------------------------------------
	// ���ش󴰿�
	m_ClickImage->setVisible(true);
	m_SmallTalkBack->setVisible(false);
	m_BigTalkBack->setVisible(true);
	m_GirlInfo->setVisible(true);
	m_armatur->setVisible(true);
	//---------------------------------------

	auto temp1 = this->getChildByTag(ClicpNodeTag);
	if ( temp1 )
	{
		removeChild( temp1 );
	}

	auto temp2 = this->getChildByTag(ClickTagOf9Sprite);
	if ( temp2 )
	{
		removeChild( temp2 );
	}
	auto temp3 = this->getChildByTag(ActionTagOf9Sprite);
	if ( temp3 )
	{
		removeChild( temp3 );
	}

	// ����
	DBC_DEFINEHANDLE(s_pParamDBC, DBC_PARAM_INFO);

	// �ؿ�ѡ������еĶԻ�
	const _DBC_PARAM_INFO *pParamInfo = (const _DBC_PARAM_INFO*)s_pParamDBC->Search_Index_EQU( nTalkId );
	if ( pParamInfo )
	{
		std::string str = pParamInfo->getStringValue();
		m_GirlInfo->setString( str );
	}else
	{
		CloseListener();
		this->setVisible(false);
		return ;
	}

}

void NiceGirl::TalkingWhitBattleIn(int nTalkId)
{
	// ���ش󴰿�
	m_ClickImage->setVisible(true);
	m_SmallTalkBack->setVisible(true);
	m_BigTalkBack->setVisible(false);
	m_GirlInfo->setVisible(false);
	m_armatur->setVisible(false);
	//---------------------------------------

	auto temp1 = this->getChildByTag(ClicpNodeTag);
	if ( temp1 )
	{
		removeChild( temp1 );
	}

	auto temp2 = this->getChildByTag(ClickTagOf9Sprite);
	if ( temp2 )
	{
		removeChild( temp2 );
	}
	auto temp3 = this->getChildByTag(ActionTagOf9Sprite);
	if ( temp3 )
	{
		removeChild( temp3 );
	}


	// ����
	DBC_DEFINEHANDLE(s_pParamDBC, DBC_PARAM_INFO);

	// �ؿ�ѡ������еĶԻ�
	const _DBC_PARAM_INFO *pParamInfo = (const _DBC_PARAM_INFO*)s_pParamDBC->Search_Index_EQU( nTalkId );
	if ( pParamInfo )
	{
		std::string str = pParamInfo->getStringValue();
		m_SmallTalkInfo->setString( str );

	}else
	{
		this->setVisible(false);
		CloseListener();
		return ;
	}
}

void NiceGirl::ClickPos( float x, float y, float scaleX, float scaleY , float width ,float hight )
{
	//---------------------------------------
	// ���ش���
	m_ClickImage->setVisible(false);
	m_SmallTalkBack->setVisible(false);
	m_BigTalkBack->setVisible(false);
	m_GirlInfo->setVisible(false);
	m_armatur->setVisible(false);
	//---------------------------------------
	log("STAT  1");
	auto temp1 = this->getChildByTag(ClicpNodeTag);
	if ( temp1 )
	{
		removeChild( temp1 );
	}

	auto temp2 = this->getChildByTag(ClickTagOf9Sprite);
	if ( temp2 )
	{
		removeChild( temp2 );
	}
	auto temp3 = this->getChildByTag(ActionTagOf9Sprite);
	if ( temp3 )
	{
		removeChild( temp3 );
	}

	// ָ������ؿ�ѡ��ť
	auto _clip = ClippingNode::create();
	_clip->setInverted( true );
	_clip->setAlphaThreshold( 0.0f );
	_clip->setTag(ClicpNodeTag);
	this->addChild(_clip);
	
	auto _layerColor = LayerColor::create( Color4B( 0, 0, 0, 200 ) );
	_clip->addChild( _layerColor );
	
	auto _sprite = Sprite::createWithSpriteFrameName("MessageBox_01.png");

	_sprite->setPosition( Utils().GetFixedX( x ), Utils().GetFixedY( y ) );
	_sprite->setScaleX( scaleX );
	_sprite->setScaleY( scaleY );

	//_clip->addChild( _sprite );

	_clip->setStencil( _sprite );
	
	cocos2d::ui::Scale9Sprite* _9Sprite = cocos2d::ui::Scale9Sprite::createWithSpriteFrameName("xinshou_6.png");
	_9Sprite->setAnchorPoint( Vec2(0.5,0.5) );
	_9Sprite->setPosition( Utils().GetFixedX( x + 1.5 ), Utils().GetFixedY( y + 2 ) );
	_9Sprite->setPreferredSize( Size( width-32, hight-22 ) );
	_9Sprite->setTag( ClickTagOf9Sprite );

	this->addChild( _9Sprite );
	
	cocos2d::ui::Scale9Sprite* _Action9Sprite = cocos2d::ui::Scale9Sprite::createWithSpriteFrameName("xinshou_5.png");
	_Action9Sprite->setAnchorPoint( Vec2(0.5,0.5) );
	_Action9Sprite->setPosition( Utils().GetFixedX( x + 1.5 ), Utils().GetFixedY( y + 2 ) );
	_Action9Sprite->setPreferredSize( Size( width-32, hight-22 ) );
	_Action9Sprite->setTag( ActionTagOf9Sprite );
	
	// ��˸����
	ActionInterval* _pFadeIn = FadeIn::create(0.5);
	ActionInterval* _pFadeOut = FadeOut::create(0.5);
	ActionInterval* _Sequence = Sequence::create( _pFadeIn, _pFadeOut, NULL );
	ActionInterval* _Forever = RepeatForever::create( _Sequence );

	_Action9Sprite->runAction( _Forever );

	this->addChild( _Action9Sprite );
	log("END  DONGHUA");
}

void NiceGirl::CloseMeWithNewPlayer()
{
	log("CHU JI START VISIBLE");
	//---------------------------------------
	// ���ش󴰿�
	m_SmallTalkBack->setVisible(false);
	m_BigTalkBack->setVisible(false);
	m_GirlInfo->setVisible(false);
	m_armatur->setVisible(false);
	//---------------------------------------
	log("CHU JI NEW CLOSE ClicpNodeTag");
	// �رձ�����
	auto temp1 = this->getChildByTag(ClicpNodeTag);
	if ( temp1 )
	{
		removeChild( temp1 );
	}
	log("CHU JI NEW CLOSE ClickTagOf9Sprite");
	auto temp2 = this->getChildByTag(ClickTagOf9Sprite);
	if ( temp2 )
	{
		removeChild( temp2 );
	}
	auto temp3 = this->getChildByTag(ActionTagOf9Sprite);
	if ( temp3 )
	{
		removeChild( temp3 );
	}

	CloseListener();
	m_IsFreshGuidIng = false;
	this->setVisible(false);
	log("CHU JI REMOVE ALL END===");
}

void NiceGirl::ShowMissionStartStory(int nMissionID)
{
	// ��ͣ��Ϸ
	dynamic_cast<MainScene*>( this->getParent() )->GamePause();
	
	m_IsMissionStoryIng = true;

	this->setVisible(true);
	OpenListener();

	// ���ش󴰿�
	m_ClickImage->setVisible(false);
	m_SmallTalkBack->setVisible(true);
	m_BigTalkBack->setVisible(false);
	m_GirlInfo->setVisible(false);
	m_armatur->setVisible(false);
	//---------------------------------------

	// ����
	DBC_DEFINEHANDLE(s_pParamDBC, DBC_PARAM_INFO);

	// �ؿ�ѡ������еĶԻ�

	int nIdx = startStoryIdx[nMissionID-1];

	const _DBC_PARAM_INFO *pParamInfo = (const _DBC_PARAM_INFO*)s_pParamDBC->Search_Index_EQU( nIdx );
	if ( pParamInfo )
	{
		std::string str = pParamInfo->getStringValue();
		m_SmallTalkInfo->setString( str );

	}else
	{
		// ������Ϸ
		dynamic_cast<MainScene*>( this->getParent() )->GameContinue();

		this->setVisible(false);
		CloseListener();
		return ;
	}
}

void NiceGirl::ShowMissionEndStory(int nMissionID)
{
	// ��ͣ��Ϸ
	dynamic_cast<MainScene*>( this->getParent() )->GamePause();

	m_IsMissionStoryIng = true;

	this->setVisible(true);
	OpenListener();

	// ���ش󴰿�
	m_ClickImage->setVisible(false);
	m_SmallTalkBack->setVisible(true);
	m_BigTalkBack->setVisible(false);
	m_GirlInfo->setVisible(false);
	m_armatur->setVisible(false);
	//---------------------------------------

	// ����
	DBC_DEFINEHANDLE(s_pParamDBC, DBC_PARAM_INFO);

	// �ؿ�ѡ������еĶԻ�

	int nIdx = endStoryIdx[ nMissionID-1 ];

	const _DBC_PARAM_INFO *pParamInfo = (const _DBC_PARAM_INFO*)s_pParamDBC->Search_Index_EQU( nIdx );
	if ( pParamInfo )
	{
		std::string str = pParamInfo->getStringValue();
		m_SmallTalkInfo->setString( str );

	}else
	{
		// ������Ϸ
		dynamic_cast<MainScene*>( this->getParent() )->GameContinue();

		this->setVisible(false);
		CloseListener();
		return ;
	}
}