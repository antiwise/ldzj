//����װ������


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

	//��ʼ��
	void	InitData(); 

	//button
	void	UpgradeEvent(Ref* pSender, Widget::TouchEventType type);		// ����
	void	MaxEvent(Ref* pSender, Widget::TouchEventType type);			// ��ѡ��������
	void	MaxAllEvent(Ref* pSender, Widget::TouchEventType type);			// ȫ������
	void	BackEvent(Ref* pSender, Widget::TouchEventType type);			// ����
	void	HelpEvent(Ref* pSender, Widget::TouchEventType type);			// ����
	void	BuyEvent(Ref* pSender, Widget::TouchEventType type);			// ����ʯ

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

	//���õȼ�
	void	SetDiamondLevel(int level);
	void	SetWingLevel(int level);
	void	SetFireLevel(int level);
	void	SetCrazyLevel(int level);
	void	SetShieldLevel(int level);
	void	SetBombLevel(int level);

	int		GetSelectedType();			//��ȡѡ��������

	/*---------------------------------------------------------------------*/
	// �����رջص�
	void	HelpCloseTouchEvent(Ref* pSender, Widget::TouchEventType type);

private:

	void	SelectCheckBox(int Index);

	int				m_nSelectedType;	//

	TextAtlas*		m_Diamond;			// ӵ�б�ʯ
	TextAtlas*		m_UpgradeCost;		// ��������
	
	Button*			m_Upgrade;			// ����
	Button*			m_Max;				// ��������
	Button*			m_MaxAll;			// һ������
	Button*			m_Back;				// ����
	Button*			m_Help;				// ����
	Button*			m_BuyDiamond;		// ��ñ�ʯ

	Text*			m_Describe;			// ����

	CheckBox*		m_DiamonCB;			// ��ʯת��
	CheckBox*		m_WingCB;			// �Ż�֧Ԯ
	CheckBox*		m_FireCB;			// ���ӹ���
	CheckBox*		m_EruptCB;			// ���߹���
	CheckBox*		m_ShieldCB;			// ���ӻ���
	CheckBox*		m_SkillCB;			// ��ɱ����
	CheckBox*		m_CheckBox[6];		// checkbox�ϼ�

	ImageView*		m_DiamondLevel[5];	// ��ʯ�ȼ�
	ImageView*		m_WingLevel[5];		// �Ż��ȼ�
	ImageView*		m_FireLevel[5];		// ���ڵȼ�
	ImageView*		m_EruptLevel[5];	// ���ߵȼ�
	ImageView*		m_ShieldLevel[5];	// ���ܵȼ�
	ImageView*		m_SkillLevel[5];	// ��ɱ�ȼ�


	//--�������
	ImageView*		m_HelpBackImage;	//����ͼƬ���� 
	ImageView*		m_HelpImage;		//����ͼƬ
	Button*			m_HelpClose;		//�����رհ�ť

};
