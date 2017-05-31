#pragma once
#include "ObjectEx/LXObject.h"

class LXPlane : public LXObject
{
public:
	LXPlane();
	~LXPlane();

public:
	// ��ʼ��
	virtual void				Init(int value);

	// ���߼�ѭ��
	virtual void				Tick(float fDelta);

	// ����
	virtual void				Destroy();
	
	// ˮ�������߼�
	virtual void				DropCrystal();

	// ��Ʒ�����߼�
	virtual void				DropItem();

	// ˮ������
	virtual void				CreatCrystal(bool DropGoodsCrystal,std::vector<int> vec);

	// ���ߴ���
	virtual void				CreatItem(int ItemId);

	// ��������
	void						SetDieAnimate(const char* anim){ m_DieAnimate = anim; }
	std::string					GetDieAnimate(){ return m_DieAnimate; }

public:

	void			SetDropItemID( int nId ){ m_nDropItemID = nId; }
	int				GetDropItemID(){ return m_nDropItemID; }

	void			SetDropItemNum( int nNum ){ m_nDropItemNum = nNum; }
	int				GetDropItemNum(){ return m_nDropItemNum; }

	void			SetScore( int nScore ){ m_nScore = nScore; }
	int				GetScore(){ return m_nScore; }

private:
	int				m_nDropItemID;		// ������ƷID
	int				m_nDropItemNum;		// ���䱦ʯ����ID
	int				m_nScore;			// ս������

	std::string		m_DieAnimate;		// ��������
};
