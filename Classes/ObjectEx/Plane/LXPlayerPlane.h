#pragma once
#include "LXPlane.h"

enum
{
	player_plane_type_airplane = 0,
	player_plane_type_airplane2,
	player_plane_type_airplane3,
	player_plane_type_zuheji,
	player_plane_type_erhaoji
};

class ParticleEffect;
class LXWingPlane;
class LXPlayerPlane : public LXPlane
{
public:
	LXPlayerPlane();
	~LXPlayerPlane();

public:
	// ��ʼ��
	virtual void				Init(int value);
						
	// ���ݳ�ʼ��
	virtual void				InitData();

	// ���߼�ѭ��				
	virtual void				Tick(float fDelta);
								
	// ����					
	virtual void				Destroy();
	
	// �˺�
	virtual void				Hit(int damage);

	// �����Ż�
	void						AddWingPlane(LXWingPlane* pWing);

	// �ƶ�
	virtual void				Move(Vec2 delta);
	
	// ��ʼ������߼�
	virtual void				InitShootLogic();

	// ��������������Ч
	void						SetWeaponSpecialEffect();

	// ֹͣ��Ļ�����߼�
	virtual void				StopShootLogic();

public:

	// �ȼ�
	void						SetLevel(int value);
	int							GetLevel() { return m_nLevel; }

	// �ɻ�����
	void						SetPlaneType(int type);
	int							GetPlaneType(){ return m_PlaneType; }

	// ʰȡ����
	void						PickItem(LXObject *pObject);

	// ��ȡս��״̬
	void						SetFireState(bool value);
	bool						GetFireState() { return m_bAutoShoot; }
	
	// �޵�ʱ��
	void						SetInvincibleTime(float ft){ m_fInvincible = ft; }
	float						GetInvincibleTime(){ return m_fInvincible; }

	// ����ʱ��
	void						SetProtectTime(float ft){ m_fProtected = ft; }
	float						GetProtectTime() { return m_fProtected;}

	// ���ñ���״̬
	void						SetCrazyState(bool isCrazy);


	//  ��������
	void						CreatProtectEffect();

	//	����ʹ��
	void						ShowProtectEffect();

	// ���û�����Ч
	void						ClearUpProtectEffect();

	// ������Ļ
	void						RunIntoScreen();

	// ���������½��㻤��
	void						RefreshProtectWithReLife();

	// ������ҷɻ�����ʾ��������
	void						ShowPlane(bool bShow);

	//  ս��ʤ�� �� ���
	void						FighterWinAndCharge();
	
	//  ս��ʤ������
	void						FighterWin();

	// �ص�
	void						FighterWinAndChargeCallback();

	// �ص�
	void						FighterWinCallback();

	// �Ƿ����Զ��ƶ�״̬
	bool						IsAutoMove(){ return m_IsAutoMove; };

	// �����ɱ
	void						ActiveBombPlane();
	bool						IsBombPlaneActive() { return m_bBombActive; }  

protected:
	// �����������ID
	int							calcUserDataID();
	
	// ���������������ID
	int							calcShooterID();	

	// ��ʼ����Ļ
	void						initShooter();


protected:
	int					m_nLevel;			// ������ǰ�ȼ�
	int					m_PlaneType;		// �ɻ�����
	int					m_nWeaponLevel;		// �����ȼ�
	bool				m_bAutoShoot;		// �Զ����
	bool				m_isRenascenceing;	// �Ƿ�������
	
	bool				m_IsAutoMove;		// �Ƿ����Զ��ƶ�
	bool				m_bCrazyMode;		// ����ģʽ
	float				m_fInvincible;		// �޵�״̬
	float				m_fProtected;		// ����״̬
	float				m_fCrazyTime;		// ����ʱ��
	float				m_RenascenceTime;	// ����ʱ��
	
	int					m_nWingPosParam1;	// �Ż�λ�ò���1
	int					m_nWingPosParam2;	// �Ż�λ�ò���1
	float				m_fWingPosParam3;	// �Ż�λ�ò���1
	float				m_fWingPosParam4;	// �Ż�λ�ò���1

	std::vector<LXWingPlane*>	m_WingPlanes;	// �Ż�
	std::vector<int>	m_nWeaponInfo;			// ��������

	float m_WingTempXPos[10];					// �Ż�����ƫ��
	float m_WingTempYPos[10];					// �Ż�����ƫ��

	ParticleEffect*		m_ProtectEffect;		// ����������Ч
	ParticleEffect*		m_EffectY;				// ����������Ч

	unsigned int		m_nShooterEffect;		// �ӵ���Ч
	bool				m_bBombActive;			// ��ɱ����
	float				m_fBombTime;			// ��ɱ����ʱ��

};
