#include "MapLayer.h"
#include "Common/FileUtils.h"
#include "GameDefine.h"
#include "Common/LXUtils.h"

USING_NS_CC;
using namespace cocostudio;

MapLayer::MapLayer():Layer()
{
	m_pBackgroundSpritr_1 = nullptr;		// 背景1
	m_pBackgroundSpritr_2 = nullptr;		// 背景2
	m_pFogSprite1 = nullptr;				// 雾
	m_pFogSprite2 = nullptr;				// 雾
	m_pClouds1 = nullptr;					// 云层
	m_pClouds2 = nullptr;					// 云层
	m_bPause = false;

	m_nMapID = 0;

	for (int i = 0; i < 10; i++)
	{
		DecorateList[i] = nullptr;
	}

	m_nDecorateNum = 0;
}

MapLayer::~MapLayer()
{

}

bool MapLayer::init()
{
	// 1. super init first
	if ( !Layer::init() )
	{
		return false;
	}

	//SetMapId(5);


	return true;

}

void MapLayer::Tick(float dt)
{
	if (m_bPause)
	{
		return;
	}

	//背景循环
	m_pBackgroundSpritr_1->setPositionY(m_pBackgroundSpritr_1->getPositionY()-0.5);
	if ( m_pBackgroundSpritr_1->getPositionY() == 0 )
	{
		m_pBackgroundSpritr_2->setPositionY(m_pBackgroundSpritr_1->getContentSize().height);

	}

	m_pBackgroundSpritr_2->setPositionY(m_pBackgroundSpritr_2->getPositionY()-0.5);

	if ( m_pBackgroundSpritr_2->getPositionY() == 0 )
	{
		m_pBackgroundSpritr_1->setPositionY(m_pBackgroundSpritr_2->getContentSize().height);

	}

	//雾气循环
	if (m_pFogSprite1 && m_pFogSprite2)
	{
		m_pFogSprite1->setPositionY(m_pFogSprite1->getPositionY()-1);
		if ( m_pFogSprite1->getPositionY() == 0 )
		{
			m_pFogSprite2->setPositionY(m_pFogSprite1->getContentSize().height);

		}

		m_pFogSprite2->setPositionY(m_pFogSprite2->getPositionY()-1);

		if ( m_pFogSprite2->getPositionY() == 0 )
		{
			m_pFogSprite1->setPositionY(m_pFogSprite2->getContentSize().height);

		}
	}
	

	//云层循环
	if (m_pClouds1 && m_pClouds2)
	{
		m_pClouds1->setPositionY(m_pClouds1->getPositionY()-1);
		if ( m_pClouds1->getPositionY() == 0 )
		{
			m_pClouds2->setPositionY(m_pClouds1->getContentSize().height);

		}

		m_pClouds2->setPositionY(m_pClouds2->getPositionY()-1);

		if ( m_pClouds2->getPositionY() == 0 )
		{
			m_pClouds1->setPositionY(m_pClouds2->getContentSize().height);

		}
	}


	//装饰精灵
	for (int i = 0; i < m_nDecorateNum; i++)
	{
		if (DecorateList[i])
		{
			if (DecorateList[i]->getPositionY()<=-854)
			{
				DecorateList[i]->setPositionY(1708);
			}
			else
			{
				DecorateList[i]->setPositionY(DecorateList[i]->getPositionY()-2);
			}
		}
	}
}

bool MapLayer::SetMapId(int id)
{
	if (id<0 || id>5)
	{
		return false;
	}
	m_nMapID = id;
// 	if (id == 1)
// 	{
// 		m_nMapID = 3;
// 	}
	log("map====%d",m_nMapID);
	//
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//设置背景精灵
// #ifdef MINIPACK
// 	std::string fullPathMapBg = "bg" + StringUtils::toString(m_nMapID) + "/bg.jpg";
// #else
	std::string fullPathMapBg = "bg" + StringUtils::toString(m_nMapID) + "bg.png";
//#endif
	//std::string fullPathMapBg = "bg" + StringUtils::toString(m_nMapID) + "/bg.png";
	

	LXFileUtils::GetMapBgFullPath(fullPathMapBg);
	//log("BackGround 1  START====%s",fullPathMapBg.c_str());
	m_pBackgroundSpritr_1 = Sprite::create(fullPathMapBg);
	m_pBackgroundSpritr_1->setAnchorPoint(Vec2(0.5,0));
	m_pBackgroundSpritr_1->setPosition(Vec2( visibleSize.width/2 + origin.x ,0));
	this->addChild(m_pBackgroundSpritr_1);
	/*log("BackGround 1  END====");
	log("BackGround 2  START====");*/
	m_pBackgroundSpritr_2 = Sprite::create(fullPathMapBg);
	m_pBackgroundSpritr_2->setAnchorPoint(Vec2(0.5,0));
	m_pBackgroundSpritr_2->setPosition(Vec2( visibleSize.width/2+ origin.x ,m_pBackgroundSpritr_1->getContentSize().height -2 ));
	this->addChild(m_pBackgroundSpritr_2);
	log("BackGround 2  END====");
	//设置雾气精灵
	std::string fullPathFog =  "bg" + StringUtils::toString(m_nMapID) + "fog.png";
	LXFileUtils::GetMapBgFullPath(fullPathFog);
	m_pFogSprite1 = Sprite::create(fullPathFog);
	if (m_pFogSprite1)
	{
		m_pFogSprite1->setAnchorPoint(Vec2(0.5,0));
		m_pFogSprite1->setPosition(Vec2( visibleSize.width/2 + origin.x ,0));
		this->addChild(m_pFogSprite1);
	}
	
	m_pFogSprite2 = Sprite::create(fullPathFog);
	if (m_pFogSprite2)
	{
		m_pFogSprite2->setAnchorPoint(Vec2(0.5,0));
		m_pFogSprite2->setPosition(Vec2( visibleSize.width/2+ origin.x ,m_pFogSprite1->getContentSize().height -2 ));
		this->addChild(m_pFogSprite2);
	}
	
	
	//加载装饰层json
	std::string fullPathMapdec = StringUtils::toString(m_nMapID) + ".json";
	LXFileUtils::GetMapFullPath(fullPathMapdec);
	LoadMapJson(fullPathMapdec);

	//设置云层精灵
	std::string fullPathCloud =  "bg" + StringUtils::toString(m_nMapID) + "clouds.png";
	LXFileUtils::GetMapBgFullPath(fullPathCloud);
	m_pClouds1 = Sprite::create(fullPathCloud);
	if (m_pClouds1)
	{
		m_pClouds1->setAnchorPoint(Vec2(0.5,0));
		m_pClouds1->setPosition(Vec2( visibleSize.width/2 + origin.x ,0));
		this->addChild(m_pClouds1);
	}
	
	m_pClouds2 = Sprite::create(fullPathCloud);
	if (m_pClouds2)
	{
		m_pClouds2->setAnchorPoint(Vec2(0.5,0));
		m_pClouds2->setPosition(Vec2( visibleSize.width/2+ origin.x ,m_pClouds1->getContentSize().height -2 ));
		this->addChild(m_pClouds2);
	}
	


	return true;
}

bool MapLayer::LoadMapJson(const std::string& fileName)
{
	bool bRet = false;
	rapidjson::Document _doc;
	do {

		//读取json文件
		std::string contentStr = FileUtils::getInstance()->getStringFromFile(fileName);
		_doc.Parse<0>(contentStr.c_str());
		CC_BREAK_IF(_doc.HasParseError());

		// 解析内容
		std::string strName = DICTOOL->getStringValue_json(_doc, "name");
		int	nId = DICTOOL->getIntValue_json(_doc,"id");
		std::string strDescription = DICTOOL->getStringValue_json(_doc,"description");

		//解析子json

		const rapidjson::Value &dataDict = DICTOOL->getSubDictionary_json(_doc, "data");
		const int dataSize = dataDict.Size();
		for (int j = 0; j < dataSize; j++)
		{
			//解析地图装饰层位置信息
			const rapidjson::Value &subDataDict = DICTOOL->getSubDictionary_json(dataDict, j);
			if (!DICTOOL->checkObjectExist_json(dataDict))
			{
				break;
			}

			//装饰层图块
			std::string strExDat = DICTOOL->getStringValue_json(subDataDict, "exDat");		//其他数据(未使用)
			int	strId		= DICTOOL->getIntValue_json(subDataDict, "id");					//图片资源
			int	rotation	= DICTOOL->getIntValue_json(subDataDict,"rotation");			//旋转角度(未使用)
			int	transMode	= DICTOOL->getIntValue_json(subDataDict,"transMode");			//是否反转
			float	x		= DICTOOL->getIntValue_json(subDataDict,"x");					//X轴位置
			float	y		= DICTOOL->getIntValue_json(subDataDict,"y");					//Y轴位置

			//添加精灵
			std::string fullPathDecorate = "dec" + StringUtils::toString(m_nMapID) + StringUtils::toString(strId) + ".png";
			LXFileUtils::GetMapBgFullPath(fullPathDecorate);
			Sprite* pSpriteDecorate = Sprite::create(fullPathDecorate);
			//pSpriteDecorate->setAnchorPoint( Vec2::ANCHOR_MIDDLE);
			pSpriteDecorate->setAnchorPoint( Vec2::ZERO);

			pSpriteDecorate->setFlippedX(transMode);

			////坐标转换: 坐标系转换DesignHight-y   锚点(0,0)相反点相差一个高度 -height   锚点从(0,0)->(0.5,0.5) +height/2
			////居中处理: 向左平移x向左平移20像素  ()520-DesignWidth)/2
			//pSpriteDecorate->setPosition(Vec2(x+pSpriteDecorate->getContentSize().width/2 - 20, DesignHight-y - pSpriteDecorate->getContentSize().height/2));

			pSpriteDecorate->setPosition(Vec2(x * Utils().GetXRate(),y));
			pSpriteDecorate->setScale(Utils().GetXRate());
			//log("------------------------------------------------------fullPathDecorate:%s",fullPathDecorate.c_str());
			//log("------------------------------------------------------x:%f",x);
			//log("------------------------------------------------------x:%f",x);
			//log("------------------------------------------------------posRatio:%f",posRatio);

			/*BlendFunc blf = {GL_ONE,GL_ONE};
			pSpriteDecorate->setBlendFunc(blf);*/

			this->addChild(pSpriteDecorate);
			DecorateList[m_nDecorateNum] = pSpriteDecorate;
			m_nDecorateNum++;
		}

		bRet = true;
	} while (0);
	return bRet;


}