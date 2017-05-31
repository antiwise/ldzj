#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

class ParticleEffect;

class RankUp : public Layer
{
public:
	RankUp(void);
	~RankUp(void);
	
	CREATE_FUNC(RankUp);

	virtual bool init();

public:

	void	InitRankUpInfo();

	// �����ص�
	void	GoOnTouchEvent(Ref* pSender, Widget::TouchEventType type);

private:

};

