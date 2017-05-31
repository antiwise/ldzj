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

	// ֱ�ӵ���
	void	InitBuyType( BuyType eType, bool isPlayAnima = true);

	// ��2�ο�
	void	InitOverflowBag( BuyType eType = BuyType::Buy_Diamond, bool isPlayAnima = true );

	// ����ʱ����
	void	InitCountDown( BuyType eType = BuyType::Buy_Life, int durationTime = 5 , bool isPlayAnima = true );

	// ����
	//void	Pay();//����
	void	GoToBuy();

	// -- ��ťcallback ---------------------
	void	SureTouchEvent(Ref* pSender, Widget::TouchEventType type);

	void	CloseTouchEvent(Ref* pSender, Widget::TouchEventType type);

	// ���ݽӿ�
	void	CallSDKBuy(int buyType);
	void	SDKReply(int butType);

	static void	SetIsShowBettlePlane( bool _isShow ){ m_IsShowBettlePlane = _isShow; } 
	
	// --- ����ʱ ---------------------------------------------------
	void	InitCountDownInfo();				// ��ʼ������ʱ��Ϣ

	void	StopCountDownTick();				// ֹͣ����ʱ

	void	CountDownTick(float dt);

	void	CountDownAnimationCallBack();		// ����ʱ�ص�
	

private:

	int				m_BuyType;				// ��������
	int				m_OverflowBagNextType;	// ������ֵ���Ȼ�󵯳��Ĺ�������
	int				m_CountDownDoneNextType;// ����ʱ����Ȼ�󵯳��Ĺ�������

	ImageView*		m_BigBuyBackImage;		// ��������
	Layout*			m_SmallBuyLayout;		// С��
	Layout*			m_BigBuyLayout;			// ����

	ImageView*		m_BoxName;				// ����
	ImageView*		m_BuyInfo;				// ͼƬ

	ImageView*		m_BackImage;			// ������
	Button*			m_CloseButton;			// �رհ�ť

	Button*			m_Button_duihuan;
	Button*			m_Button_lingqu;
	Button*			m_Button_shengji;
	Button*			m_Button_shiyong;
	Button*			m_Button_zhuangbei;


	// ����ʱ
	Layout*			m_CountDownBack;		// ����ʱ����
	ImageView*		m_FrameImage;			// �߿�ͼ
	ImageView*		M_NumberImageIn;		// ����ͼ
	ImageView*		M_NumberImageOut;		// ����ͼ

	bool			m_GameSceneIsPause;		// ս�������Ƿ���ͣ

	bool			m_IsPlayAnimation;		// �Ƿ񲥷Ŷ���

	bool			m_BuyWithCountDown;		// ����ʱ����

	static bool		m_IsShowBettlePlane;	// �Ƿ���ʾ��÷ɻ�

	int				m_DurationTime;			// ����ʱʱ��;
};
