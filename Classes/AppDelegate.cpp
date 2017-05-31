#include "AppDelegate.h"
#include "GameDefine.h"
#include "ObjectEx/LXObjectManager.h"
#include "SimpleAudioEngine.h"
#include "Common/LXUtils.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)	
#include <stdarg.h>
#include <sys/stat.h>
#include<unistd.h>
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)	
#include "io.h"
#include "direct.h"
#endif

USING_NS_CC;
using namespace CocosDenshion;

static cocos2d::Size designResolutionSize = cocos2d::Size(480, 800);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate() {
	m_pMainScene = nullptr;
}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();

	/*#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)*/
	// DATA文件初始化
//	std::string dataFilePath = CCFileUtils::sharedFileUtils()->getWritablePath() + "data.lx";
//	log("path:==%s",dataFilePath.c_str());
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)	
//	if (access(dataFilePath.c_str(), 0) != 0)
//	{
//		Utils().copyDataFile("data.lx", dataFilePath.c_str());
//		log("path:==%s", dataFilePath.c_str());
//	}
//#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
//	if (_access(dataFilePath.c_str(), 0) != 0)
//	{
//		Utils().copyDataFile("data.lx", dataFilePath.c_str());
//		log("path:==%s", dataFilePath.c_str());
//	}
//#endif

	/*const zp::String dataPath(dataFilePath);
	PackManager().open(dataPath, true);*/
	/*#endif*/

    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("PiLiZhanJi", Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("PiLiZhanJi");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    //director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

	//动态设置涉及分辨率
	const auto screenSize = glview->getFrameSize();
	const int screenRatio = screenSize.height * 10 / screenSize.width;
	//log("screenSize--------width:%f----height:%f", screenSize.width, screenSize.height);
	//if (screenRatio == 13) //1.333
	//{

	//	glview->setDesignResolutionSize(600, 800, ResolutionPolicy::SHOW_ALL);
	//}
	//else if (screenRatio == 15) //1.5
	//{
	//	glview->setDesignResolutionSize(533, 800, ResolutionPolicy::SHOW_ALL);
	//}
	//else if (screenRatio == 16) //1.6
	//{
	//	glview->setDesignResolutionSize(480, 800, ResolutionPolicy::SHOW_ALL);
	//}
	//else if (screenRatio == 17) //1.7
	//{
	//	glview->setDesignResolutionSize(480, 854, ResolutionPolicy::SHOW_ALL);
	//}
	//else
	//{
	//	glview->setDesignResolutionSize(480, 800, ResolutionPolicy::SHOW_ALL);
	//}

    // Set the design resolution
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::SHOW_ALL);
    //Size frameSize = glview->getFrameSize();
    //// if the frame's height is larger than the height of medium size.
    //if (frameSize.height > mediumResolutionSize.height)
    //{        
    //    director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
    //}
    //// if the frame's height is larger than the height of small size.
    //else if (frameSize.height > smallResolutionSize.height)
    //{        
    //    director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
    //}
    //// if the frame's height is smaller than the height of medium size.
    //else
    //{        
    //    director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
    //}

    register_all_packages();

	// set resource file path
	std::vector<std::string> searchPaths;
	searchPaths.push_back("data");
	//searchPaths.push_back("data/Texture/Res");
	CCFileUtils::getInstance()->setSearchPaths(searchPaths);

    // create a scene. it's an autorelease object
	director->setProjection(kCCDirectorProjection2D);
	auto scene = Scene::create();
	m_pMainScene = MainScene::create();
	scene->addChild(m_pMainScene, 0, "mainSence");

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
     SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
     SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
