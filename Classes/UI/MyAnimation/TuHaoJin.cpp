#include "TuHaoJin.h"
#include "Scene/MainScene.h"
#include "GameDefine.h"
#include "Sound/LXSoundSystem.h"
#include "Common/FileUtils.h"

MainScene* TuHaoJin::m_TempMainScene = nullptr;

TuHaoJin::TuHaoJin(void):Layer()
{
	m_armatur = nullptr;
	m_ClickImage = nullptr;
}


TuHaoJin::~TuHaoJin(void)
{
	m_armatur = nullptr;
	m_ClickImage = nullptr;
}

bool TuHaoJin::init()
{
	if ( ! Layer::init() )
	{
		return false;
	}

	LXFileUtils::AddSpriteFrameCache("ui/MessageBox.plist", "ui/MessageBox.png");

	//this->setContentSize( Size( 115,80 ) );

	// 加载动画数据
	// 	ArmatureDataManager::getInstance()->addArmatureFileInfo("Texture/NewAnimation.ExportJson");
	// 	m_armatur = Armature::create("NewAnimation");
	ArmatureDataManager::getInstance()->addArmatureFileInfo("Texture/jixie.ExportJson");
	m_armatur = Armature::create("jixie");
	if ( !m_armatur )
	{
		return false;
	}

	m_armatur->setScale(0.8);

	// 获取动画
	ArmatureAnimation* _MyAnimation = m_armatur->getAnimation();
	if ( !_MyAnimation  )
	{
		return false;
	}

	// 播放动作
	_MyAnimation->play("jixie");

	this->addChild(m_armatur);

	m_ClickImage = ImageView::create( "MessageBox_0.png",Widget::TextureResType::PLIST );
	if (!m_ClickImage)
	{
		return false;
	}

	m_ClickImage->setAnchorPoint( Vec2(0.55,0.5) );
	m_ClickImage->setScaleX(5);
	m_ClickImage->setScaleY(5);
	m_ClickImage->setTouchEnabled(true);
	m_ClickImage->addTouchEventListener( CC_CALLBACK_2( TuHaoJin::ClickTouchEvent, this ) );

	this->addChild( m_ClickImage );
	
	return true;
}

void TuHaoJin::Init(float scale /* = 0.8f */, float rotate /* = 0.0f */)
{
	if ( m_armatur )
	{
		m_armatur->setScale(scale);
		m_armatur->setRotation(rotate);
	}
}

void TuHaoJin::SetMainSceneNode(MainScene* _MainNode)
{
	if ( _MainNode )
	{
		m_TempMainScene = _MainNode;
	}
}


void TuHaoJin::ClickTouchEvent(Ref* pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
			if (m_TempMainScene)
			{
				SoundSystem().playEffect(SoundName::Mic_sure);
				m_TempMainScene->ShowBuyBoxLayer(BuyType::Buy_TuHaoJin);
			}
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}
