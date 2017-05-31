#include "SuspendLayer.h"
#include "Scene/GameScene.h"
#include "Sound/LXSoundSystem.h"
#include "GameDefine.h"
#include "DB/DBManager.h"
#include "Common/FileUtils.h"

SuspendLayer::SuspendLayer():Layer()
{

}

SuspendLayer::~SuspendLayer()
{

}

bool SuspendLayer::init()
{
	// 1. super init first
	if ( !Layer::init() )
	{
		return false;
	}

	// ��ͣ����
	std::string key = "zanting";
	LXFileUtils::Get_UI_Json_FullPath(key);
	Layout* pSuspendLayout = static_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile( key.c_str() ));
	if (!pSuspendLayout)
	{
		return false;
	}

	this->addChild(pSuspendLayout);

	// ͼƬ
	m_LeftPicture = static_cast<ImageView*>(Helper::seekWidgetByName(pSuspendLayout,"leftPic"));

	m_RightPicture = static_cast<ImageView*>(Helper::seekWidgetByName(pSuspendLayout,"rightPic"));

	m_SuspendPicture = static_cast<ImageView*>(Helper::seekWidgetByName(pSuspendLayout,"zanting"));

	m_Small_1 = static_cast<ImageView*>(Helper::seekWidgetByName(pSuspendLayout,"x1"));

	m_Small_2 = static_cast<ImageView*>(Helper::seekWidgetByName(pSuspendLayout,"x2"));

	m_Small_3 = static_cast<ImageView*>(Helper::seekWidgetByName(pSuspendLayout,"x3"));

	m_Big_1 = static_cast<ImageView*>(Helper::seekWidgetByName(pSuspendLayout,"1"));

	m_Big_2 = static_cast<ImageView*>(Helper::seekWidgetByName(pSuspendLayout,"2"));

	m_Big_3 = static_cast<ImageView*>(Helper::seekWidgetByName(pSuspendLayout,"3"));

	m_Hexagon_1 = static_cast<ImageView*>(Helper::seekWidgetByName(pSuspendLayout,"guangquan1"));

	m_Hexagon_2 = static_cast<ImageView*>(Helper::seekWidgetByName(pSuspendLayout,"guangquan2"));

	m_AbandonBack = static_cast<ImageView*>(Helper::seekWidgetByName(pSuspendLayout,"Image_5"));

	m_GoOnBack = static_cast<ImageView*>(Helper::seekWidgetByName(pSuspendLayout,"Image_6"));

	// ������ť
	m_GoOnBtn	= static_cast<Button*>(Helper::seekWidgetByName(pSuspendLayout, "jixu"));
	m_GoOnBtn->addTouchEventListener(CC_CALLBACK_2(SuspendLayer::GoOnTouchEvent, this));

	// ������ť
	m_AbandonBtn = static_cast<Button*>(Helper::seekWidgetByName(pSuspendLayout, "fangqi"));
	m_AbandonBtn->addTouchEventListener(CC_CALLBACK_2(SuspendLayer::AbandonTouchEvent, this));

	// λ��
	m_LeftPicPos		= m_LeftPicture->getPosition();
	m_RightPicPos		= m_RightPicture->getPosition();
	m_AbandonBackPos	= m_AbandonBack->getPosition();
	m_GoOnBackPos		= m_GoOnBack->getPosition();
	m_AbandonBtnPos		= m_AbandonBtn->getPosition();
	m_GoOnBtnPos		= m_GoOnBtn->getPosition();

	// �ߴ�
	m_LeftPicSize		= m_LeftPicture->getContentSize();
	m_RightPicSize		= m_RightPicture->getContentSize();
	m_AbandonBackSize	= m_AbandonBack->getContentSize();
	m_GoOnBackSize		= m_GoOnBack->getContentSize();
	m_AbandonBtnSize	= m_AbandonBtn->getContentSize();
	m_GoOnBtnSize		= m_GoOnBtn->getContentSize();

	//Init();

	return true;
}

void SuspendLayer::Init()
{
	this->setVisible(true);
	m_IsClickGoOnButton = false;

	m_LeftPicture ->setVisible(false);			// ��ͼ
	m_RightPicture ->setVisible(false);			// ��ͼ
	m_SuspendPicture ->setVisible(false);		// ��ͣͼ
	m_Small_1 ->setVisible(false);				// С 1
	m_Small_2 ->setVisible(false);				// С 2
	m_Small_3 ->setVisible(false);				// С 3
	m_Big_1 ->setVisible(false);				// �� 1
	m_Big_2 ->setVisible(false);				// �� 2
	m_Big_3 ->setVisible(false);				// �� 3
	m_Hexagon_1 ->setVisible(false);			// ������ 1
	m_Hexagon_2 ->setVisible(false);			// ������ 2
	m_AbandonBack ->setVisible(false);			// ��������
	m_GoOnBack ->setVisible(false);				// ��������
	m_AbandonBtn ->setVisible(false);			// ������ť
	m_GoOnBtn ->setVisible(false);				// ������ť

	float time1 = 0.4f;
	float time2 = 0.2f;
	float time3 = 0.09f;
	float time4 = 0.04f;
	//
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// ��ͼ
	ActionInterval*	_placeOfLeft1	=	MoveTo::create( 0, Vec2( 0 - m_LeftPicSize.width/2, m_LeftPicPos.y + 15 ));				// �����������
	ActionInstant*	_show1			=	Show::create();																			// ��ʾ
	ActionInterval*	_moveToRight1_1	=	MoveTo::create( time1, Vec2( m_LeftPicPos.x - 15, m_LeftPicPos.y + 15 ));				// �ƶ�����ʼλ��
	ActionInterval*	_easeSineIn1	=	EaseSineIn::create( _moveToRight1_1 );													// �ƶ�״̬Ϊ��������
	ActionInterval* _delay1			=	DelayTime::create( time3 );
	ActionInterval*	_moveToRight1_2	=	MoveTo::create( time4, m_LeftPicPos );													// �ƶ�����ʼλ��
	ActionInterval * easeBackIn1	=	EaseBackIn::create(_moveToRight1_2);
	ActionInterval* _LeftAction1	=	Sequence::create( _placeOfLeft1, _show1, _easeSineIn1, _delay1, easeBackIn1, NULL);
	m_LeftPicture->runAction(_LeftAction1);

	// ��ͼ
	ActionInterval*	_placeOfRight2	=	MoveTo::create( 0, Vec2( visibleSize.width + origin.x + m_RightPicSize.width/2, m_RightPicPos.y - 15 ));	// �����������
	ActionInstant*	_show2			=	Show::create();																								// ��ʾ
	ActionInterval*	_moveToLeft2_1	=	MoveTo::create( time1, Vec2(m_RightPicPos.x + 15, m_RightPicPos.y - 15 ));									// �ƶ�����ʼλ��
	ActionInterval*	_easeSineIn2	=	EaseSineIn::create( _moveToLeft2_1 );																		// �ƶ�״̬Ϊ��������
	ActionInterval* _delay2			=	DelayTime::create( time3 );
	ActionInterval*	_moveToLeft2_2	=	MoveTo::create( time4 , m_RightPicPos );
	ActionInterval * easeBackIn2	=	EaseBackIn::create(_moveToLeft2_2);
	ActionInterval* _RightAction2	=	Sequence::create( _placeOfRight2, _show2, _easeSineIn2, _delay2, easeBackIn2, NULL);
	m_RightPicture->runAction(_RightAction2);

	// ��������
	ActionInterval*	_placeOfLeft3	=	MoveTo::create( 0, Vec2( 0 - m_AbandonBackSize.width/2, m_AbandonBackPos.y ));				// �����������
	ActionInstant*	_show3			=	Show::create();																			// ��ʾ
	ActionInterval*	_moveToRight3	=	MoveTo::create( time2, m_AbandonBackPos );													// �ƶ�����ʼλ��
	ActionInterval*	_easeSineIn3	=	EaseSineIn::create( _moveToRight3 );														// �ƶ�״̬Ϊ��������
	ActionInterval* _LeftAction3	=	Sequence::create( _placeOfLeft3, _show3, _easeSineIn3, NULL );	
	m_AbandonBack->runAction(_LeftAction3);

	// ��������
	ActionInterval*	_placeOfRight4	=	MoveTo::create( 0, Vec2( visibleSize.width + origin.x + m_GoOnBackSize.width/2, m_GoOnBackPos.y ));		// �����������
	ActionInstant*	_show4			=	Show::create();																							// ��ʾ
	ActionInterval*	_moveToLeft4	=	MoveTo::create( time2, m_GoOnBackPos );																	// �ƶ�����ʼλ��
	ActionInterval*	_easeSineIn4	=	EaseSineIn::create( _moveToLeft4 );																		// �ƶ�״̬Ϊ��������
	ActionInterval* _RightAction4	=	Sequence::create( _placeOfRight4, _show4, _easeSineIn4, NULL );
	m_GoOnBack->runAction(_RightAction4);

	// ������ť
	ActionInterval*	_placeOfLeft5	=	MoveTo::create( 0, Vec2( 0 - m_AbandonBtnSize.width/2, m_AbandonBtnPos.y ));					// �����������
	ActionInstant*	_show5			=	Show::create();																			// ��ʾ
	ActionInterval*	_moveToRight5	=	MoveTo::create( time2, m_AbandonBtnPos );													// �ƶ�����ʼλ��
	ActionInterval*	_easeSineIn5	=	EaseSineIn::create( _moveToRight5 );														// �ƶ�״̬Ϊ��������
	ActionInterval* _LeftAction5	=	Sequence::create( _placeOfLeft5, _show5, _easeSineIn5, NULL );	
	m_AbandonBtn->runAction(_LeftAction5);

	// ������ť
	ActionInterval*	_placeOfRight6	=	MoveTo::create( 0, Vec2( visibleSize.width + origin.x + m_GoOnBtnSize.width/2, m_GoOnBtnPos.y ));		// �����������
	ActionInstant*	_show6			=	Show::create();																							// ��ʾ
	ActionInterval*	_moveToLeft6	=	MoveTo::create( time2, m_GoOnBtnPos );																		// �ƶ�����ʼλ��
	ActionInterval*	_easeSineIn6	=	EaseSineIn::create( _moveToLeft6 );																		// �ƶ�״̬Ϊ��������
	ActionInterval* _RightAction6	=	Sequence::create( _placeOfRight6, _show6, _easeSineIn6, NULL );
	m_GoOnBtn->runAction(_RightAction6);

	ActionInterval* _Delay7 = DelayTime::create(time1);
	ActionInstant*	_Show7	= Show::create();
	ActionInterval* _Action7 = Sequence::create( _Delay7, _Show7, NULL);
	m_SuspendPicture->runAction(_Action7);

}

void SuspendLayer::GoOnTouchEvent(Ref* pSender, Widget::TouchEventType type)
{
	// �����һ�κ󣬲������ٴε��
	if (m_IsClickGoOnButton)
	{
		return ;
	}

	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		{
			SoundSystem().playEffect(Mic_count);
			m_IsClickGoOnButton = true;
			GoOnAnimationPlay();
			
		}
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void SuspendLayer::AbandonTouchEvent(Ref* pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		{
			SoundSystem().playEffect(Mic_sure);
			static_cast<GameScene*>(this->getParent()->getParent())->GameGiveUp();
		}
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}


void SuspendLayer::GoOnAnimationPlay()
{
	// Ϊ�˷������ʱ�䶨��
	float time1 = 0.4f;
	float time2 = 0.2f;
	float time3 = 0.12f;
	float time4 = 0.7f;
	float time5 = 0.04f;

	//
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// ��ͣ����
	m_SuspendPicture->setVisible(false);
	
	// ����1 �Ŵ� and ���� -- ����ʱ 3 2 1
	ActionInterval* _scaleto1_1 =	ScaleTo::create( 0.01, 1 );			// ��λ
	ActionInterval* _fadeIn1	=	FadeIn::create( 0.01 );				// ����
	ActionInstant*	_show		=	Show::create();						// ��ʾ
	ActionInterval* _scaleto1_2 =	ScaleTo::create( time4, 1.5 );		// �Ŵ�	1.5
	ActionInterval* _scaleto1_3 =	ScaleTo::create( time4, 2.0 );		// �Ŵ�	2.0
	ActionInterval* _fadeout1	=	FadeOut::create( time4 );			// ����
	ActionInstant*  _hide		=	Hide::create();

	FiniteTimeAction* _spawn1 =		Spawn::create( _scaleto1_2, _fadeout1, NULL );		// �Ŵ� �� ���� ͬʱ����
	FiniteTimeAction* _spawn2 =		Spawn::create( _scaleto1_3, _fadeout1, NULL );		// �Ŵ� �� ���� ͬʱ����

	ActionInterval* _sequenc1_1 =	Sequence::create( _scaleto1_1, _fadeIn1, _show, _spawn1, _hide, NULL );
	ActionInterval* _sequenc1_2 =	Sequence::create( _scaleto1_1->clone(), _fadeIn1->clone(), _show->clone(), _spawn2, _hide->clone(), NULL );

	m_Hexagon_2->runAction( _sequenc1_1 );
	m_Big_3->runAction( _sequenc1_2 );

	ActionInterval* _dalay2		= DelayTime::create( time4 + 0.1 );
	ActionInterval* _sequenc2_1 =	Sequence::create( _dalay2, _sequenc1_1->clone(), NULL );
	ActionInterval* _sequenc2_2 =	Sequence::create( _dalay2->clone(), _sequenc1_2->clone(), NULL );

	m_Hexagon_2->runAction( _sequenc2_1 );
	m_Big_2->runAction( _sequenc2_2 );

	ActionInterval* _dalay3		= DelayTime::create( time4*2 + 0.2 );
	ActionInterval* _sequenc3_1 =	Sequence::create( _dalay3, _sequenc1_1->clone(), NULL );
	ActionInterval* _sequenc3_2 =	Sequence::create( _dalay3, _sequenc1_2->clone(), NULL );

	m_Hexagon_2->runAction( _sequenc3_1 );
	m_Big_1->runAction( _sequenc3_2 );


	// ����2 -- ����ʱ �� ͬʱ �����Сͼ 3 2 1 ͬʱ��ʾ

	ActionInstant*	_showSmall		=	Show::create();						// ��ʾ
	ActionInstant*  _hideSmall		=	Hide::create();
	ActionInterval* _dalaySmall		=	DelayTime::create( time4 );
	ActionInterval* _sequencSmall =	Sequence::create( _showSmall, _dalaySmall,_hideSmall, NULL );	// 0�ȴ�

	m_Small_3->runAction(_sequencSmall);

	ActionInterval* _sequencSmall2 =	Sequence::create( _dalaySmall->clone(), _showSmall->clone(), _dalaySmall->clone(),_hideSmall->clone(), NULL );	// 1��time4�ȴ�
	
	m_Small_2->runAction(_sequencSmall2);

	ActionInterval* _sequencSmall3 =	Sequence::create( _dalaySmall->clone(), _dalaySmall->clone(),_showSmall->clone(), _dalaySmall->clone(),_hideSmall->clone(), NULL ); // 2��time4�ȴ�

	m_Small_1->runAction(_sequencSmall3);

	ActionInterval* _sequencSmall4 =	Sequence::create( _showSmall->clone(), _dalaySmall->clone(), _dalaySmall->clone(), _dalaySmall->clone(),_hideSmall->clone(), NULL );
	m_Hexagon_1->runAction(_sequencSmall4);

	// ����ʱ��Ϻ���� ���� �� ���� �˳�
	// ��ͼ
	ActionInterval* _dalayLeave		=	DelayTime::create( time4*3 + 0.3 );

	ActionInterval*	_moveToTopLeft1	= MoveTo::create( time5, Vec2( m_LeftPicPos.x - 15, m_LeftPicPos.y + 15 ));				// �ƶ�������
	ActionInterval*	_dalayToLeft1	= DelayTime::create(0.09);
	ActionInterval*	_placeOfLeft1	= MoveTo::create( time1, Vec2( 0 - m_LeftPicSize.width/2, m_LeftPicPos.y + 15 ));		// �ƶ��������
	ActionInstant*	_hide1			= Hide::create();									  
	ActionInterval* _action1		= Sequence::create( _dalayLeave, _moveToTopLeft1, _dalayToLeft1, _placeOfLeft1, _hide1, NULL);
	m_LeftPicture->runAction(_action1);

	// ��ͼ
	ActionInterval*	_moveToBomRight2	= MoveTo::create( time5, Vec2(m_RightPicPos.x + 15, m_RightPicPos.y - 15 ));									// �ƶ�������
	ActionInterval*	_dalayToRight2		= DelayTime::create(0.09);
	ActionInterval*	_placeOfRight2		= MoveTo::create( time1, Vec2( visibleSize.width + origin.x + m_RightPicSize.width/2, m_RightPicPos.y - 15 ));	// ���������ұ�
	ActionInstant*	_hide2				= Hide::create();
	ActionInterval* _action2			= Sequence::create( _dalayLeave->clone(), _moveToBomRight2, _dalayToRight2, _placeOfRight2, _hide2, NULL);
	m_RightPicture->runAction(_action2);
	
	// �����ж���ִ����Ϻ� ���ر�����
	ActionInterval*	_dalayToRight3		= DelayTime::create( time4*3 + 0.3 + 0.09 + time5 + time1 + 0.1);
	ActionInstant*	_hide3				= Hide::create();
	CallFunc *pCallFunc = CallFunc::create(CC_CALLBACK_0(SuspendLayer::Continue,this));
	ActionInterval* _sequencSmall5 =	Sequence::create( _dalayToRight3, _hide3,pCallFunc, NULL );
	this->runAction(_sequencSmall5);


	// ��� ������Ϸ ���²��� ���� ���� ���� �˳�
	// ��������
	ActionInterval*	_moveToRight3	=	MoveTo::create( time2,  Vec2( 0 - m_AbandonBackSize.width/2, m_AbandonBackPos.y ) );						// �ƶ��������
	m_AbandonBack->runAction(_moveToRight3);

	// ��������
	ActionInterval*	_moveToLeft4	=	MoveTo::create( time2, Vec2( visibleSize.width + origin.x + m_GoOnBackSize.width/2, m_GoOnBackPos.y ) );	// �ƶ������ұ�
	m_GoOnBack->runAction(_moveToLeft4);

	// ������ť
	ActionInterval*	_moveToRight5	=	MoveTo::create( time2, Vec2( 0 - m_AbandonBtnSize.width/2, m_AbandonBtnPos.y ) );							// �ƶ��������
	m_AbandonBtn->runAction(_moveToRight5);

	// ������ť
	ActionInterval*	_moveToLeft6	=	MoveTo::create( time2, Vec2( visibleSize.width + origin.x + m_GoOnBtnSize.width/2, m_GoOnBtnPos.y ) );		// �ƶ������ұ�
	m_GoOnBtn->runAction(_moveToLeft6);

}

void SuspendLayer::Continue()
{
	static_cast<GameScene*>(this->getParent()->getParent())->GameContinue();
}