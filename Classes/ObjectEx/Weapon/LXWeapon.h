#pragma once
#include "ObjectEx/LXObject.h"

enum 
{
	bullet_type_animiaton = 0,
	bullet_type_particle
};

class LXShooterLogic;
struct _DBC_BULLET;
class LXWeapon : public LXObject
{
public:
	LXWeapon();
	~LXWeapon();

public:
	// ��ʼ��
	virtual void				Init(int value);
	
	// ����
	virtual void		Relive();

	// ��ʼ���ӵ����
	virtual void				InitRenderInfo(const _DBC_BULLET *pBulletData);

	// ���߼�ѭ��
	virtual void				Tick(float fDelta);

	// ����
	virtual void				Destroy();

	// ������ʼ��
	virtual void				InitAction() {}

	// ������Ч
	virtual void				PlayShootEffect();

public:
	// ������������
	void						CalcBulletDegree();

	// ���ݳ�ʼ��
	virtual void				SetShooterLogic(LXShooterLogic* pLogic) { m_pShooterLogic = pLogic; }
	LXShooterLogic*				GetShooterLogic() { return m_pShooterLogic; }

	// �Ƿ񶯻�
	bool						IsAnimation() { return m_bAnimation; }


protected:
	// ����ӵ��߼�
	virtual void				playerWeaponLogic();

	// AI�ӵ��߼�
	virtual void				AIWeaponLogic();


protected:
	LXShooterLogic			*m_pShooterLogic;		// ���������߼�
	bool					m_bResDir;				// �ӵ���Դ�Ƿ��г�����Ϣ
	bool					m_bHit;					// �Ƿ��Ѿ�����Ŀ��
	bool					m_bAnimation;			// �Ƿ񶯻�
	const _DBC_BULLET		*m_pBulletData;
};
