//升级装备界面


#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

class UpgradeLayer : public cocos2d::Layer
{
public:

	UpgradeLayer();
	~UpgradeLayer();

	virtual bool init();

	CREATE_FUNC(UpgradeLayer);

	//初始化
	void	InitData(); 

	//button
	void	UpgradeEvent(Ref* pSender, Widget::TouchEventType type);		// 升级
	void	MaxEvent(Ref* pSender, Widget::TouchEventType type);			// 所选部件满级
	void	MaxAllEvent(Ref* pSender, Widget::TouchEventType type);			// 全部满级
	void	BackEvent(Ref* pSender, Widget::TouchEventType type);			// 返回
	void	HelpEvent(Ref* pSender, Widget::TouchEventType type);			// 帮助
	void	BuyEvent(Ref* pSender, Widget::TouchEventType type);			// 购买宝石

	//checkbox
	void	DiamondEvent(Ref* pSender, Widget::TouchEventType type);
	void	WingEvent(Ref* pSender, Widget::TouchEventType type);
	void	FireEvent(Ref* pSender, Widget::TouchEventType type);
	void	EruptEvent(Ref* pSender, Widget::TouchEventType type);
	void	ShieldEvent(Ref* pSender, Widget::TouchEventType type);
	void	SkillEvent(Ref* pSender, Widget::TouchEventType type);

	//
	void	SetDiamond(int diamond);
	void	SetUpgradeCost(int cost);
	void	SetDiscribe(std::string& text);

	//设置等级
	void	SetDiamondLevel(int level);
	void	SetWingLevel(int level);
	void	SetFireLevel(int level);
	void	SetCrazyLevel(int level);
	void	SetShieldLevel(int level);
	void	SetBombLevel(int level);

	int		GetSelectedType();			//获取选中项类型

	/*---------------------------------------------------------------------*/
	// 帮助关闭回调
	void	HelpCloseTouchEvent(Ref* pSender, Widget::TouchEventType type);

private:

	void	SelectCheckBox(int Index);

	int				m_nSelectedType;	//

	TextAtlas*		m_Diamond;			// 拥有宝石
	TextAtlas*		m_UpgradeCost;		// 升级消耗
	
	Button*			m_Upgrade;			// 升级
	Button*			m_Max;				// 升至满级
	Button*			m_MaxAll;			// 一键满级
	Button*			m_Back;				// 返回
	Button*			m_Help;				// 帮助
	Button*			m_BuyDiamond;		// 获得宝石

	Text*			m_Describe;			// 描述

	CheckBox*		m_DiamonCB;			// 宝石转化
	CheckBox*		m_WingCB;			// 僚机支援
	CheckBox*		m_FireCB;			// 粒子光炮
	CheckBox*		m_EruptCB;			// 暴走攻击
	CheckBox*		m_ShieldCB;			// 量子护盾
	CheckBox*		m_SkillCB;			// 必杀攻击
	CheckBox*		m_CheckBox[6];		// checkbox合集

	ImageView*		m_DiamondLevel[5];	// 宝石等级
	ImageView*		m_WingLevel[5];		// 僚机等级
	ImageView*		m_FireLevel[5];		// 光炮等级
	ImageView*		m_EruptLevel[5];	// 暴走等级
	ImageView*		m_ShieldLevel[5];	// 护盾等级
	ImageView*		m_SkillLevel[5];	// 必杀等级


	//--帮助相关
	ImageView*		m_HelpBackImage;	//帮助图片背景 
	ImageView*		m_HelpImage;		//帮助图片
	Button*			m_HelpClose;		//帮助关闭按钮

};
