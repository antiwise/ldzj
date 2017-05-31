#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

struct RankingInfo
{
	int				_Randking;		// ����
	int				_Score;			// �÷�
	std::string		_Name;			// ����

};

class RankingLayer : public Layer
{
public:
	RankingLayer(void);
	~RankingLayer(void);
	
	CREATE_FUNC(RankingLayer);

	virtual bool init();

public:

	// ��ʼ��
	void	Init();

	// ѭ��
	void	Tick(float dt);

	// ���سɾͽ�������
	void	LoadRewardInfo();
	void	ShowRewardInfo();

	// ������������
	void	LoadRankingInfo();
	void	ShowRankingInfo();

	// �رհ�ť
	void	CloseCallback(Ref* pSender, Widget::TouchEventType type);

	// ����ɾͰ�ťҳ
	void	ClickRewardCallback(Ref* pSender, Widget::TouchEventType type);

	// ������а�ťҳ
	void	ClickRankingCallback(Ref* pSender, Widget::TouchEventType type);

	// ���������ť
	void	ReceiveCallback(Ref* pSender, Widget::TouchEventType type,int id);
	
	// �б�ѡ��
	void	SelectedItemEvent(Ref* pSender, ListView::EventType type);


	// �����ҵķ���
	void	SetMyScore(int nNum);

	// ���ݷ��������Լ�������
	static int		CalcMyRankingWithMyScore(int nMyScore);

	// �������������������
	int		CalcRanking(int idx);

	// ������������÷�
	static int		CalcScore(int randking);

	static std::string		RandomOtherName();
	
	static std::string		RandomMyName();
	

private:

	std::list<Layout*>			m_listRewardInfo;

	int							m_DeleteID;

	bool						m_isNeedDelete;
	
	/*---------------------------------------------------------------------*/

	int			m_CurrRanking;		// ��¼�ҵ�ǰ������
	
	int			m_CurrScore;		// ��¼�ҵĵ÷�

	bool		m_isCalcRanking;	// ���¼�������

	std::vector<RankingInfo*>	m_vecRanking;

private:
	ImageView*		m_RewardButton;						// �ɾͽ�����ť

	ImageView*		m_RankingButton;					// ����������ť


	// -- �ɾͽ��� ----------------------------

	ListView*		m_ListView;							// �б�����

	// -- �������� ----------------------------
	ImageView*					m_RankingBackGround;	// ��������
	ImageView*					m_FirstIcon;			// ��һͼ��
	ImageView*					m_SecondIcon;			// ��һͼ��
	ImageView*					m_ThreeIcon;			// ��һͼ��
	std::vector<ImageView*>		m_vecBackImage;			// ����ͼƬ
	std::vector<ImageView*>		m_vecIcon;				// ͼ��
	std::vector<Text*>			m_vecPlace;				// ����
	std::vector<Text*>			m_vecName;				// ����
	std::vector<Text*>			m_vecScore;				// �÷�
	
};