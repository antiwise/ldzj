#pragma once
#include "LXPlane.h"
#include "DataBase/LXDBC_Struct.h"
#include "GameDefine.h"


class LXEnemyPlane : public LXPlane
{
public:
	LXEnemyPlane();
	~LXEnemyPlane();

public:
	// ��ʼ��
	virtual void				Init(int value);
	
	// ����
	virtual void				Relive();
	
	// ���ݳ�ʼ��
	virtual void				InitData();

	// ���߼�ѭ��
	virtual void				Tick(float fDelta);

	// �����Ч
	virtual void				PlayDestroyEffect();

	// �����Ч
	virtual void				PlayDestroySound();

	// ����߼�
	virtual void				DestroyLogic();

	// ˮ�������߼�
	virtual void				DropCrystal();

	// ����
	virtual void				Destroy();
	
	// ��ȡ����·��
	virtual std::string			GetStartAnimationPath();

	// ��ȡ��������
	virtual std::string			GetStartAnimationName();

	// �˺�
	virtual void				Hit(int damage);

protected:
	const _DBC_ENEMY		*m_pEnemyData;	// ��������
	const _DBC_PLANE		*m_pPlaneData;	// ��������

	Sprite					*m_pBloodBack;	// Ѫ������
	ProgressTimer			*m_pBloodBar;	// Ѫ��

};
