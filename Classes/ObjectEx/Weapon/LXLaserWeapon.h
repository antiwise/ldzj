#pragma once
#include "LXWeapon.h"

class LXLaserWeapon : public LXWeapon
{
public:
	LXLaserWeapon();
	~LXLaserWeapon();

public:
	// ��ʼ��
	virtual void				Init(int value);
	
	// ��ʼ���ӵ����
	virtual void				InitRenderInfo(const _DBC_BULLET *pBulletData) {}

	// ���߼�ѭ��
	virtual void				Tick(float fDelta);

	// ����
	virtual void				Destroy();
	
	// ������ʼ��
	virtual void				InitAction();
	
	// ������Ч
	virtual void				PlayShootEffect();

	// ���ݳ�ʼ��
	virtual void				SetShooterLogic(LXShooterLogic* pLogic);

	void						SetPos(Vec2 value) { m_Pos = value; }

public:
	void				SetCanThrough(bool value) { m_bCanThrough = value;}
	bool				GetCanThrough() { return m_bCanThrough; }
	
protected:
	// ����ӵ��߼�
	virtual void				playerWeaponLogic();

	// AI�ӵ��߼�
	virtual void				AIWeaponLogic();
	
	// ��ȡ����Ԫ��
	Sprite*						GetRenderElement(int index);

private:
	std::vector<Sprite*> m_SpritePool;	// ����Ԫ�ػ����
	
	Vec2				m_Pos;				// ����λ��
	bool				m_bCanThrough;		// �Ƿ��л���Ч��
	float				m_fLastTick;		// ����ʱ��
	int					m_nPos;				// λ��
	int					m_nPosOffset;		// λ�ñ仯
	int					m_nTickNum;			// �û������ܻ�Ч��
	LXObject*			m_pObject;			// ���е�Ŀ��
	float				m_fHeight;			// ���ⳤ��
	Rect				m_Rect;				// ����ͼ���С
	std::string			m_ResPath;			// ��Դ·��

	ParticleEffect*		m_Effect_Fire;		// ����������Ч
	ParticleEffect*		m_Effect_Hited;		// ����������Ч
	float				m_fScale;			// ����

};
