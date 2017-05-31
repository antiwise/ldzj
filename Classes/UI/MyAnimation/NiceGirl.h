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

	// ÿ���һ�δ�ʱ����
	void	ShowNiceGirl();

	/*---------------------------------------------------------------------*/
	// ��ѧʱ����
	void	ShowNewPlayerInfo();

	// ���ֽ�ѧ����
	void	NewPlayerStep();

	static void SetFreshManCurrStep(int nId);
	static int	GetFreshManCurrStep(){ return FreshManCurrStep; };

	// ս����˵��
	void	TalkingWithBattleOut(int nTalkId);

	// ս����˵��
	void	TalkingWhitBattleIn(int nTalkId);

	// ���ֵ��
	void	ClickPos( float x, float y, float scaleX, float scaleY , float width ,float hight );

	// ���ֹر�
	void	CloseMeWithNewPlayer();


	/*---------------------------------------------------------------------*/
	// �ؿ�������� -------
	// ����-�ؿ���ʼ
	void	ShowMissionStartStory(int nMissionID);

	// ����-�ؿ�����
	void	ShowMissionEndStory(int nMissionID);



	// �������� --- ���㴥��
	void		OpenListener();
	void		CloseListener();

	// �����ص�
	virtual bool onTouchBegan(Touch *touch, Event *unused_event); 
	virtual void onTouchEnded(Touch *touch, Event *unused_event); 

private:
	
	ImageView*	m_ClickImage;


	Armature*	m_armatur;

	ImageView*	m_BigTalkBack;			// ��Ի���
	Text*		m_GirlInfo;				// ��Ů�Ի���

	ImageView*	m_SmallTalkBack;		// С�Ի���
	Text*		m_SmallTalkInfo;		// С�Ի���Ի���Ϣ

	bool		m_IsMissionStoryIng;	// �Ƿ��ǹؿ�����
	bool		m_IsFreshGuidIng;		// �Ƿ�����������

	static int	FreshManCurrStep;		// ��ǰ���ֲ���

};

