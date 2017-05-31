#pragma once
#include "cocos2d.h"
#include "ParticleEffect.h"

USING_NS_CC;

class ParticleLogic 
{
public:
	ParticleLogic();
	~ParticleLogic ();

public:
	// �����ʼ��
	void				InitEffectObject(std::string& name, int num);

	// ��������
	ParticleEffect*		CreateEffect(Node *pNode, float x, float y, int zorder);
	void				DeleteEffect(ParticleEffect* pEffect);
	void				CleanEffect();

	// Tick
	void				Tick(float fDelta);	

	// ������ЧΪѭ����Ч
	void				SetEffectContinues();

	// ��ȡ��������
	int					GetActiveNum() { return m_nActiveNum; }

protected:
	ParticleEffect*		m_EffectList[100];

	bool				m_bLoop;
	std::string			m_Name;
	int					m_nNum;
	int					m_nActiveNum;
};
