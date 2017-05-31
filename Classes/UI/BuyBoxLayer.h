#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "GameDefine.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;


class BuyBoxLayer : public Layer
{
public:
	BuyBoxLayer(void);
	~BuyBoxLayer(void);
	
	CREATE_FUNC(BuyBoxLayer);

	virtual bool init();

public:

	// 直接弹窗
	void	InitBuyType( BuyType eType, bool isPlayAnima = true);

	// 弹2次框
	void	InitOverflowBag( BuyType eType = BuyType::Buy_Diamond, bool isPlayAnima = true );

	// 倒计时弹窗
	void	InitCountDown( BuyType eType = BuyType::Buy_Life, int durationTime = 5 , bool isPlayAnima = true );

	// 购买
	//void	Pay();//付费
	void	GoToBuy();

	// -- 按钮callback ---------------------
	void	SureTouchEvent(Ref* pSender, Widget::TouchEventType type);

	void	CloseTouchEvent(Ref* pSender, Widget::TouchEventType type);

	// 数据接口
	void	CallSDKBuy(int buyType);
	void	SDKReply(int butType);

	static void	SetIsShowBettlePlane( bool _isShow ){ m_IsShowBettlePlane = _isShow; } 
	
	// --- 倒计时 ---------------------------------------------------
	void	InitCountDownInfo();				// 初始化倒计时信息

	void	StopCountDownTick();				// 停止倒计时

	void	CountDownTick(float dt);

	void	CountDownAnimationCallBack();		// 倒计时回调
	

private:

	int				m_BuyType;				// 购买类型
	int				m_OverflowBagNextType;	// 不购买超值礼包然后弹出的购买类型
	int				m_CountDownDoneNextType;// 倒计时结束然后弹出的购买类型

	ImageView*		m_BigBuyBackImage;		// 大卖背景
	Layout*			m_SmallBuyLayout;		// 小卖
	Layout*			m_BigBuyLayout;			// 大卖

	ImageView*		m_BoxName;				// 标题
	ImageView*		m_BuyInfo;				// 图片

	ImageView*		m_BackImage;			// 背景框
	Button*			m_CloseButton;			// 关闭按钮

	Button*			m_Button_duihuan;
	Button*			m_Button_lingqu;
	Button*			m_Button_shengji;
	Button*			m_Button_shiyong;
	Button*			m_Button_zhuangbei;


	// 倒计时
	Layout*			m_CountDownBack;		// 倒计时背景
	ImageView*		m_FrameImage;			// 边框图
	ImageView*		M_NumberImageIn;		// 数字图
	ImageView*		M_NumberImageOut;		// 数字图

	bool			m_GameSceneIsPause;		// 战斗场景是否暂停

	bool			m_IsPlayAnimation;		// 是否播放动画

	bool			m_BuyWithCountDown;		// 倒计时购买

	static bool		m_IsShowBettlePlane;	// 是否显示最好飞机

	int				m_DurationTime;			// 倒计时时间;
};
