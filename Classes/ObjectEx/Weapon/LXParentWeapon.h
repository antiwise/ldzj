#pragma once
#include "LXWeapon.h"

class LXParentWeapon : public LXWeapon
{
public:
	LXParentWeapon();
	~LXParentWeapon();

public:
	// ��ʼ��
	virtual void				Init(int value);

	// ���߼�ѭ��
	virtual void				Tick(float fDelta);

	// ����
	virtual void				Destroy();

};
