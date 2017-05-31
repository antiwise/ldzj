#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;


class MainScene;

class TuHaoJin :	public Layer
{
public:
	TuHaoJin(void);
	~TuHaoJin(void);

	CREATE_FUNC(TuHaoJin);

	virtual bool init();

	void		Init(float scale = 0.8f, float rotate = 0.0f);

	static void	SetMainSceneNode(MainScene* _MainNode);

	// µã»÷»Øµ÷
	void	ClickTouchEvent(Ref* pSender, Widget::TouchEventType type);

private:
	
	ImageView*	m_ClickImage;

	Armature*	m_armatur;

	static MainScene*	m_TempMainScene;

};

