#pragma once
#include "LXEnemyPlane.h"

class LXBossPlane : public LXEnemyPlane
{
public:
	LXBossPlane();
	~LXBossPlane();

public:
	// ��ʼ��
	virtual void				Init(int value);

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

	// ����
	virtual void				Destroy();

	// ˮ�������߼�
	virtual void				DropCrystal();

	// ��Ʒ�����߼�
	virtual void				DropItem();

	// ���bossѪ��
	virtual bool				CheckHP(int value);

	// ��ȡ����·��
	virtual std::string			GetStartAnimationPath();

	// ��ȡ��������
	virtual std::string			GetStartAnimationName();

	virtual void				Hit(int damage);

private:
	// ���BOSS��ײ��Ϣ
	void						SplitBossCollideInfo();

protected:
	const _DBC_BOSS		*m_pBossData;	// ��������
};
