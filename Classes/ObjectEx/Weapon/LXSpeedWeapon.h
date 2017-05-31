#pragma once
#include "LXWeapon.h"

class LXSpeedWeapon : public LXWeapon
{
public:
	LXSpeedWeapon();
	~LXSpeedWeapon();

public:
	// ��ʼ��
	virtual void				Init(int value);

	// ���߼�ѭ��
	virtual void				Tick(float fDelta);

	// ����
	virtual void				Destroy();
	
	// ������ʼ��
	virtual void				InitAction();
	
	// ���ݳ�ʼ��
	virtual void				SetShooterLogic(LXShooterLogic* pLogic);
};
