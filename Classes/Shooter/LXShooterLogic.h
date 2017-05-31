#pragma once
#include <string>
#include "cocos2d.h"

USING_NS_CC;
class LXObject;
class LXWeapon;
class LXShooter;
class LXShooterLogic
{
public:
	LXShooterLogic();
	~LXShooterLogic();

public:
	// ���ø������ӵ��ķ�����
	void					SetParent(LXObject* pObject) { m_pParent = pObject; } 
	LXObject*				GetParent() { return m_pParent; }

	// ��������������Ϣ
	void					SetShooter(LXShooter* pShooters) { m_pShooter = pShooters; }
	LXShooter*				GetShooter() { return m_pShooter; }

	// ����ê����Ϣ
	void					SetAnchorPointX(float x) { m_fOffX = x; }
	void					SetAnchorPointY(float y) { m_fOffY = y; }
	float					GetAnchorPointX() { return m_fOffX; }
	float					GetAnchorPointY() { return m_fOffY; }
	void					ChangeAnchorPoint(float x, float y);

	// ���÷�����Ч
	void					SetEffect(std::string& effect) { m_Effect = effect; }

public:
	// ��ʼ��������Ϊ
	void					InitAction();

	// ֹͣ����
	void					StopAction();

	// ����
	void					Fire(float fDegree, float fSpeed);

	// ���㷢��Ƕ�
	float					CalcDegree();

	// ������Ч
	void					PlayEffect();

private:
	int						m_nTag;

	LXObject*				m_pParent;		// ������
	LXWeapon*				m_pWeapon;		// ����
	LXShooter*				m_pShooter;		// ��������

	float					m_fOffX;		// ê��X
	float					m_fOffY;		// ê��Y
	std::string				m_Effect;		// �ӵ�������Ч
	float					m_fEffectDelat; // �ӵ���Ч���ż��
};
