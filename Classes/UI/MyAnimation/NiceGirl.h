#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;


class NiceGirl :	public Layer
{
public:
	NiceGirl(void);
	~NiceGirl(void);

	CREATE_FUNC(NiceGirl);

	virtual bool init();
	bool	Init();

	// 每天第一次打开时调用
	void	ShowNiceGirl();

	/*---------------------------------------------------------------------*/
	// 教学时调用
	void	ShowNewPlayerInfo();

	// 新手教学步骤
	void	NewPlayerStep();

	static void SetFreshManCurrStep(int nId);
	static int	GetFreshManCurrStep(){ return FreshManCurrStep; };

	// 战斗外说话
	void	TalkingWithBattleOut(int nTalkId);

	// 战斗内说话
	void	TalkingWhitBattleIn(int nTalkId);

	// 新手点击
	void	ClickPos( float x, float y, float scaleX, float scaleY , float width ,float hight );

	// 新手关闭
	void	CloseMeWithNewPlayer();


	/*---------------------------------------------------------------------*/
	// 关卡剧情调用 -------
	// 剧情-关卡开始
	void	ShowMissionStartStory(int nMissionID);

	// 剧情-关卡结束
	void	ShowMissionEndStory(int nMissionID);



	// 触摸监听 --- 单点触摸
	void		OpenListener();
	void		CloseListener();

	// 触摸回调
	virtual bool onTouchBegan(Touch *touch, Event *unused_event); 
	virtual void onTouchEnded(Touch *touch, Event *unused_event); 

private:
	
	ImageView*	m_ClickImage;


	Armature*	m_armatur;

	ImageView*	m_BigTalkBack;			// 大对话框
	Text*		m_GirlInfo;				// 美女对话框

	ImageView*	m_SmallTalkBack;		// 小对话框
	Text*		m_SmallTalkInfo;		// 小对话框对话信息

	bool		m_IsMissionStoryIng;	// 是否是关卡剧情
	bool		m_IsFreshGuidIng;		// 是否是新手引导

	static int	FreshManCurrStep;		// 当前新手步骤

};

