#pragma once
#include "LXPlane.h"

class LXWingPlane : public LXPlane
{
public:
	LXWingPlane();
	~LXWingPlane();

public:
	// ��ʼ��
	virtual void				Init(int value);
	
	// ���ݳ�ʼ��
	virtual void				InitData();

	// ���߼�ѭ��
	virtual void				Tick(float fDelta);

	// ����
	virtual void				Destroy();
	
public:
	// ��ʼ��
	void						Init(int resID, bool bFlip);

};
