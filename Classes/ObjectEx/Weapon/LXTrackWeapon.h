#pragma once
#include "LXWeapon.h"

class LXMoveBy;
class LXTrackWeapon : public LXWeapon
{
public:
	LXTrackWeapon();
	~LXTrackWeapon();

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

private:
	LXMoveBy			*m_pTrackAction;		// ���ٶ���

};
