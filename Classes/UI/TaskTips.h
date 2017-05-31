#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

class TaskTips : public Layer
{
public:
	TaskTips(void);
	~TaskTips(void);
	
	CREATE_FUNC(TaskTips);

	virtual bool init();

public:

	// ��ʼ��
	void	Init(std::string str);

	void	Tick(float dt);

private:

	float	m_MoveLength;		// �ƶ�����

	Label*	m_ShowLable;
};

