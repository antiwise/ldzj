#include "LoadingLayer.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "Common/LXUtils.h"
#include "Common/FileUtils.h"

using namespace ui;
using namespace cocostudio;
USING_NS_CC;

LoadingLayer::LoadingLayer():Layer()
{
}

LoadingLayer::~LoadingLayer()
{

}

// on "init" you need to initialize your instance
bool LoadingLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
	this->scheduleUpdate();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	log("sxxxx   pic star");
	std::string bgname = "ui/TextAtlas/mask.png";
	LXFileUtils::GetTexturePath(bgname);

	Sprite *pBg = Sprite::create(bgname);
	pBg->setPosition(Utils().GetDesignWidth() / 2, Utils().GetDesignHeight()/ 2);
	pBg->setScale(100);
	addChild(pBg);
	

	log("sxxxx   pic end");
	std::string name = "ui/TextAtlas/logo.png";
	LXFileUtils::GetTexturePath(name);
	Sprite *pLogo = Sprite::create(name);
	log("xxxxxxxxxxxxx");
	Size ws = Director::getInstance()->getWinSize();
	pLogo->setPosition(ws.width / 2, ws.height*0.5  + 135);
	pLogo->setScale(0.9);
	addChild(pLogo);

	// 加载动画数据
	ArmatureDataManager::getInstance()->addArmatureFileInfo("Texture/Loading.ExportJson");
	Armature* m_armatur = Armature::create("Loading");
	if ( !m_armatur )
	{
		return false;
	}

	m_armatur->setScale( 0.6 );
	// 获取动画
	ArmatureAnimation* _MyAnimation = m_armatur->getAnimation();
	if ( !_MyAnimation  )
	{
		return false;
	}

	// 播放动作
	_MyAnimation->play("Loading");
	
	m_armatur->setPosition( (Utils().GetDesignWidth() - m_armatur->getContentSize().width) / 2  + 230, 80 );
	addChild(m_armatur);


	// 完成度
	std::string textFileName = "data/Texture/Res/ui/TextAtlas/a24.png";
	m_Progress = TextAtlas::create("0/", textFileName, 10, 16, "/");
	m_Progress->setPosition(Vec2(430, 70));
	addChild(m_Progress);

    return true;
}
