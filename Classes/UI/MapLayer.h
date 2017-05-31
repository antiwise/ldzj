#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

class MapLayer : public cocos2d::Layer
{
	MapLayer();
	~MapLayer();

public:

	CREATE_FUNC(MapLayer);

	virtual bool init();

	//设置地图编号
	bool	SetMapId(int id);//0~5

	// 初始化
	void	Init();
	
	// 游戏循环
	void Tick(float dt);

	// 暂停移动
	void Pause() { m_bPause = true;};
	
	// 继续移动
	void Continue() { m_bPause = false;};
	

private:	

	//读取地图装饰层布局文件
	bool LoadMapJson(const std::string& fileName);

	Sprite*					m_pBackgroundSpritr_1;		// 背景1
	Sprite*					m_pBackgroundSpritr_2;		// 背景2

	Sprite*					m_pFogSprite1;				// 雾气层
	Sprite*					m_pFogSprite2;				// 雾气层

	Sprite*					m_pClouds1;					// 云层
	Sprite*					m_pClouds2;					// 云层

	int						m_nDecorateNum;				// 装饰数量
	Sprite*					DecorateList[10];			// 装饰层

	
	int			m_nMapID;		//地图编号
	bool		m_bPause;		//暂停移动

};
