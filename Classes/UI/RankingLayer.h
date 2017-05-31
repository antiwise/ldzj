#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

struct RankingInfo
{
	int				_Randking;		// 排名
	int				_Score;			// 得分
	std::string		_Name;			// 名字

};

class RankingLayer : public Layer
{
public:
	RankingLayer(void);
	~RankingLayer(void);
	
	CREATE_FUNC(RankingLayer);

	virtual bool init();

public:

	// 初始化
	void	Init();

	// 循环
	void	Tick(float dt);

	// 加载成就奖励数据
	void	LoadRewardInfo();
	void	ShowRewardInfo();

	// 加载排行数据
	void	LoadRankingInfo();
	void	ShowRankingInfo();

	// 关闭按钮
	void	CloseCallback(Ref* pSender, Widget::TouchEventType type);

	// 点击成就按钮页
	void	ClickRewardCallback(Ref* pSender, Widget::TouchEventType type);

	// 点击排行按钮页
	void	ClickRankingCallback(Ref* pSender, Widget::TouchEventType type);

	// 点击奖励按钮
	void	ReceiveCallback(Ref* pSender, Widget::TouchEventType type,int id);
	
	// 列表选择
	void	SelectedItemEvent(Ref* pSender, ListView::EventType type);


	// 设置我的分数
	void	SetMyScore(int nNum);

	// 根据分数计算自己的排名
	static int		CalcMyRankingWithMyScore(int nMyScore);

	// 根据索引计算别人排名
	int		CalcRanking(int idx);

	// 根据排名计算得分
	static int		CalcScore(int randking);

	static std::string		RandomOtherName();
	
	static std::string		RandomMyName();
	

private:

	std::list<Layout*>			m_listRewardInfo;

	int							m_DeleteID;

	bool						m_isNeedDelete;
	
	/*---------------------------------------------------------------------*/

	int			m_CurrRanking;		// 记录我当前的排名
	
	int			m_CurrScore;		// 记录我的得分

	bool		m_isCalcRanking;	// 重新计算排名

	std::vector<RankingInfo*>	m_vecRanking;

private:
	ImageView*		m_RewardButton;						// 成就奖励按钮

	ImageView*		m_RankingButton;					// 世界排名按钮


	// -- 成就奖励 ----------------------------

	ListView*		m_ListView;							// 列表容器

	// -- 世界排名 ----------------------------
	ImageView*					m_RankingBackGround;	// 排名背景
	ImageView*					m_FirstIcon;			// 第一图标
	ImageView*					m_SecondIcon;			// 第一图标
	ImageView*					m_ThreeIcon;			// 第一图标
	std::vector<ImageView*>		m_vecBackImage;			// 背景图片
	std::vector<ImageView*>		m_vecIcon;				// 图标
	std::vector<Text*>			m_vecPlace;				// 名次
	std::vector<Text*>			m_vecName;				// 名字
	std::vector<Text*>			m_vecScore;				// 得分
	
};