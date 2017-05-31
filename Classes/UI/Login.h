#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

class Login : public Layer
{
public:
	Login(void);
	~Login(void);
	
	CREATE_FUNC(Login);

	virtual bool init();

public:


	// ��ʼ��
	void	Init();

	// ��Ӽ���
	void  OpenListener();
	void  CloseListener();

	// �����ص�
	virtual bool onTouchBegan(Touch *touch, Event *unused_event); 
	virtual void onTouchEnded(Touch *touch, Event *unused_event); 

	// ��ȡ�ص�
	void	ReceiveTouchEvent(Ref* pSender, Widget::TouchEventType type);

	// ��ȡ�ص�
	void	SureTouchEvent(Ref* pSender, Widget::TouchEventType type);


private:

	std::vector<ImageView*>		m_vecRewardBack;		// ��������

	std::vector<ImageView*>		m_vecRewardNum;			// ��������

	std::vector<ImageView*>		m_vecRewardEnd;			// ����ȡ

	ImageView*					m_ImageSureBack;		// ȷ������

	std::vector<ImageView*>		m_vecPrizeImage;		// ��Ʒ

	std::vector<ImageView*>		m_vecPrizeNum;			// ��Ʒ����

	int							m_CurrIndex;			// ��ǰ����
	int							m_RandNum;				// �������

};

