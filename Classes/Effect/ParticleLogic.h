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
	// 对象初始化
	void				InitEffectObject(std::string& name, int num);

	// 创建对象
	ParticleEffect*		CreateEffect(Node *pNode, float x, float y, int zorder);
	void				DeleteEffect(ParticleEffect* pEffect);
	void				CleanEffect();

	// Tick
	void				Tick(float fDelta);	

	// 设置特效为循环特效
	void				SetEffectContinues();

	// 获取激活数量
	int					GetActiveNum() { return m_nActiveNum; }

protected:
	ParticleEffect*		m_EffectList[100];

	bool				m_bLoop;
	std::string			m_Name;
	int					m_nNum;
	int					m_nActiveNum;
};
