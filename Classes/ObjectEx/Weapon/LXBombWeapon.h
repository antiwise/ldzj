#pragma once
#include "LXWeapon.h"

class LXBombWeapon : public LXWeapon
{
public:
	LXBombWeapon();
	~LXBombWeapon();

public:
	// ��ʼ��
	virtual void				Init(int value);

	// ���߼�ѭ��
	virtual void				Tick(float fDelta);

	// ����
	virtual void				Destroy();
};
