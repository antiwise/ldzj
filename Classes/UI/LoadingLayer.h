#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

class LoadingLayer : public cocos2d::Layer
{
public:

	LoadingLayer();
	~LoadingLayer();

    virtual bool init();

	void setProgressInfo(std::string value) { m_Progress->setString(value); }
    
	CREATE_FUNC(LoadingLayer);

private:
	TextAtlas*	m_Progress;
};
