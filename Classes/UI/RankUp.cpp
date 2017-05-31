#include "RankUp.h"
#include "Common/FileUtils.h"
#include "Common/LXUtils.h"
#include "DB/DBManager.h"
#include "SetName.h"
#include "Scene/GameScene.h"
#include "Action/CCActionIntervalEx.h"
#include "RankingLayer.h"
#include "Effect/LXEffectManager.h"
#include "DataBase/DataBase.h"

RankUp::RankUp(void):Layer()
{

}

RankUp::~RankUp(void)
{

}

bool RankUp::init()
{
	// 1. super init first
	if ( !Layer::init() )
	{
		return false;
	}

	// 加载排名上升图片
	LXFileUtils::AddSpriteFrameCache("ui/RankUp.plist", "ui/RankUp.png");

	return true;
}

void RankUp::InitRankUpInfo()
{	
	this->removeAllChildren();

	this->setVisible( true );


	const int nWidth = Utils().GetDesignWidth();
	const int nHeight = Utils().GetDesignHeight();

	/*---------------------------------------------------------------------*/
	// 背景
	ImageView* _BackGroundImage = ImageView::create( "RankUp_10.png",Widget::TextureResType::PLIST );
	if (!_BackGroundImage)
	{
		return ;
	}

	_BackGroundImage->setScaleX(  _BackGroundImage->getContentSize().width / Utils().GetDesignWidth() );
	_BackGroundImage->setScaleX(  _BackGroundImage->getContentSize().height / Utils().GetDesignHeight() );
	_BackGroundImage->setTouchEnabled(true);	// 不让点击事件穿透
	_BackGroundImage->setPosition( Vec2( nWidth/2, nHeight/2) );

	this->addChild( _BackGroundImage );


	// 这里进行名字判断
	std::string _MyName = DBManager().GetPlayerName();

	if (_MyName.empty() || _MyName == "Player" )
	{
		// 起名
		SetName* _SetName = SetName::create();
		_SetName->Init(OpenSetNameType_Battle);
		dynamic_cast<GameScene*>( this->getParent()->getParent())->addChild( _SetName, 111 );

		return ;
	}


	std::string strScore = "Score:";

	// 读表
	DBC_DEFINEHANDLE(s_pParamDBC, DBC_PARAM_INFO);
	const _DBC_PARAM_INFO *pParamInfo = (const _DBC_PARAM_INFO*)s_pParamDBC->Search_Index_EQU(68);
	if ( pParamInfo )
	{
		strScore = pParamInfo->getStringValue();
	}

	/*---------------------------------------------------------------------*/
	// 继续按钮
	Button* _GoOnButtton = Button::create( "RankUp_02.png","RankUp_01.png","",Widget::TextureResType::PLIST );
	if (!_GoOnButtton)
	{
		return ;
	}

	_GoOnButtton->addTouchEventListener( CC_CALLBACK_2( RankUp::GoOnTouchEvent, this ) );
	_GoOnButtton->setPosition( Vec2( nWidth/2, -100 ) );
	this->addChild( _GoOnButtton );

	// 动画	
	ActionInterval* _delay_btn	= DelayTime::create( 1.5f );
	ActionInterval* _moveBy_btn = MoveBy::create( 0.3f, Vec2( 0.0f, 205.0f ) ); 
	ActionInterval* _sequ_btn	= Sequence::create( _delay_btn, _moveBy_btn, nullptr );
	
	_GoOnButtton->runAction( _sequ_btn );
	
	/*---------------------------------------------------------------------*/
	// 排名上升图片 + 特效动画
	Node*	_UpTextNode  = Node::create();
	if ( !_UpTextNode )
	{
		return ;
	}

	// 上升文字图片
	ImageView*	_UpImage = ImageView::create( "RankUp_07.png", Widget::TextureResType::PLIST );
	if( !_UpImage )
	{
		return ;
	}

	// 转圈效果
	ImageView*	_TurnImage = ImageView::create( "RankUp_06.png", Widget::TextureResType::PLIST );
	if( !_TurnImage )
	{
		return ;
	}

	// 转圈效果1
	ImageView*	_TurnImage1 = ImageView::create( "RankUp_08.png", Widget::TextureResType::PLIST );
	if( !_TurnImage1 )
	{
		return ;
	}
	// 光晕效果2
	ImageView*	_TurnImage2 = ImageView::create( "RankUp_09.png", Widget::TextureResType::PLIST );
	if( !_TurnImage2 )
	{
		return ;
	}


	int nMaxLength = std::max(  _TurnImage->getContentSize().width, _TurnImage->getContentSize().width );
	_UpTextNode->setAnchorPoint( Vec2(0.5, 0.5 ) );
	_UpTextNode->setPosition( nWidth/2, nHeight/2 );	
	_UpTextNode->setContentSize( Size( nMaxLength, nMaxLength ) );
	_UpTextNode->setScale( 0.0f );

	_UpImage->setPosition( Vec2( nMaxLength/2, nMaxLength/2 ) );

	_TurnImage->setPosition( Vec2( nMaxLength/2, nMaxLength/2 ) );
	_TurnImage1->setPosition( Vec2( nMaxLength/2, nMaxLength/2 ) );
	_TurnImage2->setPosition( Vec2( nMaxLength/2, nMaxLength/2 ) );

	// 旋转动画
	ActionInterval* _rotatBy_turnImage = RotateBy::create( 4.0f, 360 );
	ActionInterval* _Forever_turnImage = RepeatForever::create( _rotatBy_turnImage );
	_TurnImage->runAction( _Forever_turnImage );


	ActionInterval* _rotatBy_turnImage1 = RotateBy::create( 7.0f, -360 );
	ActionInterval* _Forever_turnImage1 = RepeatForever::create( _rotatBy_turnImage1 );
	_TurnImage1->runAction( _Forever_turnImage1 );


	ActionInterval* _rotatBy_turnImage2_1 = RotateBy::create( 3.0f, 30 );
	ActionInterval* _rotatBy_turnImage2_2 = RotateBy::create( 3.0f, -30 );
	ActionInterval* _sequ_turnImage2	= Sequence::create( _rotatBy_turnImage2_1, _rotatBy_turnImage2_2, nullptr );
	ActionInterval* _Forever_turnImage2 = RepeatForever::create( _sequ_turnImage2 );
	_TurnImage2->runAction( _Forever_turnImage2 );


	//Action var14 = com.sg.raiden.b.b.r.a("planebigtosmall.ogg");		// 音效

	// _UpTextNode 动画
	ActionInterval* _delay_UpTextNode	= DelayTime::create( 1.5f );
	ActionInterval* _scaleTo_UpTextNode = ScaleTo::create( 0.5f, 1.0f );
	ActionInterval* _sequence_UpTextNode = Sequence::create( _delay_UpTextNode, _scaleTo_UpTextNode, nullptr );
	ActionInterval*	_easeSineIn_UpTextNode	=	EaseSineIn::create( _sequence_UpTextNode );
	_UpTextNode->runAction( _sequence_UpTextNode );

	this->addChild(_UpTextNode);
	_UpTextNode->addChild( _TurnImage1 );
	_UpTextNode->addChild( _TurnImage );
	_UpTextNode->addChild( _TurnImage2,1 );
	_UpTextNode->addChild( _UpImage,1 );


	/*---------------------------------------------------------------------*/
	// 上升图标
	Node* _ToUpNode = Node::create();
	if( !_ToUpNode )
	{
		return ;
	}

	// 向上背景
	ImageView*	_ToUpBackImage = ImageView::create( "RankUp_05.png", Widget::TextureResType::PLIST );
	if( !_ToUpBackImage )
	{
		return ;
	}

	// 向上箭头
	ImageView*	_ToUpTipImage = ImageView::create( "RankUp_03.png", Widget::TextureResType::PLIST );
	if( !_ToUpTipImage )
	{
		return ;
	}

	// 名字
	Text* _myNameText = Text::create( _MyName,"Consolas",20 );
	if ( !_myNameText )
	{
		return ;
	}

	// 分数
	std::stringstream _MyScore;
	_MyScore << strScore << DBManager().GetData( DATA_TOP_SCORE );
	Text* _MyScoreText = Text::create( _MyScore.str(), "Consolas", 18 );
	if (!_MyScoreText)
	{
		return ;
	}

	// 排名
	int _MyRanking = RankingLayer::CalcMyRankingWithMyScore( DBManager().GetData( DATA_TOP_SCORE ) );
	std::string _TextAtlasPath = "Texture/Res/ui/TextAtlas/a25.png";
	TextAtlas* _MyRankingToUp = TextAtlas::create( StringUtils::toString( _MyRanking ), _TextAtlasPath, 22, 25, "0" );
	if (!_MyRankingToUp)
	{
		return ;
	}

	_ToUpNode->setContentSize( _ToUpBackImage->getContentSize() );
	_ToUpNode->setPosition( nWidth/2, nHeight/2 - 100 - _ToUpNode->getContentSize().height/2 );


	_ToUpBackImage->setPosition( Vec2(0,0) );
	_ToUpBackImage->setScaleY(0.9f);
	_ToUpBackImage->setFlippedX(true);

	_ToUpTipImage->setPosition( Vec2(-103, 72 ) );
	_ToUpTipImage->setVisible(false);

	ActionInterval*		_delay_ToUpTipIamge = DelayTime::create(1.0f);
	ActionInstant*		_show_ToUpTipImage = Show::create();
	FiniteTimeAction*	_sequ_ToUpTipImage = Sequence::create( _delay_ToUpTipIamge,_show_ToUpTipImage, nullptr );
	_ToUpTipImage->runAction(_sequ_ToUpTipImage);

	_myNameText->setPosition( Vec2( 70.0f, 0.0F ) );

	_MyScoreText->setPosition( Vec2( 50.0f, -20.0F ) );

	_MyRankingToUp->setPosition( Vec2( -110.0f, 5.0F ) );

	this->addChild( _ToUpNode );
	_ToUpNode->addChild(_ToUpTipImage);
	_ToUpNode->addChild(_ToUpBackImage);
	_ToUpNode->addChild(_myNameText);
	_ToUpNode->addChild(_MyScoreText);
	_ToUpNode->addChild(_MyRankingToUp);

	ActionInterval* _delay_ToUpNode		= DelayTime::create(0.5f);
	ActionInterval* _arcMoveBy_ToUpNode = arcMoveBy::create( 1.0f, 0, -280, -140, -35 ); 
	ActionInterval* _sequ_ToUpNode		= Sequence::create( _delay_ToUpNode, _arcMoveBy_ToUpNode, nullptr );
	ActionInterval*	_easeSineIn_ToUpNode	=	EaseSineIn::create( _sequ_ToUpNode );	
	_ToUpNode->runAction( _easeSineIn_ToUpNode );

	/*---------------------------------------------------------------------*/
	// 向下图标
	Node*	_ToDownNode = Node::create();
	if( !_ToDownNode )
	{
		return ;
	}

	// 向下背景
	ImageView*	_ToDownBackImage = ImageView::create( "RankUp_05.png", Widget::TextureResType::PLIST );
	if( !_ToDownBackImage )
	{
		return ;
	}

	// 向下箭头
	ImageView*	_ToDownTipImage = ImageView::create( "RankUp_04.png", Widget::TextureResType::PLIST );
	if( !_ToDownTipImage )
	{
		return ;
	}

	// 别人名字
	std::string _OtherName = RankingLayer::RandomOtherName();
	Text* _OtherNameText = Text::create( _OtherName,"Consolas",20 );
	if ( !_OtherNameText )
	{
		return ;
	}

	// 别人分数
	std::stringstream _OtherScore;
	_OtherScore << strScore << RankingLayer::CalcScore( _MyRanking + 1 );
	Text* _OtherScoreText = Text::create( _OtherScore.str(), "Consolas", 18 );
	if (!_OtherScoreText)
	{
		return ;
	}

	// 别人排名
	TextAtlas* _OtherRankingToDown = TextAtlas::create( StringUtils::toString( _MyRanking + 1 ),"Texture/Res/ui/TextAtlas/a25.png",22, 25, "0" );
	if (!_OtherRankingToDown)
	{
		return ;
	}

	_ToDownNode->setContentSize( _ToDownBackImage->getContentSize() );
	_ToDownNode->setPosition( nWidth/2, nHeight/2 + 100 + _ToDownNode->getContentSize().height/2 );


	_ToDownBackImage->setPosition( Vec2(0,0) );
	_ToDownBackImage->setScaleY(0.9f);

	_ToDownTipImage->setPosition( Vec2(123, -60 ) );
	_ToDownTipImage->setVisible(false);

	ActionInterval*		_delay_ToDownTipIamge = DelayTime::create(1.0f);
	ActionInstant*		_show_ToDownTipImage = Show::create();
	FiniteTimeAction*	_sequ_ToDownTipImage = Sequence::create( _delay_ToDownTipIamge,_show_ToDownTipImage, nullptr );
	_ToDownTipImage->runAction(_sequ_ToDownTipImage);

	_OtherNameText->setPosition( Vec2( -70.0F, 0.0F ) );

	_OtherScoreText->setPosition( Vec2( -50.0F, -20.0F ) );

	_OtherRankingToDown->setPosition( Vec2(105.0F, 5.0F) );

	this->addChild(_ToDownNode);
	_ToDownNode->addChild(_ToDownTipImage);
	_ToDownNode->addChild(_ToDownBackImage);
	_ToDownNode->addChild(_OtherNameText);
	_ToDownNode->addChild(_OtherScoreText);
	_ToDownNode->addChild(_OtherRankingToDown);

	ActionInterval* _delay_ToDownNode		= DelayTime::create(0.5f);
	ActionInterval* _arcMoveBy_ToDownNode	= arcMoveBy::create( 1.0f, 0, 280, 140, 35 );
	ActionInterval* _sequ_ToDownNode		= Sequence::create( _delay_ToDownNode, _arcMoveBy_ToDownNode, nullptr );
	ActionInterval*	_easeSineIn_ToDownNode	=	EaseSineIn::create( _sequ_ToDownNode );	
	_ToDownNode->runAction( _easeSineIn_ToDownNode );


}

void RankUp::GoOnTouchEvent(Ref* pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
			this->setVisible(false);
			dynamic_cast< GameScene* >( this->getParent()->getParent() )->ShowPassCountInfo();
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}