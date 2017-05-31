#include "SetName.h"
#include "Common/FileUtils.h"
#include "DB/DBManager.h"
#include "Sound/LXSoundSystem.h"
#include "Common/LXUtils.h"
#include "Scene/MainScene.h"
#include "Scene/GameScene.h"
#include "RankingLayer.h"
#include "Sound/LXSoundSystem.h"
#include "DataBase/DataBase.h"


SetName::SetName(void):Layer()
{
	m_OpenType = OpenSetNameType_Ranking;
	m_TextField = nullptr;
	m_SetNameBackGround = nullptr;
	m_SpriteWarning = nullptr;
}


SetName::~SetName(void)
{
}

bool SetName::init()
{
	// 1. super init first
	if ( !Layer::init() )
	{
		return false;
	}


	LXFileUtils::AddSpriteFrameCache("ui/setname.plist", "ui/setname.png");
	LXFileUtils::AddSpriteFrameCache("ui/settings.plist", "ui/settings.png");

	int nWidth = Utils().GetDesignWidth();
	int nHeight = Utils().GetDesignHeight();

	ImageView* _bakeground = ImageView::create( "MessageBox_01.png",Widget::TextureResType::PLIST );
	if ( !_bakeground )
	{
		return false;
	}

	_bakeground->setOpacity(200);
	_bakeground->setScaleX( nWidth/24 + 100 );
	_bakeground->setScaleY(  nHeight/24 + 100 );
	_bakeground->setPosition( Vec2( nWidth/2-24, nHeight/2 ) );
	_bakeground->setTouchEnabled(true);

	this->addChild( _bakeground );

	// 命名背景
	m_SetNameBackGround = ImageView::create( "setname_01.png",Widget::TextureResType::PLIST );
	if ( !m_SetNameBackGround )
	{
		return false;
	}

	m_SetNameBackGround->setAnchorPoint( Vec2(0.5f,0.5f) );
	m_SetNameBackGround->setPosition( Vec2( nWidth/2, nHeight/2 ) );

	this->addChild( m_SetNameBackGround );

	// 随机命名按钮
	Button*	_RandName = Button::create("setname_03.png","setname_02.png","",Widget::TextureResType::PLIST);
	if (!_RandName)
	{
		return false;
	}
	_RandName->setAnchorPoint( Vec2( 0.5, 0.5f) );
	_RandName->setPosition( Vec2( 126, 63 ) );	
	_RandName->addTouchEventListener( CC_CALLBACK_2( SetName::RandNaneTouchEvent, this ) );
	m_SetNameBackGround->addChild( _RandName );

	// 确定按钮
	Button*	_SureName = Button::create("setname_05.png","setname_04.png","",Widget::TextureResType::PLIST);
	if (!_SureName)
	{
		return false;
	}
	m_SetNameBackGround->addChild( _SureName );
	_SureName->setPosition( Vec2( 316, 63 ) );
	_SureName->addTouchEventListener( CC_CALLBACK_2( SetName::SureTouchEvent, this ) );

	// 输入框
	m_TextField = TextField::create("","Helvetica",20);
	if ( !m_TextField )
	{
		return false;
	}

	m_SetNameBackGround->addChild( m_TextField );
	m_TextField->ignoreContentAdaptWithSize(false);
	m_TextField->setMaxLengthEnabled(true);
	m_TextField->setMaxLength( 11 );
	m_TextField->setTextAreaSize( Size(394,50));
	m_TextField->setTextHorizontalAlignment(TextHAlignment::CENTER);
	m_TextField->setTextVerticalAlignment(TextVAlignment::CENTER);
	m_TextField->setPosition( Vec2( m_SetNameBackGround->getContentSize().width/2, m_SetNameBackGround->getContentSize().height/2 - 17 ) );
	
	m_TextField->addEventListener( CC_CALLBACK_2( SetName::textFieldEvent, this) );

	// 敏感词提示
	m_SpriteWarning = Sprite::createWithSpriteFrameName("settings_122.png");
	m_SpriteWarning->setAnchorPoint(Vec2(0.5,0.5));
	m_SpriteWarning->setPosition(221,135);
	m_SetNameBackGround->addChild(m_SpriteWarning);
	m_SpriteWarning->setVisible(false);

	return true;
}

void SetName::Init( OpenSetNameType eType )
{
	m_OpenType = eType;
	std::string strName = RankingLayer::RandomMyName();
	m_TextField->setText( strName );
}

void SetName::textFieldEvent(Ref *pSender, TextField::EventType type)
{
	switch (type)
	{
	case TextField::EventType::ATTACH_WITH_IME:
		{
			if (m_TextField)
			{
				m_TextField->setText( "" );
			}

			m_SetNameBackGround->setPositionY( Utils().GetDesignHeight()/2.0f + 100 );

		}
		break;
	case TextField::EventType::DETACH_WITH_IME:
		{
			m_SetNameBackGround->setPositionY( Utils().GetDesignHeight()/2.0f );
		}
		break;
	case TextField::EventType::INSERT_TEXT:
		{

		}
		break;
	case TextField::EventType::DELETE_BACKWARD:
		{

		}
		break;
	default:
		break;
	}
}

void SetName::RandNaneTouchEvent(Ref* pSender, Widget::TouchEventType type)
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
			static std::string str;
			str = RankingLayer::RandomMyName();
			if( ! str.empty() )
			{
				m_TextField->setText( "" );
				m_TextField->setText( str );
			}
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void SetName::SureTouchEvent(Ref* pSender, Widget::TouchEventType type)
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
			if ( m_TextField )
			{
				std::string str = m_TextField->getStringValue();

				if (str.empty() || str == "" || str == "Player" )
				{
					return ;
				}

				//过滤词汇判断
				if (HasForbiddenWords(str))
				{
					//
					m_SpriteWarning->setScale(0.0);
					m_SpriteWarning->setVisible(true);
					ActionInterval* _ScaleToBig = ScaleTo::create(0.2,1);
					ActionInterval* _DelayTime	= DelayTime::create( 0.5 );
					ActionInstant*	_Hide		= Hide::create();
					ActionInterval* _SequenceAction = Sequence::create( _ScaleToBig, _DelayTime, _Hide, nullptr );
					m_SpriteWarning->runAction(_SequenceAction);

					//敏感词提示
					return;
				}

				DBManager().SetPlayerName( str );

				if ( m_OpenType == OpenSetNameType_Ranking )
				{
					dynamic_cast<MainScene*>(this->getParent())->ReLoadRankingInfo();			// 重新加载排行界面

				}else if( m_OpenType == OpenSetNameType_Battle )
				{
					dynamic_cast<GameScene*>(this->getParent())->OpenRankUpLayer();				// 排名上升界面
				}

				this->setVisible(false);
			}
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

bool SetName::HasForbiddenWords(const std::string& strIn)
{
	DBC_DEFINEHANDLE(_FilterData, DBC_TALK_FILTER);
	const int length = _FilterData->GetRecordsNum();
	int count = 0;
	for (int i = 0; i < length; i++)
	{
		const _DBC_TALK_FILTER* pLine = (const _DBC_TALK_FILTER*)_FilterData->Search_LineNum_EQU(i);
		if (pLine)
		{
			//const std::string strFilter = pLine->szString;
			if(std::string::npos != strIn.find(pLine->szString))
			{
				return true;
			}
		}
		count = i;
	}

	return false;

}

/*---------------------------------------------------------------------*/
//测试用
// 
// 
// // void				onDraw(const Mat4 &transform);
// void SetName::draw(Renderer* renderer, const Mat4 &transform, uint32_t flags)
// {
// 	Layer::draw(renderer, transform, flags);
// 	//CustomCommand		m_CustomCommand;			// 自定义绘制
// 	m_CustomCommand.init(_globalZOrder);
// 	m_CustomCommand.func = CC_CALLBACK_0(SetName::onDraw, this, transform);
// 	renderer->addCommand(&m_CustomCommand);
// 
// }

// 
// // virtual void		draw(Renderer* renderer, const Mat4 &transform, uint32_t flags);
// void SetName::onDraw(const Mat4 &transform)
// {

// 	Director* director = Director::getInstance();
// 	Mat4 oldModelView;
// 	oldModelView = director->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
// 	director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, _modelViewTransform);
// 
// 	int x1 = 240 - 221 + m_TextField->getPosition().x - m_TextField->getContentSize().width/2;

// 	int y1 = 400 - 135 + m_TextField->getPosition().y - m_TextField->getContentSize().height/2;

// 	int x2 = 240 - 221 + m_TextField->getPosition().x + m_TextField->getContentSize().width/2;

// 	int y2 = 400 - 135 + m_TextField->getPosition().y + m_TextField->getContentSize().height/2;

// 

// 	CCPoint pt1 = ccp(x1, y1);
// 	CCPoint pt2 = ccp(x2, y1);
// 	CCPoint pt3 = ccp(x2, y2);
// 	CCPoint pt4 = ccp(x1, y2);
// 
// 	Vec2 vertices[4] = { pt1,	pt2,	pt3,	pt4, };
// 
// 	DrawPrimitives::drawPoly(vertices, 4, true);
// 
// 	director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, oldModelView);

// }

