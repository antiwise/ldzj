#include "TaskTips.h"
#include "Common/FileUtils.h"
#include "Common/LXUtils.h"

TaskTips::TaskTips(void):Layer()
{
	m_MoveLength = 0.0f;
	m_ShowLable = nullptr;
}


TaskTips::~TaskTips(void)
{
}

bool TaskTips::init()
{
	// 1. super init first
	if ( !Layer::init() )
	{
		return false;
	}


	Sprite* _Image = Sprite::create("Texture/Res/ui/TextAtlas/tishi.png");
	if ( !_Image )
	{
		return false;
	}

	m_MoveLength = _Image->getContentSize().height;

	this->addChild( _Image );


	m_ShowLable = Label::create("","Consolas",20);
	if (!m_ShowLable)
	{
		return false;
	}
	m_ShowLable->setColor( Color3B( 255,210,54 ));

	this->addChild( m_ShowLable );

	this->setPositionX( Utils().GetDesignWidth()/2 );
	this->setPositionY( Utils().GetDesignHeight() + _Image->getContentSize().height/2 );

	return true;
}

void TaskTips::Init(std::string str)
{
	if ( this->isVisible() )
	{
		m_ShowLable->setString(str);

	}else
	{
		m_ShowLable->setString(str);
		this->setVisible(true);

		ActionInterval* _moveDown	= MoveBy::create( 0.8f, Vec2( 0,-m_MoveLength ) );
		ActionInterval* _delayTime	= DelayTime::create( 0.5f );
		ActionInterval* _moveUp		= MoveBy::create( 0.75, Vec2( 0,m_MoveLength  ) );
		ActionInstant*	_Hide		= Hide::create();

		Action* _Action = Sequence::create( _moveDown, _delayTime, _moveUp, _Hide, nullptr );

		this->runAction( _Action );
	}
	
}
