#pragma once
#include "LXWeapon.h"

class LXCommonWeapon : public LXWeapon
{
public:
	LXCommonWeapon();
	~LXCommonWeapon();

public:
	// ��ʼ��
	virtual void				Init(int value);

	// ���߼�ѭ��
	virtual void				Tick(float fDelta);

	// ����
	virtual void				Destroy();

	// ������ʼ��
	virtual void				InitAction();

};
