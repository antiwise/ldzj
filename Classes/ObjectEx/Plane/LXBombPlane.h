#pragma once
#include "LXPlane.h"

class LXBombPlane : public LXPlane
{
public:
	LXBombPlane();
	~LXBombPlane();

public:
	// ��ʼ��
	virtual void				Init(int value);

	// ���߼�ѭ��
	virtual void				Tick(float fDelta);

	// ����
	virtual void				Destroy();

public:
	// ����״̬
	void				setBombStatus(bool value) { m_bBomb = value; }


private:
	bool				m_bBomb;
	float				m_fBombTime;			// ��ɱ����ʱ��

};
